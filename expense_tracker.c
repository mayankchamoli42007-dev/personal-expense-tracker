#include <stdio.h>
#include <string.h>

#define MAX 1000
#define MAX_BUDGETS 120
#define TRANSACTION_FILE "transactions.txt"
#define BUDGET_FILE "budgets.txt"

typedef struct
{
    int id;
    int type; /* 1 = income, 2 = expense */
    int day;
    int month;
    int year;
    double amount;
    char category[30];
    char note[80];
} Transaction;

typedef struct
{
    int month;
    int year;
    double limit;
} Budget;

int readInt(const char message[], int minimum, int maximum);
double readAmount(const char message[], int allowZero);
void readText(const char message[], char text[], int size);
int validDate(int day, int month, int year);
void readDate(int *day, int *month, int *year);
int findTransaction(const Transaction transactions[], int count, int id);
int findBudget(const Budget budgets[], int count, int month, int year);
void loadTransactions(Transaction transactions[], int *count);
int saveTransactions(const Transaction transactions[], int count);
void loadBudgets(Budget budgets[], int *count);
int saveBudgets(const Budget budgets[], int count);
void printTransaction(const Transaction *transaction);
void addTransaction(Transaction transactions[], int *count, int type);
void viewTransactions(const Transaction transactions[], int count);
void dailyReport(const Transaction transactions[], int count);
void monthlyReport(const Transaction transactions[], int count);
void categoryReport(const Transaction transactions[], int count);
void searchTransactions(const Transaction transactions[], int count);
void editTransaction(Transaction transactions[], int count);
void deleteTransaction(Transaction transactions[], int *count);
void setBudget(Budget budgets[], int *count);
void budgetStatus(const Transaction transactions[], int count, const Budget budgets[], int budgetCount);
void highestExpense(const Transaction transactions[], int count);
void savingsSummary(const Transaction transactions[], int count);
void dayWiseBreakdown(const Transaction transactions[], int count);
void displayMenu(void);

int main(void)
{
    Transaction transactions[MAX];
    Budget budgets[MAX_BUDGETS];
    int count = 0;
    int budgetCount = 0;
    int choice;

    loadTransactions(transactions, &count);
    loadBudgets(budgets, &budgetCount);

    do
    {
        displayMenu();
        choice = readInt("Choose an option: ", 0, 14);

        switch (choice)
        {
            case 1:
                addTransaction(transactions, &count, 1);
                break;
            case 2:
                addTransaction(transactions, &count, 2);
                break;
            case 3:
                viewTransactions(transactions, count);
                break;
            case 4:
                dailyReport(transactions, count);
                break;
            case 5:
                monthlyReport(transactions, count);
                break;
            case 6:
                categoryReport(transactions, count);
                break;
            case 7:
                searchTransactions(transactions, count);
                break;
            case 8:
                editTransaction(transactions, count);
                break;
            case 9:
                deleteTransaction(transactions, &count);
                break;
            case 10:
                setBudget(budgets, &budgetCount);
                break;
            case 11:
                budgetStatus(transactions, count, budgets, budgetCount);
                break;
            case 12:
                highestExpense(transactions, count);
                break;
            case 13:
                savingsSummary(transactions, count);
                break;
            case 14:
                dayWiseBreakdown(transactions, count);
                break;
            case 0:
                printf("Thank you for using Expense Tracker!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}

int readInt(const char message[], int minimum, int maximum)
{
    char line[100];
    int value;
    char extra;

    while (1)
    {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("Input closed.\n");
            return minimum;
        }

        if (sscanf(line, " %d %c", &value, &extra) == 1 &&
            value >= minimum && value <= maximum)
        {
            return value;
        }

        printf("Enter a number between %d and %d.\n", minimum, maximum);
    }
}

double readAmount(const char message[], int allowZero)
{
    char line[100];
    double amount;
    char extra;

    while (1)
    {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return 0;
        }

        if (sscanf(line, " %lf %c", &amount, &extra) == 1 &&
            amount <= 1000000000.0 &&
            (amount > 0 || (allowZero && amount == 0)))
        {
            return amount;
        }

        printf("Enter a valid %samount (maximum 1000000000).\n",
               allowZero ? "non-negative " : "positive ");
    }
}

void readText(const char message[], char text[], int size)
{
    char line[256];
    int i;

    while (1)
    {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            text[0] = '\0';
            return;
        }

        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0' || (int)strlen(line) >= size ||
            strchr(line, '|') != NULL)
        {
            printf("Enter 1 to %d characters without '|'.\n", size - 1);
            continue;
        }

        for (i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == '\r')
            {
                line[i] = ' ';
            }
        }

        strcpy(text, line);
        return;
    }
}

