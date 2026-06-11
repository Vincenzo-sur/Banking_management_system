// ============================================================
//  admin.cpp  –  Admin class implementation
//  Banking Management System
// ============================================================
#include "admin.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <limits>

// ─────────────────────────────────────────────────────────────
//  Utility: clear input buffer
// ─────────────────────────────────────────────────────────────
static void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────

Admin::Admin(const std::string& id, const std::string& pin, Bank& bankRef)
    : adminId(id), adminPin(pin), bank(bankRef) {}

// ─────────────────────────────────────────────────────────────
//  Authentication
// ─────────────────────────────────────────────────────────────

bool Admin::authenticate(const std::string& inputPin) const {
    return adminPin == inputPin;
}

// ─────────────────────────────────────────────────────────────
//  Display helpers
// ─────────────────────────────────────────────────────────────

void Admin::printHeader(const std::string& title) const {
    std::cout << "\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |  " << std::left << std::setw(43) << title << "|\n";
    std::cout << "  +--------------------------------------------+\n\n";
}

void Admin::printFooter() const {
    std::cout << "\n  --------------------------------------------\n";
}

// ─────────────────────────────────────────────────────────────
//  Admin Panel  (main loop)
// ─────────────────────────────────────────────────────────────

void Admin::showPanel() {
    int choice = 0;
    do {
        std::cout << "\n";
        std::cout << "  +--------------------------------------------+\n";
        std::cout << "  |          *  ADMIN PANEL  *                 |\n";
        std::cout << "  +--------------------------------------------+\n";
        std::cout << "  |  [1]  View All Accounts                    |\n";
        std::cout << "  |  [2]  Search Customer                      |\n";
        std::cout << "  |  [3]  Delete Account                       |\n";
        std::cout << "  |  [4]  Freeze Account                       |\n";
        std::cout << "  |  [5]  Unfreeze Account                     |\n";
        std::cout << "  |  [6]  Bank Statistics                      |\n";
        std::cout << "  |  [0]  Back to Main Menu                    |\n";
        std::cout << "  +--------------------------------------------+\n";
        std::cout << "  Enter choice: ";
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: viewAllAccounts();    break;
            case 2: searchCustomer();     break;
            case 3: deleteAccount();      break;
            case 4: freezeAccount();      break;
            case 5: unfreezeAccount();    break;
            case 6: viewBankStatistics(); break;
            case 0: std::cout << "  Returning to main menu...\n"; break;
            default:
                std::cout << "  [!] Invalid option. Please try again.\n";
        }
    } while (choice != 0);
}

// ─────────────────────────────────────────────────────────────
//  View All Accounts
// ─────────────────────────────────────────────────────────────

void Admin::viewAllAccounts() const {
    printHeader("ALL ACCOUNTS");

    const auto& accounts = bank.getAllAccounts();
    if (accounts.empty()) {
        std::cout << "  No accounts found.\n";
        printFooter();
        return;
    }

    // Table header
    std::cout << "  " << std::left
              << std::setw(14) << "Account No"
              << std::setw(22) << "Holder Name"
              << std::setw(14) << "Balance (Rs)"
              << std::setw(10) << "Status"
              << "\n";
    std::cout << "  " << std::string(58, '-') << "\n";

    for (const Account& acc : accounts) {
        acc.printSummary();
    }

    printFooter();
    std::cout << "  Total Accounts: " << accounts.size() << "\n";
}

// ─────────────────────────────────────────────────────────────
//  Search Customer
// ─────────────────────────────────────────────────────────────

void Admin::searchCustomer() const {
    printHeader("SEARCH CUSTOMER");

    std::cout << "  Search by:\n"
              << "  [1] Account Number\n"
              << "  [2] Name (partial match)\n"
              << "  Choice: ";
    int opt; std::cin >> opt; clearInputBuffer();

    const auto& accounts = bank.getAllAccounts();
    bool found = false;

    if (opt == 1) {
        std::cout << "  Enter Account Number: ";
        std::string accNum; std::getline(std::cin, accNum);
        const Account* acc = bank.findAccountConst(accNum);
        if (acc) {
            acc->printDetails();
            acc->printMiniStatement(10);
            found = true;
        }
    } else if (opt == 2) {
        std::cout << "  Enter Name (partial): ";
        std::string keyword; std::getline(std::cin, keyword);
        // Case-insensitive partial match
        std::string kwLower = keyword;
        std::transform(kwLower.begin(), kwLower.end(), kwLower.begin(), ::tolower);

        for (const Account& acc : accounts) {
            std::string nameLower = acc.getHolderName();
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(kwLower) != std::string::npos) {
                acc.printSummary();
                found = true;
            }
        }
    } else {
        std::cout << "  [!] Invalid option.\n";
        return;
    }

    if (!found) std::cout << "  [!] No matching customer found.\n";
    printFooter();
}

// ─────────────────────────────────────────────────────────────
//  Delete Account
// ─────────────────────────────────────────────────────────────

void Admin::deleteAccount() {
    printHeader("DELETE ACCOUNT");

    std::cout << "  Enter Account Number to delete: ";
    std::string accNum; std::getline(std::cin, accNum);

    const Account* acc = bank.findAccountConst(accNum);
    if (!acc) {
        std::cout << "  [ERROR] Account not found.\n";
        return;
    }

    // Show account before deletion and confirm
    acc->printDetails();
    std::cout << "\n  [!] Are you sure you want to DELETE this account? (yes/no): ";
    std::string confirm; std::getline(std::cin, confirm);

    if (confirm == "yes" || confirm == "YES") {
        if (bank.deleteAccount(accNum)) {
            std::cout << "  [OK] Account " << accNum << " deleted successfully.\n";
        }
    } else {
        std::cout << "  [INFO] Deletion cancelled.\n";
    }
    printFooter();
}

// ─────────────────────────────────────────────────────────────
//  Freeze / Unfreeze
// ─────────────────────────────────────────────────────────────

void Admin::freezeAccount() {
    printHeader("FREEZE ACCOUNT");

    std::cout << "  Enter Account Number to freeze: ";
    std::string accNum; std::getline(std::cin, accNum);

    if (bank.freezeAccount(accNum)) {
        std::cout << "  [OK] Account " << accNum << " has been FROZEN.\n";
    }
    printFooter();
}

void Admin::unfreezeAccount() {
    printHeader("UNFREEZE ACCOUNT");

    std::cout << "  Enter Account Number to unfreeze: ";
    std::string accNum; std::getline(std::cin, accNum);

    if (bank.unfreezeAccount(accNum)) {
        std::cout << "  [OK] Account " << accNum << " is now ACTIVE.\n";
    }
    printFooter();
}

// ─────────────────────────────────────────────────────────────
//  Bank Statistics
// ─────────────────────────────────────────────────────────────

void Admin::viewBankStatistics() const {
    printHeader("BANK STATISTICS");

    std::cout << "  Total Accounts  : " << bank.getTotalAccounts() << "\n";
    std::cout << "  Total Deposits  : Rs. "
              << std::fixed << std::setprecision(2) << bank.getTotalDeposits() << "\n";

    // Count active vs frozen
    int active = 0, frozen = 0;
    for (const Account& acc : bank.getAllAccounts()) {
        if (acc.isActive()) active++; else frozen++;
    }
    std::cout << "  Active Accounts : " << active << "\n";
    std::cout << "  Frozen Accounts : " << frozen << "\n";

    printFooter();
}
