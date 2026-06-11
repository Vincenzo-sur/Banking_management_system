// ============================================================
//  bank.cpp  –  Bank class implementation
//  Banking Management System
// ============================================================
#include "bank.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────

Bank::Bank(const std::string& dataFilePath)
    : fileManager(dataFilePath)
{
    load();
}

// ─────────────────────────────────────────────────────────────
//  Persistence
// ─────────────────────────────────────────────────────────────

void Bank::save() const {
    fileManager.saveAccounts(accounts);
}

void Bank::load() {
    accounts = fileManager.loadAccounts();
}

// ─────────────────────────────────────────────────────────────
//  Internal helpers
// ─────────────────────────────────────────────────────────────

int Bank::findAccountIndex(const std::string& accNum) const {
    for (int i = 0; i < (int)accounts.size(); i++) {
        if (accounts[i].getAccountNumber() == accNum)
            return i;
    }
    return -1;
}

std::string Bank::generateAccountNumber() const {
    // Simple sequential number based on count
    int next = 1001 + (int)accounts.size();
    return "ACC-" + std::to_string(next);
}

std::string Bank::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm*    ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << (1900 + ltm->tm_year) << "-"
        << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon)  << "-"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday        << " "
        << std::setw(2) << std::setfill('0') << ltm->tm_hour        << ":"
        << std::setw(2) << std::setfill('0') << ltm->tm_min;
    return oss.str();
}

// ─────────────────────────────────────────────────────────────
//  Account Management
// ─────────────────────────────────────────────────────────────

bool Bank::createAccount(const std::string& name, const std::string& pin,
                         double initialDeposit) {
    if (name.empty() || pin.size() < 4) {
        std::cerr << "  [ERROR] Name cannot be empty and PIN must be at least 4 digits.\n";
        return false;
    }
    if (initialDeposit < 0) {
        std::cerr << "  [ERROR] Initial deposit cannot be negative.\n";
        return false;
    }

    std::string accNum = generateAccountNumber();
    std::string ts     = getCurrentTimestamp();
    Account newAcc(accNum, name, pin, initialDeposit, ts);
    accounts.push_back(newAcc);
    save();

    std::cout << "\n  ✔  Account created successfully!\n";
    std::cout << "     Account Number : " << accNum << "\n";
    std::cout << "     Opening Balance: Rs. " << std::fixed << std::setprecision(2)
              << initialDeposit << "\n\n";
    return true;
}

Account* Bank::findAccount(const std::string& accNum) {
    int idx = findAccountIndex(accNum);
    return (idx != -1) ? &accounts[idx] : nullptr;
}

const Account* Bank::findAccountConst(const std::string& accNum) const {
    int idx = findAccountIndex(accNum);
    return (idx != -1) ? &accounts[idx] : nullptr;
}

// ─────────────────────────────────────────────────────────────
//  Authentication
// ─────────────────────────────────────────────────────────────

Account* Bank::login(const std::string& accNum, const std::string& pin) {
    Account* acc = findAccount(accNum);
    if (!acc) {
        std::cerr << "  [ERROR] Account not found.\n";
        return nullptr;
    }
    if (!acc->verifyPin(pin)) {
        std::cerr << "  [ERROR] Incorrect PIN.\n";
        return nullptr;
    }
    return acc;
}

// ─────────────────────────────────────────────────────────────
//  Transactions
// ─────────────────────────────────────────────────────────────

bool Bank::deposit(const std::string& accNum, double amount) {
    Account* acc = findAccount(accNum);
    if (!acc) return false;
    bool ok = acc->deposit(amount, getCurrentTimestamp());
    if (ok) save();
    return ok;
}

bool Bank::withdraw(const std::string& accNum, double amount) {
    Account* acc = findAccount(accNum);
    if (!acc) return false;
    bool ok = acc->withdraw(amount, getCurrentTimestamp());
    if (ok) save();
    return ok;
}

bool Bank::transfer(const std::string& fromAcc, const std::string& toAcc, double amount) {
    if (fromAcc == toAcc) {
        std::cerr << "  [ERROR] Cannot transfer to the same account.\n";
        return false;
    }
    Account* src = findAccount(fromAcc);
    Account* dst = findAccount(toAcc);
    if (!src) { std::cerr << "  [ERROR] Source account not found.\n";      return false; }
    if (!dst) { std::cerr << "  [ERROR] Destination account not found.\n"; return false; }
    if (dst->isFrozen()) {
        std::cerr << "  [ERROR] Destination account is frozen.\n";
        return false;
    }

    std::string ts = getCurrentTimestamp();
    bool ok = src->transfer(amount, toAcc, ts);
    if (ok) {
        dst->receiveTransfer(amount, fromAcc, ts);
        save();
    }
    return ok;
}

// ─────────────────────────────────────────────────────────────
//  Admin Operations
// ─────────────────────────────────────────────────────────────

bool Bank::deleteAccount(const std::string& accNum) {
    int idx = findAccountIndex(accNum);
    if (idx == -1) {
        std::cerr << "  [ERROR] Account not found.\n";
        return false;
    }
    accounts.erase(accounts.begin() + idx);
    save();
    return true;
}

bool Bank::freezeAccount(const std::string& accNum) {
    Account* acc = findAccount(accNum);
    if (!acc) { std::cerr << "  [ERROR] Account not found.\n"; return false; }
    if (acc->isFrozen()) { std::cout << "  [INFO] Account is already frozen.\n"; return false; }
    acc->setStatus(AccountStatus::FROZEN);
    save();
    return true;
}

bool Bank::unfreezeAccount(const std::string& accNum) {
    Account* acc = findAccount(accNum);
    if (!acc) { std::cerr << "  [ERROR] Account not found.\n"; return false; }
    if (acc->isActive()) { std::cout << "  [INFO] Account is already active.\n"; return false; }
    acc->setStatus(AccountStatus::ACTIVE);
    save();
    return true;
}

// ─────────────────────────────────────────────────────────────
//  Queries
// ─────────────────────────────────────────────────────────────

const std::vector<Account>& Bank::getAllAccounts() const {
    return accounts;
}

int Bank::getTotalAccounts() const {
    return (int)accounts.size();
}

double Bank::getTotalDeposits() const {
    double total = 0.0;
    for (const Account& acc : accounts) total += acc.getBalance();
    return total;
}

// ─────────────────────────────────────────────────────────────
//  Sample Data
// ─────────────────────────────────────────────────────────────

void Bank::loadSampleData() {
    fileManager.createSampleData(accounts);
    save();
}