int validDate(int day, int month, int year)
{
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (year < 2000 || year > 2100 || month < 1 || month > 12)
    {
        return 0;
    }

    if (month == 2 &&
        (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
    {
        days[2] = 29;
    }

    return day >= 1 && day <= days[month];
}

void readDate(int *day, int *month, int *year)
{
    while (1)
    {
        *day = readInt("Day: ", 1, 31);
        *month = readInt("Month: ", 1, 12);
        *year = readInt("Year (2000-2100): ", 2000, 2100);

        if (validDate(*day, *month, *year))
        {
            return;
        }

        printf("Invalid calendar date. Try again.\n");
    }
}

int findTransaction(const Transaction transactions[], int count, int id)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

int findBudget(const Budget budgets[], int count, int month, int year)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (budgets[i].month == month && budgets[i].year == year)
        {
            return i;
        }
    }

    return -1;
}

void loadTransactions(Transaction transactions[], int *count)
{
    FILE *file = fopen(TRANSACTION_FILE, "r");
    char line[300];
    Transaction item;
    int fields;

    *count = 0;
    if (file == NULL)
    {
        return;
    }

    while (*count < MAX && fgets(line, sizeof(line), file) != NULL)
    {
        fields = sscanf(line, "%d|%d|%d|%d|%d|%lf|%29[^|]|%79[^\n]",
                        &item.id, &item.type, &item.day, &item.month,
                        &item.year, &item.amount, item.category, item.note);

        if (fields == 8 && item.id > 0 &&
            (item.type == 1 || item.type == 2) &&
            validDate(item.day, item.month, item.year) &&
            item.amount > 0 && item.amount <= 1000000000.0 &&
            findTransaction(transactions, *count, item.id) == -1)
        {
            transactions[*count] = item;
            (*count)++;
        }
    }

    fclose(file);
}

int saveTransactions(const Transaction transactions[], int count)
{
    FILE *file = fopen(TRANSACTION_FILE, "w");
    int i;

    if (file == NULL)
    {
        printf("Error: Could not save transactions.\n");
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        if (fprintf(file, "%d|%d|%d|%d|%d|%.2f|%s|%s\n",
                    transactions[i].id, transactions[i].type,
                    transactions[i].day, transactions[i].month,
                    transactions[i].year, transactions[i].amount,
                    transactions[i].category, transactions[i].note) < 0)
        {
            fclose(file);
            printf("Error writing transactions.\n");
            return 0;
        }
    }

    if (fclose(file) != 0)
    {
        printf("Error closing transactions file.\n");
        return 0;
    }

    return 1;
}

void loadBudgets(Budget budgets[], int *count)
{
    FILE *file = fopen(BUDGET_FILE, "r");
    Budget item;
    char line[100];

    *count = 0;
    if (file == NULL)
    {
        return;
    }

    while (*count < MAX_BUDGETS && fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d|%d|%lf", &item.month, &item.year,
                   &item.limit) == 3 && item.month >= 1 && item.month <= 12 &&
            item.year >= 2000 && item.year <= 2100 &&
            item.limit > 0 && item.limit <= 1000000000.0 &&
            findBudget(budgets, *count, item.month, item.year) == -1)
        {
            budgets[*count] = item;
            (*count)++;
        }
    }

    fclose(file);
}

