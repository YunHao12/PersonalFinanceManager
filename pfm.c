#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INCOME_TRANSACTION 100
#define EXPENSE_TRANSACTION 100
#define DESC_MAX_LENGTH 50

struct Transaction {
    int year;
    int month;
    int day;
    char type[DESC_MAX_LENGTH];
    char category[DESC_MAX_LENGTH];
    float amount;
};

struct PersonalFinance {
    float income;
    float expense;
    int incomeIndex;
    int expenseIndex;
    struct Transaction* transaction_Expense[EXPENSE_TRANSACTION];
    struct Transaction* transaction_Income[INCOME_TRANSACTION];
};

struct Transaction* createTransaction(int year, int month, int day,
                                      const char type[DESC_MAX_LENGTH], const char category[DESC_MAX_LENGTH], float amount) {
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    newTransaction->year = year;
    newTransaction->month = month;
    newTransaction->day = day;
    strcpy(newTransaction->type, type);
    strcpy(newTransaction->category,category);
    newTransaction->amount = amount;
    return newTransaction;
}

struct PersonalFinance* createPersonalFinance() {
    struct PersonalFinance* newPersonalFinance = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    newPersonalFinance->income = 0;
    newPersonalFinance->expense = 0;
    newPersonalFinance->incomeIndex = 0;
    newPersonalFinance->expenseIndex= 0;
    return newPersonalFinance;
}

//time complexity: O(1)
void addTransaction(struct PersonalFinance* pf, struct Transaction* t) {
    if (strncmp (t->type, "Income", DESC_MAX_LENGTH)==0) {
        pf->transaction_Income[pf->incomeIndex] = t;
        pf->income += t->amount;
        pf->incomeIndex++;
    }
    else {
        pf->transaction_Expense[pf->expenseIndex] = t;
        pf->expense += t->amount;
        pf->expenseIndex++;
    }
}

//helper method for sorting by date
int compareDates(struct Transaction* t1, struct Transaction* t2) {
    if (t1->year != t2->year) {
        return t1->year - t2->year;
    } else if (t1->month != t2->month) {
        return t1->month - t2->month;
    } else {
        return t1->day - t2->day;
    }
}

int isValidDate(int year, int month, int day) {
    // Define the number of days in each month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year in February
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (year < 1900 || year > 2023) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > daysInMonth[month]) return 0;

    return 1;
}

//helper method for quick sort
void swap(struct Transaction** a, struct Transaction** b) {
    struct Transaction* temp = *a;
    *a = *b;
    *b = temp;
}

//helper method for quick sort
int partitionI(struct PersonalFinance* pf, int low, int high) {

    struct Transaction* pivot = pf->transaction_Income[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (compareDates(pf->transaction_Income[j], pivot) < 0) {
            i++;
            swap(&pf->transaction_Income[i], &pf->transaction_Income[j]);
        }
    }
    swap(&pf->transaction_Income[i + 1], &pf->transaction_Income[high]);
    return (i + 1);

}
int partitionE(struct PersonalFinance* pf, int low, int high) {
    struct Transaction* pivot = pf->transaction_Expense[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (compareDates(pf->transaction_Expense[j], pivot) < 0) {
            i++;
            swap(&pf->transaction_Expense[i], &pf->transaction_Expense[j]);
        }
    }
    swap(&pf->transaction_Expense[i + 1], &pf->transaction_Expense[high]);
    return (i + 1);
}

//time complexity: O(nlogn)
struct PersonalFinance* sortTransactionsByDateI(struct PersonalFinance *pf, int lowI, int highI) {
    //sort Income transactions
    if (lowI < highI) {
        int pivot = partitionI(pf, lowI, highI - 1);
        sortTransactionsByDateI(pf, lowI, pivot - 1);
        sortTransactionsByDateI(pf, pivot + 1, highI);
    }
    return pf;
}

struct PersonalFinance* sortTransactionsByDateE(struct PersonalFinance *pf, int lowE, int highE) {
    //sort Expense transactions
    if (lowE < highE) {
        int pivot = partitionE(pf, lowE, highE - 1);
        sortTransactionsByDateE(pf, lowE, pivot - 1);
        sortTransactionsByDateE(pf, pivot + 1, highE);
    }
    return pf;
}

void viewTransactionsByDate(struct PersonalFinance *pf) {
    //sort by date
    struct PersonalFinance* pfDate = sortTransactionsByDateI(pf, 0, pf->incomeIndex);
    pfDate = sortTransactionsByDateE(pfDate,0,pf->expenseIndex);
    //print income transactions
    printf("Income Transactions:\n");
    for (int i = 0; i < pfDate->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pfDate->transaction_Income[i]->year, pfDate->transaction_Income[i]->month, pfDate->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pfDate->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pfDate->transaction_Income[i]->amount);
    }
    //print expense transactions
    printf("Expense Transactions:\n");
    for (int i = 0; i < pfDate->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pfDate->transaction_Expense[i]->year, pfDate->transaction_Expense[i]->month, pfDate->transaction_Expense[i]->day);
        printf("Type: Expense\n");
        printf("Category: %s\n", pfDate->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pfDate->transaction_Expense[i]->amount);
    }
}

