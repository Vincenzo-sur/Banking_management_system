// ============================================================
//  main.cpp  –  Program entry point & top-level UI
//  Banking Management System
//
//  Concepts demonstrated:
//    ✔  OOP  (Classes, Encapsulation, Inheritance)
//    ✔  File I/O with fstream
//    ✔  STL  (vector, string)
//    ✔  Switch-case menus
//    ✔  Input validation
//    ✔  Modular design (separate .h/.cpp files)
// ============================================================
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include "bank.h"
#include "admin.h"

// ─────────────────────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────────────────────
static const std::string DATA_FILE    = "bank_data.txt";
static const std::string ADMIN_ID     = "ADMIN";
static const std::string ADMIN_PIN    = "admin123";  // Change in production!
static const int         MAX_ATTEMPTS = 3;

// ─────────────────────────────────────────────────────────────
//  Utility helpers
// ─────────────────────────────────────────────────────────────

static void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void pressEnterToContinue() {
    std::cout << "\n  Press ENTER to continue...";
    std::cin.get();
}

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void printBanner() {
    std::cout << "\n";
    std::cout << "  +------------------------------------------------------+\n";
    std::cout << "  |                                                      |\n";
    std::cout << "  |          BANKING MANAGEMENT SYSTEM  v1.0            |\n";
    std::cout << "  |          Powered by C++ | Secure & Reliable         |\n";
    std::cout << "  |          Made by Suryanshu Singh                    |\n";
    std::cout << "  |                                                      |\n";
    std::cout << "  +------------------------------------------------------+\n\n";
}

static void printMainMenu() {
    std::cout << "  +------------------------------------------+\n";
    std::cout << "  |             MAIN MENU                    |\n";
    std::cout << "  +------------------------------------------+\n";
    std::cout << "  |  [1]  Create Account                     |\n";
    std::cout << "  |  [2]  Customer Login                     |\n";
    std::cout << "  |  [3]  Admin Panel                        |\n";
    std::cout << "  |  [4]  Transfer Funds                     |\n";
    std::cout << "  |  [0]  Exit                               |\n";
    std::cout << "  +------------------------------------------+\n";
    std::cout << "  Choice: ";
}

static void printCustomerMenu(const std::string& accNum) {
    std::cout << "\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |  CUSTOMER MENU   [" << std::left << std::setw(12) << accNum << "]      |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  |  [1]  Balance Inquiry                      |\n";
    std::cout << "  |  [2]  Deposit Money                        |\n";
    std::cout << "  |  [3]  Withdraw Money                       |\n";
    std::cout << "  |  [4]  Transfer Money                       |\n";
    std::cout << "  |  [5]  Mini Statement                       |\n";
    std::cout << "  |  [6]  Full Account Details                 |\n";
    std::cout << "  |  [0]  Logout                               |\n";
    std::cout << "  +--------------------------------------------+\n";
    std::cout << "  Choice: ";
}

// ─────────────────────────────────────────────────────────────
//  Feature: Transfer Funds (direct from main menu)
// ─────────────────────────────────────────────────────────────

static void handleTransferFunds(Bank& bank) {
    std::cout << "\n  +----------------------------------------+\n";
    std::cout << "  |          TRANSFER FUNDS                |\n";
    std::cout << "  +----------------------------------------+\n\n";

    std::string fromAcc, toAcc, pin;
    double amount;

    std::cout << "  Sender Account Number   : ";
    std::getline(std::cin, fromAcc);

    std::cout << "  Sender PIN              : ";
    std::getline(std::cin, pin);

    // Verify sender account and PIN
    Account* sender = bank.login(fromAcc, pin);
    if (!sender) {
        std::cout << "  [ERROR] Invalid account number or PIN.\n";
        return;
    }
    if (sender->isFrozen()) {
        std::cout << "  [ERROR] Sender account is FROZEN. Transfer not allowed.\n";
        return;
    }

    std::cout << "  Receiver Account Number : ";
    std::getline(std::cin, toAcc);

    if (fromAcc == toAcc) {
        std::cout << "  [ERROR] Sender and receiver cannot be the same account.\n";
        return;
    }

    std::cout << "  Transfer Amount (Rs.)   : ";
    if (!(std::cin >> amount)) {
        std::cout << "  [ERROR] Invalid amount.\n";
        std::cin.clear();
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount <= 0) {
        std::cout << "  [ERROR] Amount must be positive.\n";
        return;
    }

    // Confirm before transfer
    std::cout << "\n  --- Transfer Summary ---\n";
    std::cout << "  From    : " << fromAcc << " (" << sender->getHolderName() << ")\n";
    std::cout << "  To      : " << toAcc << "\n";
    std::cout << "  Amount  : Rs. " << std::fixed << std::setprecision(2) << amount << "\n";
    std::cout << "\n  Confirm transfer? (yes/no): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "yes" && confirm != "YES") {
        std::cout << "  [INFO] Transfer cancelled.\n";
        return;
    }

    if (bank.transfer(fromAcc, toAcc, amount)) {
        std::cout << "  [OK] Rs. " << std::fixed << std::setprecision(2)
                  << amount << " transferred successfully from "
                  << fromAcc << " to " << toAcc << ".\n";
        std::cout << "  Remaining balance in " << fromAcc << ": Rs. "
                  << std::fixed << std::setprecision(2)
                  << sender->getBalance() << "\n";
    }
}

