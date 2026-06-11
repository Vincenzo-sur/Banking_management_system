// ============================================================
//  filemanager.h  –  FileManager class declaration
//  Banking Management System
// ============================================================
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "account.h"

// ── FileManager Class ─────────────────────────────────────────
// Handles all persistent storage (read/write) via fstream
class FileManager {
private:
    std::string dataFile;   // path to accounts data file

    // ── Internal helpers ──────────────────────────────────────
    std::string encodeString(const std::string& s) const; // replace spaces for storage
    std::string decodeString(const std::string& s) const;
    std::string txTypeToCode(TransactionType t) const;
    TransactionType codeToTxType(const std::string& code) const;

public:
    explicit FileManager(const std::string& filePath = "bank_data.txt");

    // ── Core Operations ───────────────────────────────────────
    bool                saveAccounts(const std::vector<Account>& accounts) const;
    std::vector<Account> loadAccounts() const;

    // ── Utility ───────────────────────────────────────────────
    bool fileExists() const;
    void createSampleData(std::vector<Account>& accounts) const;
};

#endif // FILEMANAGER_H