int ApartitionI(struct PersonalFinance* pf, int low, int high) {

    float pivot = pf->transaction_Income[high]->amount;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (pf->transaction_Income[j]->amount < pivot) {
            i++;
            swap(&pf->transaction_Income[i], &pf->transaction_Income[j]);
        }
    }
    swap(&pf->transaction_Income[i + 1], &pf->transaction_Income[high]);
    return (i + 1);
}

int ApartitionE(struct PersonalFinance* pf, int low, int high) {
    float pivot = pf->transaction_Expense[high]->amount;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (pf->transaction_Expense[j]->amount< pivot) {
            i++;
            swap(&pf->transaction_Expense[i], &pf->transaction_Expense[j]);
        }
    }
    swap(&pf->transaction_Expense[i + 1], &pf->transaction_Expense[high]);
    return (i + 1);
}

//time complexity: O(nlogn)
struct PersonalFinance* sortTransactionsByAmountI(struct PersonalFinance *pf, int lowI, int highI) {
    //sort Income transactions
    if (lowI < highI) {
        int pivot = ApartitionI(pf, lowI, highI - 1);
        sortTransactionsByAmountI(pf, lowI, pivot - 1);
        sortTransactionsByAmountI(pf, pivot + 1, highI);
    }
    return pf;
}

struct PersonalFinance* sortTransactionsByAmountE(struct PersonalFinance *pf, int lowE, int highE) {
    //sort Expense transactions
    if (lowE < highE) {
        int pivot = ApartitionE(pf, lowE, highE - 1);
        sortTransactionsByAmountE(pf, lowE, pivot - 1);
        sortTransactionsByAmountE(pf, pivot + 1, highE);
    }
    return pf;
}

void viewTransactionsByAmount(struct PersonalFinance *pf) {
    //sort by amount
    struct PersonalFinance* pfAmount = sortTransactionsByAmountI(pf, 0, pf->incomeIndex);
    pfAmount = sortTransactionsByAmountE(pf, 0, pf->expenseIndex);
    printf("Income Transactions:\n");
    for (int i = 0; i < pfAmount->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pfAmount->transaction_Income[i]->year, pfAmount->transaction_Income[i]->month, pfAmount->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pfAmount->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Income[i]->amount);
    }
    printf("Expense Transactions:\n");
    for (int i = 0; i < pfAmount->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pfAmount->transaction_Expense[i]->year, pfAmount->transaction_Expense[i]->month, pfAmount->transaction_Expense[i]->day);
        printf("Type: Expense\n");
        printf("Category: %s\n", pfAmount->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Expense[i]->amount);
    }
}

void viewTransactionsIncome(struct PersonalFinance *pf) {
    printf("Income Transactions:\n");
    for (int i = 0; i < pf->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pf->transaction_Income[i]->year, pf->transaction_Income[i]->month, pf->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pf->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Income[i]->amount);
    }
}

void viewTransactionsExpense(struct PersonalFinance *pf) {
    printf("Expense Transactions:\n");
    for (int i = 0; i < pf->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pf->transaction_Expense[i]->year, pf->transaction_Expense[i]->month, pf->transaction_Expense[i]->day);
        printf("Type: Expense\n");
        printf("Category: %s\n", pf->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Expense[i]->amount);
    }
}

float viewBalance(struct PersonalFinance *pf) {
    return pf->income - pf->expense;
}

//time complexity: O(1)
void deleteExpense(struct PersonalFinance *pf, int index) {
    if (index >= 0 && index < pf->expenseIndex) {
        pf->expense -= pf->transaction_Expense[index]->amount;
        // Shift the remaining transactions to fill the gap
        for (int i = index; i < pf->expenseIndex - 1; i++) {
            pf->transaction_Expense[i] = pf->transaction_Expense[i + 1];
        }
        pf->expenseIndex--;
    } else {
        printf("Error: Expense ID %d not found.\n", index);
    }
}

//time complexity: O(1)
void deleteIncome(struct PersonalFinance *pf, int index) {
    if (index >= 0 && index < pf->incomeIndex) {
        pf->income -= pf->transaction_Income[index]->amount;
        // Shift the remaining transactions to fill the gap
        for (int i = index; i < pf->incomeIndex - 1; i++) {
            pf->transaction_Income[i] = pf->transaction_Income[i + 1];
        }
        pf->incomeIndex--;
    } else {
        printf("Error: Income ID %d not found.\n", index);
    }
}