// ─────────────────────────────────────────────────────────────
//  Feature: Create Account
// ─────────────────────────────────────────────────────────────

static void handleCreateAccount(Bank& bank) {
    std::cout << "\n  +----------------------------------------+\n";
    std::cout << "  |         CREATE NEW ACCOUNT             |\n";
    std::cout << "  +----------------------------------------+\n\n";

    std::string name, pin, confirmPin;
    double      initialDeposit;

    // Get name
    std::cout << "  Full Name      : ";
    std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "  [ERROR] Name cannot be empty.\n";
        return;
    }

    // Get PIN (min 4 digits)
    do {
        std::cout << "  Set PIN (min 4 digits): ";
        std::getline(std::cin, pin);
        if (pin.size() < 4) std::cout << "  [!] PIN too short. Try again.\n";
    } while (pin.size() < 4);

    // Confirm PIN
    std::cout << "  Confirm PIN    : ";
    std::getline(std::cin, confirmPin);
    if (pin != confirmPin) {
        std::cout << "  [ERROR] PINs do not match.\n";
        return;
    }

    // Initial deposit
    std::cout << "  Initial Deposit (Rs.): ";
    if (!(std::cin >> initialDeposit)) {
        std::cout << "  [ERROR] Invalid amount.\n";
        std::cin.clear();
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (initialDeposit < 0) {
        std::cout << "  [ERROR] Initial deposit cannot be negative.\n";
        return;
    }

    bank.createAccount(name, pin, initialDeposit);
}

// ─────────────────────────────────────────────────────────────
//  Feature: Customer Login & Operations
// ─────────────────────────────────────────────────────────────

