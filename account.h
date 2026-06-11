// ============================================================
//  account.h  –  Account class declaration
//  Banking Management System
// ============================================================
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include "transaction.h"

// ── Account Status ────────────────────────────────────────────
enum class AccountStatus {
    ACTIVE,
    FROZEN
};

// ── Account Class ─────────────────────────────────────────────
class Account {
private:
    std::string   accountNumber;
    std::string   holderName;
    std::string   pin;           // stored as plain text (demo); use hashing in production
    double        balance;
    AccountStatus status;
    std::string   createdDate;

    // Transaction history (kept in memory; persisted via FileManager)
    std::vector<Transaction> transactions;

public:
    // ── Constructors ─────────────────────────────────────────
    Account();
    Account(const std::string& accNum,
            const std::string& name,
            const std::string& pin,
            double             initialDeposit,
            const std::string& date);

    // ── Getters ───────────────────────────────────────────────
    std::string   getAccountNumber() const;
    std::string   getHolderName()    const;
    std::string   getPin()           const;
    double        getBalance()       const;
    AccountStatus getStatus()        const;
    std::string   getCreatedDate()   const;
    const std::vector<Transaction>& getTransactions() const;

    // ── Setters ───────────────────────────────────────────────
    void setPin(const std::string& newPin);
    void setStatus(AccountStatus s);
    void setBalance(double b);
    void addTransaction(const Transaction& tx);
    void setTransactions(const std::vector<Transaction>& txList);
    void setCreatedDate(const std::string& date);
    void setHolderName(const std::string& name);

    // ── Banking Operations ────────────────────────────────────
    bool deposit(double amount, const std::string& timestamp);
    bool withdraw(double amount, const std::string& timestamp);
    bool transfer(double amount, const std::string& targetAcc,
                  const std::string& timestamp);
    void receiveTransfer(double amount, const std::string& fromAcc,
                         const std::string& timestamp);

    // ── Display Helpers ───────────────────────────────────────
    void printSummary()     const;   // one-line summary
    void printDetails()     const;   // full account details
    void printMiniStatement(int count = 5) const; // last N transactions

    // ── Validation ────────────────────────────────────────────
    bool verifyPin(const std::string& inputPin) const;
    bool isActive()  const;
    bool isFrozen()  const;
};

#endif // ACCOUNT_H
