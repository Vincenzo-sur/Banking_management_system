// ============================================================
//  account.cpp  –  Account class implementation
//  Banking Management System
// ============================================================
#include "account.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ─────────────────────────────────────────────────────────────
//  Constructors
// ─────────────────────────────────────────────────────────────

Account::Account()
    : accountNumber(""), holderName(""), pin(""),
      balance(0.0), status(AccountStatus::ACTIVE), createdDate("") {}

Account::Account(const std::string& accNum,
                 const std::string& name,
                 const std::string& pinStr,
                 double             initialDeposit,
                 const std::string& date)
    : accountNumber(accNum), holderName(name), pin(pinStr),
      balance(initialDeposit), status(AccountStatus::ACTIVE), createdDate(date)
{
    // Record the opening transaction
    Transaction openTx(
        TransactionType::ACCOUNT_CREATED,
        initialDeposit,
        balance,
        "Account Opened",
        date
    );
    transactions.push_back(openTx);
}

// ─────────────────────────────────────────────────────────────
//  Getters
// ─────────────────────────────────────────────────────────────

std::string   Account::getAccountNumber() const { return accountNumber; }
std::string   Account::getHolderName()    const { return holderName;    }
std::string   Account::getPin()           const { return pin;           }
double        Account::getBalance()       const { return balance;        }
AccountStatus Account::getStatus()        const { return status;         }
std::string   Account::getCreatedDate()   const { return createdDate;    }

const std::vector<Transaction>& Account::getTransactions() const {
    return transactions;
}

// ─────────────────────────────────────────────────────────────
//  Setters
// ─────────────────────────────────────────────────────────────

void Account::setPin(const std::string& newPin)      { pin     = newPin;  }
void Account::setStatus(AccountStatus s)              { status  = s;       }
void Account::setBalance(double b)                    { balance = b;       }
void Account::setHolderName(const std::string& name)  { holderName = name; }
void Account::setCreatedDate(const std::string& date) { createdDate = date;}

void Account::addTransaction(const Transaction& tx) {
    transactions.push_back(tx);
}

void Account::setTransactions(const std::vector<Transaction>& txList) {
    transactions = txList;
}

// ─────────────────────────────────────────────────────────────
//  Validation helpers
// ─────────────────────────────────────────────────────────────

bool Account::verifyPin(const std::string& inputPin) const {
    return pin == inputPin;
}

bool Account::isActive()  const { return status == AccountStatus::ACTIVE; }
bool Account::isFrozen()  const { return status == AccountStatus::FROZEN; }

// ─────────────────────────────────────────────────────────────
//  Banking Operations
// ─────────────────────────────────────────────────────────────

bool Account::deposit(double amount, const std::string& timestamp) {
    if (amount <= 0) {
        std::cerr << "  [ERROR] Deposit amount must be positive.\n";
        return false;
    }
    if (isFrozen()) {
        std::cerr << "  [ERROR] Account is frozen. Operation not allowed.\n";
        return false;
    }
    balance += amount;
    Transaction tx(TransactionType::DEPOSIT, amount, balance, "Cash Deposit", timestamp);
    transactions.push_back(tx);
    return true;
}

bool Account::withdraw(double amount, const std::string& timestamp) {
    if (amount <= 0) {
        std::cerr << "  [ERROR] Withdrawal amount must be positive.\n";
        return false;
    }
    if (isFrozen()) {
        std::cerr << "  [ERROR] Account is frozen. Operation not allowed.\n";
        return false;
    }
    if (amount > balance) {
        std::cerr << "  [ERROR] Insufficient funds. Available balance: "
                  << std::fixed << std::setprecision(2) << balance << "\n";
        return false;
    }
    balance -= amount;
    Transaction tx(TransactionType::WITHDRAWAL, amount, balance, "Cash Withdrawal", timestamp);
    transactions.push_back(tx);
    return true;
}