static void handleCustomerLogin(Bank& bank) {
    std::cout << "\n  +----------------------------------------+\n";
    std::cout << "  |            CUSTOMER LOGIN              |\n";
    std::cout << "  +----------------------------------------+\n\n";

    std::string accNum, pin;
    Account*    acc = nullptr;
    int         attempts = 0;

    // Allow up to MAX_ATTEMPTS login tries
    while (attempts < MAX_ATTEMPTS) {
        std::cout << "  Account Number : ";
        std::getline(std::cin, accNum);
        std::cout << "  PIN            : ";
        std::getline(std::cin, pin);

        acc = bank.login(accNum, pin);
        if (acc) break;

        attempts++;
        std::cout << "  [!] Login failed. Attempt " << attempts << "/" << MAX_ATTEMPTS << "\n\n";
    }

    if (!acc) {
        std::cout << "  [SECURITY] Too many failed attempts. Returning to menu.\n";
        return;
    }

    std::cout << "\n  [OK] Welcome, " << acc->getHolderName() << "!\n";

    if (acc->isFrozen()) {
        std::cout << "  [!] Your account is currently FROZEN.\n"
                  << "     Please contact the bank.\n";
        pressEnterToContinue();
        return;
    }

    // ── Customer Operations Loop ──────────────────────────────
    int choice = -1;
    do {
        printCustomerMenu(acc->getAccountNumber());
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            // ── Balance Inquiry ───────────────────────────────
            case 1:
                std::cout << "\n  Current Balance: Rs. "
                          << std::fixed << std::setprecision(2)
                          << acc->getBalance() << "\n";
                pressEnterToContinue();
                break;

            // ── Deposit ───────────────────────────────────────
            case 2: {
                double amount;
                std::cout << "  Enter deposit amount: Rs. ";
                if (!(std::cin >> amount)) {
                    std::cout << "  [ERROR] Invalid amount.\n";
                    std::cin.clear();
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                if (bank.deposit(acc->getAccountNumber(), amount)) {
                    std::cout << "  [OK] Rs. " << std::fixed << std::setprecision(2)
                              << amount << " deposited. New balance: Rs. "
                              << acc->getBalance() << "\n";
                }
                pressEnterToContinue();
                break;
            }

            // ── Withdraw ──────────────────────────────────────
            case 3: {
                double amount;
                std::cout << "  Enter withdrawal amount: Rs. ";
                if (!(std::cin >> amount)) {
                    std::cout << "  [ERROR] Invalid amount.\n";
                    std::cin.clear();
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                if (bank.withdraw(acc->getAccountNumber(), amount)) {
                    std::cout << "  [OK] Rs. " << std::fixed << std::setprecision(2)
                              << amount << " withdrawn. Remaining balance: Rs. "
                              << acc->getBalance() << "\n";
                }
                pressEnterToContinue();
                break;
            }

            // ── Transfer ──────────────────────────────────────
            case 4: {
                std::string toAcc;
                double      amount;
                std::cout << "  Destination Account Number: ";
                std::getline(std::cin, toAcc);
                std::cout << "  Transfer Amount: Rs. ";
                if (!(std::cin >> amount)) {
                    std::cout << "  [ERROR] Invalid amount.\n";
                    std::cin.clear();
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                if (bank.transfer(acc->getAccountNumber(), toAcc, amount)) {
                    std::cout << "  [OK] Rs. " << std::fixed << std::setprecision(2)
                              << amount << " transferred to " << toAcc << ".\n"
                              << "     Remaining balance: Rs. " << acc->getBalance() << "\n";
                }
                pressEnterToContinue();
                break;
            }

            // ── Mini Statement ────────────────────────────────
            case 5:
                acc->printMiniStatement(10);
                pressEnterToContinue();
                break;

            // ── Full Account Details ──────────────────────────
            case 6:
                acc->printDetails();
                acc->printMiniStatement(20);
                pressEnterToContinue();
                break;

            case 0:
                std::cout << "\n  Logging out... Goodbye, " << acc->getHolderName() << "!\n";
                break;

            default:
                std::cout << "  [!] Invalid option. Please try again.\n";
        }
    } while (choice != 0);
}

// ─────────────────────────────────────────────────────────────
//  Feature: Admin Login
// ─────────────────────────────────────────────────────────────

static void handleAdminLogin(Bank& bank) {
    std::cout << "\n  +----------------------------------------+\n";
    std::cout << "  |            ADMIN LOGIN                 |\n";
    std::cout << "  +----------------------------------------+\n\n";

    Admin admin(ADMIN_ID, ADMIN_PIN, bank);

    std::string inputPin;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "  Admin PIN: ";
        std::getline(std::cin, inputPin);

        if (admin.authenticate(inputPin)) {
            std::cout << "  [OK] Admin authenticated.\n";
            admin.showPanel();
            return;
        }

        attempts++;
        std::cout << "  [!] Incorrect PIN. Attempt " << attempts << "/" << MAX_ATTEMPTS << "\n";
    }

    std::cout << "  [SECURITY] Admin access denied. Too many failed attempts.\n";
}

// ─────────────────────────────────────────────────────────────
//  Main Entry Point
// ─────────────────────────────────────────────────────────────

int main() {
    clearScreen();
    printBanner();

    // Initialise Bank (silently loads or creates sample data)
    Bank bank(DATA_FILE);
    if (bank.getTotalAccounts() == 0) {
        bank.loadSampleData();
    }

    // ── Main Menu Loop ────────────────────────────────────────
    int choice = -1;
    do {
        clearScreen();
        printBanner();
        printMainMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                handleCreateAccount(bank);
                pressEnterToContinue();
                break;
            case 2:
                handleCustomerLogin(bank);
                break;
            case 3:
                handleAdminLogin(bank);
                break;
            case 4:
                handleTransferFunds(bank);
                pressEnterToContinue();
                break;
            case 0:
                std::cout << "\n  Thank you for using BANK Management System.\n";
                std::cout << "  All data saved. Goodbye!\n\n";
                break;
            default:
                std::cout << "  [!] Invalid option. Please choose 0-4.\n";
                pressEnterToContinue();
        }
    } while (choice != 0);

    return 0;
}
