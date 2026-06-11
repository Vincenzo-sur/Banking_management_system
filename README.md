# Banking Management System
### A Complete C++ Console Application

---

## 📁 Project Structure

```
banking_management_system/
│
├── main.cpp          ← Entry point, main menu, all UI handlers
├── account.h         ← Account class declaration
├── account.cpp       ← Account class implementation
├── bank.h            ← Bank class declaration
├── bank.cpp          ← Bank class (manages all accounts)
├── admin.h           ← Admin class declaration (inherits Bank access)
├── admin.cpp         ← Admin panel with all privileged operations
├── transaction.h     ← Transaction struct & TransactionType enum
├── filemanager.h     ← FileManager class declaration
├── filemanager.cpp   ← File I/O using fstream (persistent storage)
├── build.bat         ← Windows build script (MinGW / MSVC)
└── README.md         ← This file
```

---

## 🚀 How to Build & Run

### Step 1: Install a C++ Compiler

**Option A – MinGW (Recommended for beginners)**
1. Download from https://winlibs.com/ (pick `winlibs-x86_64` UCRT ZIP)
2. Extract to `C:\mingw64`
3. Add `C:\mingw64\bin` to your **PATH** environment variable
4. Open a new terminal and verify: `g++ --version`

**Option B – MSVC (Visual Studio)**
1. Install [Visual Studio](https://visualstudio.microsoft.com/) with the **C++ build tools** workload
2. Use the "Developer Command Prompt" to compile

### Step 2: Compile

**Using the build script (Windows):**
```bat
build.bat
```

**Manual command (MinGW/g++):**
```bash
g++ -std=c++17 -Wall -O2 -o banking_system.exe \
    main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp
```

**Manual command (MSVC):**
```bat
cl /EHsc /std:c++17 /Fe:banking_system.exe ^
    main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp
```

### Step 3: Run
```bat
banking_system.exe
```

---

## 🏦 Features

| Feature                  | Description                                        |
|--------------------------|----------------------------------------------------|
| Create Account           | Name, PIN, initial deposit, auto account number    |
| Secure Login             | Account number + PIN with max 3 attempts           |
| Deposit                  | Cash deposit with validation                       |
| Withdraw                 | Prevents overdraft and negative amounts            |
| Transfer                 | Account-to-account transfer with validation        |
| Balance Inquiry          | Real-time balance display                          |
| Mini Statement           | Last N transactions in formatted table             |
| Admin Panel              | View all, search, delete, freeze/unfreeze accounts |
| Persistent Storage       | All data saved to `bank_data.txt` via fstream      |

---

## 🔐 Sample Login Credentials (first run)

| Account No | Name            | PIN  | Balance       |
|------------|-----------------|------|---------------|
| ACC-1001   | Alice Johnson   | 1234 | Rs. 7,800.00  |
| ACC-1002   | Bob Smith       | 5678 | Rs. 12,200.50 |
| ACC-1003   | Carol Williams  | 9999 | Rs. 800.00    |

**Admin Login PIN:** `admin123`

---

## 🎓 OOP Concepts Demonstrated

| Concept         | Where Used                                        |
|-----------------|---------------------------------------------------|
| Classes/Objects | `Account`, `Bank`, `Admin`, `FileManager`         |
| Constructors    | Default + parameterised in all classes            |
| Encapsulation   | Private members, public getters/setters           |
| Inheritance     | `Admin` operates on `Bank` (composition + refs)   |
| Polymorphism    | `TransactionType` enum + `txTypeToString` helper  |
| Vectors         | `vector<Account>`, `vector<Transaction>`          |
| File I/O        | `fstream` in `filemanager.cpp`                    |
| Enums           | `AccountStatus`, `TransactionType`                |
| Switch-case     | All menu systems                                  |
| Structs         | `Transaction` struct                              |

---

## 📝 Notes

- Data is saved to `bank_data.txt` in the **same folder** as the executable.
- The file is created automatically on first run.
- Admin PIN can be changed in `main.cpp` → `ADMIN_PIN` constant.
- For production use: replace plain-text PIN with a hash (e.g., SHA-256).
