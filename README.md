Simple C++ Banking System

A robust, console-based banking and wallet application written in C++. This project demonstrates fundamental Object-Oriented Programming (OOP) concepts, manual memory management, and hybrid file handling (binary and text) without relying on modern standard library containers like std::vector or std::string.

🚀 Features

User Management

Registration: Create new accounts with Name, CNIC, and a 4-digit PIN.

Authentication: Secure login system using CNIC and PIN.

Deletion: Remove users from the system (automatically clears associated transaction history).

Listing: View all registered users and their current balances.

Wallet Operations

Transactions: Deposit (Cash In) and Withdraw (Cash Out) funds.

Fund Transfer: Transfer money securely to other users via their CNIC.

Bill Payments: Specialized options to pay Utility Bills, Challans, and Fees.

Account Statement: View a detailed history of all transactions with timestamps.

PIN Management: Users can update their security PIN.

Data Persistence

Hybrid Storage:

Users: Stored in Binary format (users.dat) for efficiency and structure preservation.

History: Stored in Text format (history.txt) for readability and logging.

Auto-Save: Data is automatically saved when the program is exited properly.

Robust Validation

Input Safety: Prevents infinite loops if characters are entered where numbers are expected.

Format Checks: Enforces strict formatting for CNIC (XXXXX-XXXXXXX-X) and PIN (4 digits).

Logic Checks: Prevents negative transfers, overdrafts, and duplicate user registrations.

🛠 Technical Concepts

This project is excellent for students and developers looking to understand the low-level mechanics of C++:

Dynamic Memory Management: Uses new and delete manually.

Dynamic Arrays: Implements custom logic to resize arrays (vectors) from scratch.

Pointers & Structs: heavy usage of pointers to manage user data and transaction logs.

File I/O: Demonstrates both ifstream/ofstream in binary mode (for structs) and text mode (for logs).

C-Style Strings: Uses char arrays and cstring functions (strcpy, strcmp) instead of std::string.

📦 Getting Started

Prerequisites

You need a C++ compiler installed on your system (e.g., G++, Clang, or MSVC).

Compilation

Open your terminal or command prompt and navigate to the project directory. Run the following command:

g++ oop.cpp -o bank_system


Running the Application

Windows:

bank_system.exe


Linux/macOS:

./bank_system


📖 Usage Guide

Start the App: Run the executable.

Create User: Select Option 1. Enter a name, a valid CNIC (e.g., 35202-1234567-8), and a 4-digit PIN.

Login: Select Option 2. Enter your credentials to access the Wallet Menu.

Perform Transactions: Add money, transfer to other users, or pay bills.

Exit: Always use Option 5 ("Save and exit") from the Main Menu to ensure your data is written to disk.

📂 File Structure

oop.cpp: The main source code file.

users.dat: (Generated) Binary file storing user profiles and balances.

history.txt: (Generated) Text file storing transaction logs for all users.

⚠️ Disclaimer

This project is for educational purposes. While it implements basic PIN authentication, it stores data locally and uses a simple binary format. It is not intended for real-world banking security applications.
