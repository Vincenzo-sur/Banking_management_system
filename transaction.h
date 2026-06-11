// ============================================================
//  transaction.h  –  Transaction record definition
//  Banking Management System
// ============================================================
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// ── Transaction Types ────────────────────────────────────────
enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_IN,
    TRANSFER_OUT,
    ACCOUNT_CREATED
};

// ── Helper: convert enum → readable string ───────────────────
inline std::string txTypeToString(TransactionType t) {
    switch (t) {
        case TransactionType::DEPOSIT:          return "DEPOSIT";
        case TransactionType::WITHDRAWAL:       return "WITHDRAWAL";
        case TransactionType::TRANSFER_IN:      return "TRANSFER IN";
        case TransactionType::TRANSFER_OUT:     return "TRANSFER OUT";
        case TransactionType::ACCOUNT_CREATED:  return "ACCOUNT OPENED";
        default:                                return "UNKNOWN";
    }
}

// ── Transaction Struct ────────────────────────────────────────
struct Transaction {
    TransactionType type;
    double          amount;
    double          balanceAfter;
    std::string     description;   // e.g. "Transfer to ACC-1002"
    std::string     timestamp;     // stored as formatted string

    // Default constructor
    Transaction()
        : type(TransactionType::DEPOSIT), amount(0.0), balanceAfter(0.0) {}

    // Parameterised constructor
    Transaction(TransactionType t,
                double          amt,
                double          bal,
                const std::string& desc,
                const std::string& ts)
        : type(t), amount(amt), balanceAfter(bal),
          description(desc), timestamp(ts) {}
};

#endif // TRANSACTION_H
