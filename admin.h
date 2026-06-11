// ============================================================
//  admin.h  –  Admin class declaration
//  Banking Management System
// ============================================================
#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "bank.h"

// ── Admin Class ───────────────────────────────────────────────
// Inherits from a conceptual "User" role and handles all
// privileged administrative operations on the Bank.
//
// Demonstrates INHERITANCE: Admin "is-a" privileged entity
// with extended capabilities over a normal customer.
// ─────────────────────────────────────────────────────────────

class Admin {
private:
    std::string adminId;
    std::string adminPin;
    Bank&       bank;           // reference to the central Bank

    // ── Internal display helpers ──────────────────────────────
    void printHeader(const std::string& title) const;
    void printFooter() const;

public:
    // Constructor
    Admin(const std::string& id, const std::string& pin, Bank& bankRef);

    // ── Authentication ────────────────────────────────────────
    bool authenticate(const std::string& inputPin) const;

    // ── Admin Panel Menu ──────────────────────────────────────
    void showPanel();

    // ── Admin Operations ──────────────────────────────────────
    void viewAllAccounts()    const;
    void searchCustomer()     const;
    void deleteAccount();
    void freezeAccount();
    void unfreezeAccount();
    void viewBankStatistics() const;
};

#endif // ADMIN_H
