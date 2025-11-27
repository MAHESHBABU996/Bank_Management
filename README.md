# Bank_Management
📚 Library Management 

📝 Project Overview

The Library Management System (LMS) is a console-based C application developed to manage the basic operations of a library.
It allows the librarian to maintain book records, issue/return books, search books, delete books, and store data permanently using file handling.

This project is ideal for:

Mini projects

Academic submissions

GitHub portfolio

Learning file handling & structured programming

🚀 Features
✔ 1. Add New Book

Add complete details:

Book ID

Title

Author

Quantity

Stored permanently using binary file handling (library.dat).

✔ 2. Display All Books

Shows a complete list of books in a structured table format:

ID

Title

Author

Quantity

Issued copies

✔ 3. Search Book

Search by Book ID and get full details instantly.

✔ 4. Issue Book

Issues a book only if available

Updates the total issued count

Prevents issuing if all copies are already issued

✔ 5. Return Book

Accepts returned books

Decreases issued count

Prevents invalid returns

✔ 6. Delete Book

Removes a book permanently from the library database using a safe temporary-file method.

✔ 7. Permanent Storage

All data is stored in:

library.dat


Records remain saved even after the program closes.

🛠 Technologies Used

C programming

Structures

File Handling (Binary Files)

Modular Programming

📂 Project Structure
/LibraryManagementSystem
   ├── library.c          # main program file
   ├── library.dat        # auto-created database file
   ├── README.md          # documentation

🧩 Data Structure Used

Each book is stored as a structure:

Field	Description
id	Unique numeric ID
title	Name of the book
author	Author of the book
quantity	Total copies
issued	Number of copies that are currently issued
▶️ How to Run
Windows / Linux / MacOS
gcc library.c -o library
./library

Or simply run in CodeBlocks / Dev C / VS Code.
📘 Sample Output
========================================
        LIBRARY MANAGEMENT SYSTEM        
========================================
1. Add Book
2. Display All Books
3. Search Book
4. Issue Book
5. Return Book
6. Delete Book
7. Exit
Enter your choice:

⭐ Why This Project is Useful

Demonstrates use of file handling

Realistic library operations

Simple and structured

Good for college submissions

Easy to extend and upgrade


Authoor : Mahesh Babu Baswani
