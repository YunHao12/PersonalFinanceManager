#ifndef DATA_LAYER_H
#define DATA_LAYER_H
#include <mysql.h>
#include <stdbool.h>

#define DESC_MAX_LENGTH 50
#define MAX_TRANSACTIONS 100

struct PersonalFinance {
    int personalFinanceId;
    float income;
    float expense;
};
struct Transaction {
    int transactionId;
    int personalFinanceId;
    int year;
    int month;
    int day;
    char type[DESC_MAX_LENGTH];
    char category[DESC_MAX_LENGTH];
    float amount;
    bool status;
};
void initializeDataLayer();
void closeDataLayer();

int* addPersonalFinance(struct PersonalFinance* pf); 
int* addTransaction(struct Transaction* t);
int* deleteTransaction(int transactionId);
int* updateTransaction(struct Transaction* t);
struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id);
struct Transaction*[MAX_TRANSACTIONS] loadAllTransactions(int personal_finance_id);
int getMaxPersonalFinanceId();
int getMaxTransactionId();

#endif 