bool Account::transfer(double amount, const std::string& targetAcc,
                       const std::string& timestamp) {
    if (amount <= 0) {
        std::cerr << "  [ERROR] Transfer amount must be positive.\n";
        return false;
    }
    if (isFrozen()) {
        std::cerr << "  [ERROR] Account is frozen. Operation not allowed.\n";
        return false;
    }
    if (amount > balance) {
        std::cerr << "  [ERROR] Insufficient funds for transfer.\n";
        return false;
    }
    balance -= amount;
    Transaction tx(TransactionType::TRANSFER_OUT, amount, balance,
                   "Transfer to " + targetAcc, timestamp);
    transactions.push_back(tx);
    return true;
}

void Account::receiveTransfer(double amount, const std::string& fromAcc,
                              const std::string& timestamp) {
    balance += amount;
    Transaction tx(TransactionType::TRANSFER_IN, amount, balance,
                   "Transfer from " + fromAcc, timestamp);
    transactions.push_back(tx);
}

// ─────────────────────────────────────────────────────────────
//  Display Helpers
// ─────────────────────────────────────────────────────────────

void Account::printSummary() const {
    std::string statusStr = (status == AccountStatus::ACTIVE) ? "ACTIVE" : "FROZEN";
    std::cout << std::left
              << std::setw(14) << accountNumber
              << std::setw(22) << holderName
              << std::setw(12) << std::fixed << std::setprecision(2) << balance
              << std::setw(10) << statusStr
              << "\n";
}

void Account::printDetails() const {
    std::string statusStr = (status == AccountStatus::ACTIVE) ? "ACTIVE" : "FROZEN";
    std::cout << "\n";
    std::cout << "  +-----------------------------------------+\n";
    std::cout << "  |          ACCOUNT DETAILS                |\n";
    std::cout << "  +-----------------------------------------+\n";
    std::cout << "  |  Account No : " << std::left << std::setw(27) << accountNumber << "|\n";
    std::cout << "  |  Name       : " << std::left << std::setw(27) << holderName    << "|\n";
    std::cout << "  |  Balance    : Rs. " << std::left << std::setw(23)
              << (std::to_string((int)balance) + "." +
                  (balance - (int)balance < 0.1 ? "0" : "") +
                  std::to_string((int)((balance - (int)balance) * 100))) << "|\n";
    std::cout << "  |  Status     : " << std::left << std::setw(27) << statusStr    << "|\n";
    std::cout << "  |  Opened On  : " << std::left << std::setw(27) << createdDate  << "|\n";
    std::cout << "  +-----------------------------------------+\n";
}

void Account::printMiniStatement(int count) const {
    std::cout << "\n  +----------------------------------------------------------------+\n";
    std::cout << "  |                   MINI STATEMENT                               |\n";
    std::cout << "  |  Account: " << std::left << std::setw(52) << accountNumber << "  |\n";
    std::cout << "  +----------------+--------------+-------------+---------------+\n";
    std::cout << "  | Date/Time      | Type         | Amount      | Balance       |\n";
    std::cout << "  +----------------+--------------+-------------+---------------+\n";

    int total = (int)transactions.size();
    int start = std::max(0, total - count);

    for (int i = start; i < total; i++) {
        const Transaction& tx = transactions[i];
        std::string ts = tx.timestamp;
        if (ts.size() > 14) ts = ts.substr(0, 14);

        std::cout << "  | " << std::left << std::setw(15) << ts
                  << "| " << std::setw(13) << txTypeToString(tx.type)
                  << "| " << std::right << std::setw(11)
                  << std::fixed << std::setprecision(2) << tx.amount
                  << " | " << std::setw(13) << tx.balanceAfter << " |\n";
    }

    std::cout << "  +----------------+--------------+-------------+---------------+\n";
    std::cout << "  Showing last " << (total - start) << " of " << total << " transaction(s).\n\n";
}