void updateTransaction(struct PersonalFinance *pf, int index, int type, int year, int month, int day, char category[DESC_MAX_LENGTH], float amount) {
    struct Transaction *t;
    if (type == 1) { // Income type check
        if (index < 0 || index >= pf->incomeIndex) { //This checks if the provided index is valid. If it's out of the boundaries of the existing income transactions
            printf("Error: Invalid Income Index.\n");
            return;
        }
        t = pf->transaction_Income[index]; // t now points to the income transaction to be updated.
        pf->income -= t->amount; // old amount of that transaction is subtracted from the total income to adjust the sum.
    }
    else if (type == 2) { // Expense type check
        if (index < 0 || index >= pf->expenseIndex) {
            printf("Error: Invalid Expense Index.\n");
            return;
        }
        t = pf->transaction_Expense[index];
        pf->expense -= t->amount; // Deduct the old amount

    }
    else {
        printf("Error: Invalid type specified.\n"); // Handling Invalid Transaction Types:
        return;
    }

    //Updating the Transaction Details:
    t->year = year;
    t->month = month;
    t->day = day;
    strcpy(t->category, category);
    t->amount = amount;

    if (type == 1) {
        pf->income += amount; // Add the updated amount
    } else {
        pf->expense += amount; // Add the updated amount
    }
}

struct Transaction* inputTransactionDetails() {
    int year, month, day;
    char category[100];
    char type[100];
    float amount;

    time_t currentTime;
    struct tm* currentDate;
    time(&currentTime);
    currentDate = localtime(&currentTime);
    year = currentDate->tm_year + 1900;
    month = currentDate->tm_mon + 1;
    day = currentDate->tm_mday;

    printf("Enter the transaction type:\n"); //Income or Expense
    scanf("%99s", type);
    printf("Enter the transaction category:\n");
    scanf("%99s", category);
    printf("Enter the amount of the transaction:\n");
    scanf("%f", &amount);

    return createTransaction(year, month, day, type, category, amount);
}

void freePersonalFinance(struct PersonalFinance* pf) {
    for (int i = 0; i < pf->incomeIndex; i++) {
        free(pf->transaction_Income[i]);
    }
    for (int i = 0; i < pf->expenseIndex; i++) {
        free(pf->transaction_Expense[i]);
    }
    free(pf);
}

int main() {
    struct PersonalFinance *pf = createPersonalFinance();
    int choice;

    while (1) {
        printf("\nPersonal Finance Management System\n");
        printf("1. Add Transactions\n");
        printf("2. View Transactions by Date\n");
        printf("3. View Transactions by Amount\n");
        printf("4. View All Income Transactions\n");
        printf("5. View All Expense Transactions\n");
        printf("6. View Balance\n");
        printf("7. Update Transaction\n");
        printf("8. Delete Income Transaction\n");
        printf("9. Delete Expense Transaction\n");
        printf("10. Exit\n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                struct Transaction* t = inputTransactionDetails();
                addTransaction(pf, t);
                break;
            }
            case 2: {
                viewTransactionsByDate(pf);
                break;
            }
            case 3: {
                viewTransactionsByAmount(pf);
                break;
            }
            case 4: {
                viewTransactionsIncome(pf);
                break;
            }
            case 5: {
                viewTransactionsExpense(pf);
                break;
            }
            case 6: {
                float balance = viewBalance(pf);
                printf("Balance: $%.2f\n", balance);
                break;
            }
            case 7: {
                int type, index, year, month, day;
                char category[DESC_MAX_LENGTH];
                float amount;

                printf("Update Transaction\n");
                printf("------------------------------\n");
                printf("Enter the type of transaction:\n");
                printf("1. Income\n");
                printf("2. Expense\n");
                scanf("%d", &type);

                if (type == 1) {
                    printf("Enter the ID of the income transaction to update: ");
                } else if (type == 2) {
                    printf("Enter the ID of the expense transaction to update: ");
                } else {
                    printf("Error: Invalid type entered.\n");
                    break;
                }
                scanf("%d", &index);

                do {
                    printf("Enter the updated year of the transaction (e.g. 2023):\n");
                    scanf("%d", &year);
                    printf("Enter the updated month of the transaction (1-12):\n");
                    scanf("%d", &month);
                    printf("Enter the updated day of the transaction (1-31):\n");
                    scanf("%d", &day);

                    if(!isValidDate(year, month, day)) {
                        printf("Error: Invalid date entered. Please try again.\n");
                    }
                } while(!isValidDate(year, month, day));

                printf("Enter the updated category of the transaction (e.g., Groceries, Rent):\n");
                scanf("%s", category); // category is already an address, no need for &

                printf("Enter the updated amount of the transaction (e.g., 150.50):\n");
                scanf("%f", &amount);

                updateTransaction(pf, index - 1, type, year, month, day, category, amount);

                printf("Transaction updated successfully.\n");
                break;
            }
            case 8: {
                int incomeIndex;
                printf("Enter the ID of the income to delete: ");
                scanf("%d", &incomeIndex);
                deleteIncome(pf, incomeIndex - 1);
                break;
            }
            case 9: {
                int expenseIndex;
                printf("Enter the ID of the expense to delete: ");
                scanf("%d", &expenseIndex);
                deleteExpense(pf, expenseIndex - 1);
                break;
            }
            case 10: {
                freePersonalFinance(pf);
                printf("Exiting the program.\n");
                return 0;
            }
            default: {
                printf("Invalid choice. Please select a valid option.\n");
            }
        }
    }
    freePersonalFinance(pf);
    return 0;
}