int saveBudgets(const Budget budgets[], int count)
{
    FILE *file = fopen(BUDGET_FILE, "w");
    int i;

    if (file == NULL)
    {
        printf("Error: Could not save budgets.\n");
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        if (fprintf(file, "%d|%d|%.2f\n", budgets[i].month,
                    budgets[i].year, budgets[i].limit) < 0)
        {
            fclose(file);
            return 0;
        }
    }

    return fclose(file) == 0;
}

void printTransaction(const Transaction *transaction)
{
    printf("%-5d %02d/%02d/%04d %-8s %-16.16s %10.2f  %s\n",
           transaction->id, transaction->day, transaction->month,
           transaction->year, transaction->type == 1 ? "Income" : "Expense",
           transaction->category, transaction->amount, transaction->note);
}

void addTransaction(Transaction transactions[], int *count, int type)
{
    Transaction item;
    int i;
    int highestId = 0;

    if (*count >= MAX)
    {
        printf("Storage is full.\n");
        return;
    }

    for (i = 0; i < *count; i++)
    {
        if (transactions[i].id > highestId)
        {
            highestId = transactions[i].id;
        }
    }

    item.id = highestId + 1;
    item.type = type;

    printf("\nAdding %s (ID: %d)\n",
           type == 1 ? "income" : "expense", item.id);
    readDate(&item.day, &item.month, &item.year);
    item.amount = readAmount("Amount (Rs.): ", 0);
    readText("Category: ", item.category, sizeof(item.category));
    readText("Note: ", item.note, sizeof(item.note));

    transactions[*count] = item;
    (*count)++;

    if (saveTransactions(transactions, *count))
    {
        printf("Transaction added and saved.\n");
    }
}

void viewTransactions(const Transaction transactions[], int count)
{
    int i;

    if (count == 0)
    {
        printf("No transactions found.\n");
        return;
    }

    printf("\nID    Date       Type     Category             Amount  Note\n");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        printTransaction(&transactions[i]);
    }
}

void dailyReport(const Transaction transactions[], int count)
{
    int day, month, year, i, found = 0;
    double income = 0, expense = 0;

    printf("\nDAILY REPORT\n");
    readDate(&day, &month, &year);

    for (i = 0; i < count; i++)
    {
        if (transactions[i].day == day && transactions[i].month == month &&
            transactions[i].year == year)
        {
            printTransaction(&transactions[i]);
            found = 1;

            if (transactions[i].type == 1)
            {
                income += transactions[i].amount;
            }
            else
            {
                expense += transactions[i].amount;
            }
        }
    }

    if (!found)
    {
        printf("No transactions on this date.\n");
    }

    printf("Income: Rs. %.2f | Expenses: Rs. %.2f | Net: Rs. %.2f\n",
           income, expense, income - expense);
}

void monthlyReport(const Transaction transactions[], int count)
{
    int month = readInt("Month: ", 1, 12);
    int year = readInt("Year: ", 2000, 2100);
    int i;
    double income = 0, expense = 0;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].month == month && transactions[i].year == year)
        {
            if (transactions[i].type == 1)
            {
                income += transactions[i].amount;
            }
            else
            {
                expense += transactions[i].amount;
            }
        }
    }

    printf("\nMONTHLY REPORT %02d/%04d\n", month, year);
    printf("Income: Rs. %.2f\n", income);
    printf("Expenses: Rs. %.2f\n", expense);
    printf("Net savings: Rs. %.2f\n", income - expense);
}

void categoryReport(const Transaction transactions[], int count)
{
    int month = readInt("Month: ", 1, 12);
    int year = readInt("Year: ", 2000, 2100);
    int i, j, alreadyShown;
    double total = 0, categoryTotal;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].type == 2 && transactions[i].month == month &&
            transactions[i].year == year)
        {
            total += transactions[i].amount;
        }
    }

    if (total == 0)
    {
        printf("No expenses for this month.\n");
        return;
    }

    printf("\nCATEGORY ANALYSIS %02d/%04d\n", month, year);

    for (i = 0; i < count; i++)
    {
        if (transactions[i].type != 2 || transactions[i].month != month ||
            transactions[i].year != year)
        {
            continue;
        }

        alreadyShown = 0;
        for (j = 0; j < i; j++)
        {
            if (transactions[j].type == 2 && transactions[j].month == month &&
                transactions[j].year == year &&
                strcmp(transactions[j].category, transactions[i].category) == 0)
            {
                alreadyShown = 1;
                break;
            }
        }

        if (alreadyShown)
        {
            continue;
        }

        categoryTotal = 0;
        for (j = i; j < count; j++)
        {
            if (transactions[j].type == 2 && transactions[j].month == month &&
                transactions[j].year == year &&
                strcmp(transactions[j].category, transactions[i].category) == 0)
            {
                categoryTotal += transactions[j].amount;
            }
        }

        printf("%-20s Rs. %10.2f  %6.2f%%\n", transactions[i].category,
               categoryTotal, categoryTotal * 100 / total);
    }

    printf("Total expenses: Rs. %.2f\n", total);
}

