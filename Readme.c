Personal Expense Tracker in C
A console-based personal expense management project built using C. It helps users record income and expenses, review spending by day or month, and manage monthly budgets. The project uses one C source file and stores data in plain text files.
Features
1. Add income
2. Add expense
3. View all transactions
4. Daily report
5. Monthly report
6. Category-wise expense analysis
7. Search transactions
8. Edit transaction
9. Delete transaction
10. Set monthly budget
11. Check budget status
12. Find highest expense
13. Savings summary
14. Day-wise monthly breakdown
Technologies and concepts
- C programming language (C11)
- Structures and arrays
- Functions with arguments and pointers
- Loops, conditions, and switch-case
- Strings and input validation
- Text file handling (fopen, fgets, fprintf, sscanf, fclose)
Project files
Personal_Expense_Tracker/
├── main.c              # Entire program
├── README.md           # Project information
├── CODE_EXPLANATION.md # Study notes and viva preparation
├── transactions.txt    # Created when a transaction is saved
└── budgets.txt         # Created when a budget is saved
Only main.c is needed to compile the program. The two .txt files are created automatically when data is saved, in the directory where the program is run. No CSV files or database are used.
Run in VS Code on Windows
1. Install GCC (for example, through MinGW-w64) and the C/C++ extension for VS Code.
2. Open the project folder in VS Code.
3. Open Terminal > New Terminal.
4. Compile:
gcc -std=c11 -Wall -Wextra main.c -o tracker.exe
5. Run:
.\tracker.exe
If GCC is not recognized, install/configure GCC and add it to your PATH.

On Linux or macOS, compile with gcc -std=c11 -Wall -Wextra main.c -o tracker and run ./tracker.
Example workflow
1. Choose 1 to add income, enter date, amount, category, and note.
2. Choose 2 to add an expense.
3. Choose 3 to view all transactions.
4. Choose 4 to check a particular day's income and expenses.
5. Choose 10 to set a monthly budget and 11 to check its status.
6. Choose 0 to exit. Saved data is available the next time you run the program from the same folder.
How the data is stored
Transactions are stored in transactions.txt using the format:
id|type|day|month|year|amount|category|note
type = 1 means income and type = 2 means expense.

Budgets are stored in budgets.txt using:
month|year|limit
The program loads saved records at startup and writes the updated records when a transaction or budget changes. Avoid manually editing the text files while the program is running.
Function design
Transaction data is held in an array in main() and passed to functions as arguments. Functions that change the number of records receive int *count; display and reporting functions receive int count. displayMenu(void) takes no parameters because it only prints fixed menu text.

Example:
addTransaction(transactions, &count, 2);
viewTransactions(transactions, count);
Limitations
- Up to 1,000 transactions and 120 budget entries.
- Console interface; no graphical user interface.
- Data is stored locally in plain text, without encryption.
- This is a learning project, not a production banking or accounting application.
Author
Developed as a C programming learning project for a first-year B.Tech course.
