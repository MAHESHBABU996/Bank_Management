# Bank_Management
🏦 Bank Management System

A Console-Based Banking Application in C

📌 📖 Overview

The Bank Management System is a console-based C project that allows users to perform essential banking operations such as creating accounts, depositing/withdrawing money, checking balances, updating account details, and securely storing records in files.

This project is ideal for beginners learning file handling and for mini-project submissions in colleges.

🚀 ✨ Features
✔ Account Operations

Create a new bank account

Display existing account details

Update customer information

Close/delete an account

✔ Transactions

Deposit money

Withdraw money

Check account balance

Transaction validation (no negative/over-limit withdrawals)

✔ File Handling

Data stored in files for persistence

Secure read/write operations

Supports multiple user accounts

✔ Additional Advanced Features

Unique account number generation

Input validation for safer data entry

Transaction history logging (if included)

Modular functions for clean code

Menu-driven interface for ease of use

Error-free and optimized C code

📁 Project Structure
Bank_Management_System/
│
├── bank.c               # Main program code
├── accounts.dat         # File storing account records
├── transactions.log     # Optional: logs transactions
├── README.md            # Project documentation
└── compile_run.sh       # Script to compile & run (optional)

⚙️ How It Works

Program displays a menu

User selects an operation

For account-related operations, user enters account number

Program reads/writes details from accounts.dat

Transaction results are displayed with proper validation

🛠 Technologies Used
Technology	Purpose
C Programming	Core logic and operations
File Handling	Persistent storage of account records
Structs	Storing account information
Functions	Clean and modular design
🏁 How to Run
1. Compile the Program
gcc bank.c -o bank

2. Run the Program
./bank

🧪 Sample Functionalities
Creating an Account
Enter Name: Mahi
Enter Address: Hyderabad
Enter Initial Deposit: 5000
Account Created Successfully!
Account Number: 100234

Depositing Money
Enter Account Number: 100234
Enter Amount to Deposit: 2000
Deposit Successful! New Balance: 7000

Withdrawing Money
Enter Account Number: 100234
Enter Amount to Withdraw: 10000
Error: Insufficient Balance!

🌟 Why This Project Is Useful

✔ Demonstrates file handling
✔ Great for B.Tech/MCA mini-projects
✔ Teaches structs, functions, validation
✔ Can be easily extended into bigger systems
✔ Fully menu-driven and user-friendly


Authoor : Mahesh Babu Baswani