void searchTransactions(const Transaction transactions[], int count)
{
    int choice = readInt("Search by 1=ID, 2=Category, 3=Date: ", 1, 3);
    int id = 0, day = 0, month = 0, year = 0;
    int i, found = 0;
    char category[30] = "";

    if (choice == 1)
    {
        id = readInt("Transaction ID: ", 1, 2000000000);
    }
    else if (choice == 2)
    {
        readText("Category: ", category, sizeof(category));
    }
    else
    {
        readDate(&day, &month, &year);
    }

    for (i = 0; i < count; i++)
    {
        int matches = 0;

        if (choice == 1 && transactions[i].id == id)
        {
            matches = 1;
        }
        else if (choice == 2 &&
                 strcmp(transactions[i].category, category) == 0)
        {
            matches = 1;
        }
        else if (choice == 3 && transactions[i].day == day &&
                 transactions[i].month == month && transactions[i].year == year)
        {
            matches = 1;
        }

        if (matches)
        {
            printTransaction(&transactions[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching transactions.\n");
    }
}

void editTransaction(Transaction transactions[], int count)
{
    int id = readInt("ID to edit: ", 1, 2000000000);
    int index = findTransaction(transactions, count, id);
    Transaction updated;

    if (index == -1)
    {
        printf("Transaction not found.\n");
        return;
    }

    updated = transactions[index];
    printf("Enter replacement details for ID %d:\n", id);
    updated.type = readInt("Type (1=Income, 2=Expense): ", 1, 2);
    readDate(&updated.day, &updated.month, &updated.year);
    updated.amount = readAmount("Amount (Rs.): ", 0);
    readText("Category: ", updated.category, sizeof(updated.category));
    readText("Note: ", updated.note, sizeof(updated.note));

    transactions[index] = updated;
    if (saveTransactions(transactions, count))
    {
        printf("Transaction updated.\n");
    }
}

void deleteTransaction(Transaction transactions[], int *count)
{
    int id = readInt("ID to delete: ", 1, 2000000000);
    int index = findTransaction(transactions, *count, id);
    int i, confirm;

    if (index == -1)
    {
        printf("Transaction not found.\n");
        return;
    }

    printTransaction(&transactions[index]);
    confirm = readInt("Confirm delete? (1=Yes, 0=No): ", 0, 1);

    if (confirm == 0)
    {
        printf("Deletion cancelled.\n");
        return;
    }

    for (i = index; i < *count - 1; i++)
    {
        transactions[i] = transactions[i + 1];
    }

    (*count)--;
    if (saveTransactions(transactions, *count))
    {
        printf("Transaction deleted.\n");
    }
}

void setBudget(Budget budgets[], int *count)
{
    int month = readInt("Budget month: ", 1, 12);
    int year = readInt("Budget year: ", 2000, 2100);
    int index = findBudget(budgets, *count, month, year);
    double limit;

    if (index == -1 && *count >= MAX_BUDGETS)
    {
        printf("Budget storage is full.\n");
        return;
    }

    limit = readAmount("Monthly expense limit (Rs.): ", 0);

    if (index == -1)
    {
        index = *count;
        (*count)++;
    }

    budgets[index].month = month;
    budgets[index].year = year;
    budgets[index].limit = limit;

    if (saveBudgets(budgets, *count))
    {
        printf("Monthly budget saved.\n");
    }
}

void budgetStatus(const Transaction transactions[], int count,
                  const Budget budgets[], int budgetCount)
{
    int month = readInt("Month: ", 1, 12);
    int year = readInt("Year: ", 2000, 2100);
    int index = findBudget(budgets, budgetCount, month, year);
    int i;
    double spent = 0, percentage;

    if (index == -1)
    {
        printf("No budget set for this month.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (transactions[i].type == 2 && transactions[i].month == month &&
            transactions[i].year == year)
        {
            spent += transactions[i].amount;
        }
    }

    percentage = spent * 100 / budgets[index].limit;
    printf("Budget: Rs. %.2f\n", budgets[index].limit);
    printf("Spent: Rs. %.2f (%.1f%%)\n", spent, percentage);
    printf("Remaining: Rs. %.2f\n", budgets[index].limit - spent);

    if (percentage >= 100)
    {
        printf("ALERT: Monthly budget reached or exceeded!\n");
    }
    else if (percentage >= 80)
    {
        printf("WARNING: You have used at least 80%% of your budget.\n");
    }
    else
    {
        printf("Spending is within your budget.\n");
    }
}

void highestExpense(const Transaction transactions[], int count)
{
    int i, index = -1;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].type == 2 &&
            (index == -1 || transactions[i].amount > transactions[index].amount))
        {
            index = i;
        }
    }

    if (index == -1)
    {
        printf("No expenses recorded.\n");
    }
    else
    {
        printf("Highest expense:\n");
        printTransaction(&transactions[index]);
    }
}

