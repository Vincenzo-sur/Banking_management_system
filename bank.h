// ============================================================
//  bank.h  –  Bank class declaration
//  Banking Management System
// ============================================================
#ifndef BANK_H
#define BANK_H

#include <vector>
#include <string>
#include "account.h"
#include "filemanager.h"

// ── Bank Class ────────────────────────────────────────────────
// Central class managing all accounts and banking operations
class Bank {
private:
    std::vector<Account> accounts;   // in-memory account store
    FileManager          fileManager;

    // ── Internal helpers ──────────────────────────────────────
    int  findAccountIndex(const std::string& accNum) const;
    std::string generateAccountNumber() const;
    std::string getCurrentTimestamp()   const;

public:
    // Constructor – loads data from disk on startup
    explicit Bank(const std::string& dataFilePath = "bank_data.txt");

    // ── Data persistence ──────────────────────────────────────
    void save() const;
    void load();

    // ── Account Management (Customer) ─────────────────────────
    bool createAccount(const std::string& name, const std::string& pin,
                       double initialDeposit);

    Account* findAccount(const std::string& accNum);  // mutable ptr
    const Account* findAccountConst(const std::string& accNum) const;

    // ── Authentication ────────────────────────────────────────
    Account* login(const std::string& accNum, const std::string& pin);

    // ── Transactions ──────────────────────────────────────────
    bool deposit(const std::string& accNum, double amount);
    bool withdraw(const std::string& accNum, double amount);
    bool transfer(const std::string& fromAcc, const std::string& toAcc, double amount);

    // ── Admin Operations ──────────────────────────────────────
    bool deleteAccount(const std::string& accNum);
    bool freezeAccount(const std::string& accNum);
    bool unfreezeAccount(const std::string& accNum);

    // ── Queries ───────────────────────────────────────────────
    const std::vector<Account>& getAllAccounts() const;
    int  getTotalAccounts() const;
    double getTotalDeposits() const;

    // ── Sample Data ───────────────────────────────────────────
    void loadSampleData();
};

#endif // BANK_H
