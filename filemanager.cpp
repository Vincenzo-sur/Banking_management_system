// ============================================================
//  filemanager.cpp  –  FileManager class implementation
//  Banking Management System
// ============================================================
#include "filemanager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// ─────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────

FileManager::FileManager(const std::string& filePath)
    : dataFile(filePath) {}

// ─────────────────────────────────────────────────────────────
//  String encode/decode  (replace space with underscores in file)
// ─────────────────────────────────────────────────────────────

std::string FileManager::encodeString(const std::string& s) const {
    std::string result = s;
    std::replace(result.begin(), result.end(), ' ', '_');
    return result;
}

std::string FileManager::decodeString(const std::string& s) const {
    std::string result = s;
    std::replace(result.begin(), result.end(), '_', ' ');
    return result;
}

// ─────────────────────────────────────────────────────────────
//  Transaction type serialisation
// ─────────────────────────────────────────────────────────────

std::string FileManager::txTypeToCode(TransactionType t) const {
    switch (t) {
        case TransactionType::DEPOSIT:          return "DEP";
        case TransactionType::WITHDRAWAL:       return "WTH";
        case TransactionType::TRANSFER_IN:      return "TIN";
        case TransactionType::TRANSFER_OUT:     return "TOT";
        case TransactionType::ACCOUNT_CREATED:  return "OPN";
        default:                                return "UNK";
    }
}

TransactionType FileManager::codeToTxType(const std::string& code) const {
    if (code == "DEP") return TransactionType::DEPOSIT;
    if (code == "WTH") return TransactionType::WITHDRAWAL;
    if (code == "TIN") return TransactionType::TRANSFER_IN;
    if (code == "TOT") return TransactionType::TRANSFER_OUT;
    if (code == "OPN") return TransactionType::ACCOUNT_CREATED;
    return TransactionType::DEPOSIT; // fallback
}

// ─────────────────────────────────────────────────────────────
//  Save all accounts to file
// ─────────────────────────────────────────────────────────────
// File format (plain text, one account per block):
//   ACCOUNT
//   accNum holderName pin balance status createdDate
//   TRANSACTIONS <count>
//   type amount balanceAfter description timestamp   (one per line)
//   END
// ─────────────────────────────────────────────────────────────

bool FileManager::saveAccounts(const std::vector<Account>& accounts) const {
    std::ofstream ofs(dataFile, std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "[FileManager] ERROR: Cannot open file for writing: " << dataFile << "\n";
        return false;
    }

    for (const Account& acc : accounts) {
        // Header line
        ofs << "ACCOUNT\n";

        // Account meta
        std::string statusCode = (acc.getStatus() == AccountStatus::ACTIVE) ? "ACTIVE" : "FROZEN";
        ofs << encodeString(acc.getAccountNumber()) << " "
            << encodeString(acc.getHolderName())    << " "
            << encodeString(acc.getPin())           << " "
            << acc.getBalance()                     << " "
            << statusCode                           << " "
            << encodeString(acc.getCreatedDate())   << "\n";

        // Transactions
        const auto& txs = acc.getTransactions();
        ofs << "TRANSACTIONS " << txs.size() << "\n";
        for (const Transaction& tx : txs) {
            ofs << txTypeToCode(tx.type)           << " "
                << tx.amount                       << " "
                << tx.balanceAfter                 << " "
                << encodeString(tx.description)    << " "
                << encodeString(tx.timestamp)      << "\n";
        }

        ofs << "END\n";
    }

    ofs.close();
    return true;
}

// ─────────────────────────────────────────────────────────────
//  Load all accounts from file
// ─────────────────────────────────────────────────────────────

std::vector<Account> FileManager::loadAccounts() const {
    std::vector<Account> accounts;

    std::ifstream ifs(dataFile);
    if (!ifs.is_open()) {
        // File doesn't exist yet – first run
        return accounts;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        if (line == "ACCOUNT") {
            Account acc;

            // Read account meta
            std::string accNum, name, pin, statusStr, date;
            double balance;
            if (!(ifs >> accNum >> name >> pin >> balance >> statusStr >> date)) break;
            std::getline(ifs, line); // consume rest of meta line

            acc.setBalance(balance);
            acc.setPin(pin);
            acc.setHolderName(decodeString(name));
            acc.setCreatedDate(decodeString(date));
            // Re-set account number (no setter provided — use workaround via temporary Account)
            // We build a fresh Account and copy fields into it
            Account tmpAcc(decodeString(accNum), decodeString(name), pin, 0.0, decodeString(date));
            tmpAcc.setBalance(balance);
            tmpAcc.setStatus(statusStr == "FROZEN" ? AccountStatus::FROZEN : AccountStatus::ACTIVE);

            // Read transactions
            std::string txHeader;
            if (!std::getline(ifs, txHeader)) break;
            std::istringstream txss(txHeader);
            std::string txWord;
            int txCount = 0;
            txss >> txWord >> txCount; // "TRANSACTIONS N"

            std::vector<Transaction> txList;
            for (int i = 0; i < txCount; i++) {
                std::string typeCode, desc, ts;
                double amt, balAfter;
                if (!(ifs >> typeCode >> amt >> balAfter >> desc >> ts)) break;
                Transaction tx(codeToTxType(typeCode), amt, balAfter,
                               decodeString(desc), decodeString(ts));
                txList.push_back(tx);
            }
            tmpAcc.setTransactions(txList);
            std::getline(ifs, line); // consume newline after last tx
            std::getline(ifs, line); // consume "END"

            accounts.push_back(tmpAcc);
        }
    }

    ifs.close();
    return accounts;
}

// ─────────────────────────────────────────────────────────────
//  Utility
// ─────────────────────────────────────────────────────────────

bool FileManager::fileExists() const {
    std::ifstream ifs(dataFile);
    return ifs.good();
}

void FileManager::createSampleData(std::vector<Account>& accounts) const {
    // Sample accounts for demonstration
    Account a1("ACC-1001", "Suryanshu Singh", "1234", 5000.00, "2024-01-15 09:00");
    Account a2("ACC-1002", "Priyam Prakash",  "5678", 12500.50,"2024-02-20 14:30");
    Account a3("ACC-1003", "Aryan Yadav",     "9999", 800.00,  "2024-03-05 11:15");

    // Simulate some transaction history for Suryanshu
    a1.deposit(2000.0,  "2024-02-01 10:00");
    a1.withdraw(500.0,  "2024-02-10 15:45");
    a1.deposit(1000.0,  "2024-03-01 09:30");

    // Priyam sends money to Suryanshu
    a2.transfer(300.0, "ACC-1001", "2024-03-10 12:00");
    a1.receiveTransfer(300.0, "ACC-1002", "2024-03-10 12:00");

    accounts.push_back(a1);
    accounts.push_back(a2);
    accounts.push_back(a3);
}