void savingsSummary(const Transaction transactions[], int count)
{
    double income = 0, expenses = 0;
    int i;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].type == 1)
        {
            income += transactions[i].amount;
        }
        else
        {
            expenses += transactions[i].amount;
        }
    }

    printf("\nOVERALL FINANCIAL SUMMARY\n");
    printf("Total income: Rs. %.2f\n", income);
    printf("Total expenses: Rs. %.2f\n", expenses);
    printf("Net balance: Rs. %.2f\n", income - expenses);
}

void dayWiseBreakdown(const Transaction transactions[], int count)
{
    int month = readInt("Month: ", 1, 12);
    int year = readInt("Year: ", 2000, 2100);
    double dailyIncome[32] = {0};
    double dailyExpenses[32] = {0};
    int i, day, found = 0;

    for (i = 0; i < count; i++)
    {
        if (transactions[i].month == month && transactions[i].year == year)
        {
            day = transactions[i].day;
            if (transactions[i].type == 1)
            {
                dailyIncome[day] += transactions[i].amount;
            }
            else
            {
                dailyExpenses[day] += transactions[i].amount;
            }
        }
    }

    printf("\nDAY-WISE BREAKDOWN %02d/%04d\n", month, year);
    printf("Day      Income       Expenses       Net\n");

    for (day = 1; day <= 31; day++)
    {
        if (dailyIncome[day] != 0 || dailyExpenses[day] != 0)
        {
            printf("%02d   %10.2f   %10.2f   %10.2f\n", day,
                   dailyIncome[day], dailyExpenses[day],
                   dailyIncome[day] - dailyExpenses[day]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No transactions for this month.\n");
    }
}

void displayMenu(void)
{
    printf("\n================================================\n");
    printf("          PERSONAL EXPENSE TRACKER\n");
    printf("================================================\n");
    printf(" 1. Add income\n");
    printf(" 2. Add expense\n");
    printf(" 3. View all transactions\n");
    printf(" 4. Daily report\n");
    printf(" 5. Monthly report\n");
    printf(" 6. Category-wise analysis\n");
    printf(" 7. Search transactions\n");
    printf(" 8. Edit transaction\n");
    printf(" 9. Delete transaction\n");
    printf("10. Set monthly budget\n");
    printf("11. Check budget status\n");
    printf("12. Highest expense\n");
    printf("13. Savings summary\n");
    printf("14. Day-wise monthly breakdown\n");
    printf(" 0. Exit\n");
    printf("================================================\n");
}
