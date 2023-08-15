#include "data_layer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>


MYSQL *conn;

void initializeDataLayer() {
    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "username", "password", "database_name", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(1);
    }
}

void closeDataLayer() {
    mysql_close(conn);
}

int* addPersonalFinance(struct PersonalFinance* pf) {
    // Implement SQL INSERT query for personal_finance table
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO personal_finance (personalFinanceId, income, expense) VALUES (%d, %f, %f)",
             pf->personalFinanceId, pf->income, pf->expense);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT error: %s\n", mysql_error(conn));
        return NULL;
    }

    return NULL;
}

int* addTransaction(int personal_finance_id, struct Transaction* t) {
    // Implement SQL INSERT query for transactions table
    char query[256];
    snprintf(query, sizeof(query),
             "INSERT INTO transactions (transactionId, personalFinanceId, year, month, day, type, category, amount, status) VALUES (%d, %d, %d, %d, %d, '%s', '%s', %f, %d)",
             t->transactionId, personal_finance_id, t->year, t->month, t->day, t->type, t->category, t->amount, t->status);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT error: %s\n", mysql_error(conn));
        return NULL;
    }

    return NULL;
}

int* deleteTransaction(int transaction_id) {
    // Implement SQL DELETE query for transactions table
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM transactions WHERE transactionId = %d", transaction_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "DELETE error: %s\n", mysql_error(conn));
        return NULL;
    }

    return NULL;
}

int* updateTransaction(struct Transaction* t) {
    // Implement SQL UPDATE query for transactions table
    char query[256];
    snprintf(query, sizeof(query),
             "UPDATE transactions SET year = %d, month = %d, day = %d, type = '%s', category = '%s', amount = %f, status = %d WHERE transactionId = %d",
             t->year, t->month, t->day, t->type, t->category, t->amount, t->status, t->transactionId);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE error: %s\n", mysql_error(conn));
        return NULL;
    }

    return NULL;
}

struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id) {
    // Implement SQL SELECT query for personal_finance table
    char query[128];
    snprintf(query, sizeof(query), "SELECT * FROM personal_finance WHERE personalFinanceId = %d", personal_finance_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT error: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "mysql_store_result() failed\n");
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        fprintf(stderr, "No data found\n");
        mysql_free_result(result);
        return NULL;
    }

    struct PersonalFinance* pf = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    pf->personalFinanceId = atoi(row[0]);
    pf->income = atof(row[1]);
    pf->expense = atof(row[2]);

    mysql_free_result(result);
    return pf;
}

struct Transaction* loadAllTransactions(int personal_finance_id) {
    // Implement SQL SELECT query for transactions table
    char query[128];
    snprintf(query, sizeof(query), "SELECT * FROM transactions WHERE personalFinanceId = %d", personal_finance_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT error: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "mysql_store_result() failed\n");
        return NULL;
    }

    int num_transactions = mysql_num_rows(result);
    if (num_transactions == 0) {
        fprintf(stderr, "No transactions found\n");
        mysql_free_result(result);
        return NULL;
    }

    struct Transaction* transactions = (struct Transaction*)malloc(num_transactions * sizeof(struct Transaction));
    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        transactions[i].transactionId = atoi(row[0]);
        transactions[i].year = atoi(row[2]);
        transactions[i].month = atoi(row[3]);
        transactions[i].day = atoi(row[4]);
        strcpy(transactions[i].type, row[5]);
        strcpy(transactions[i].category, row[6]);
        transactions[i].amount = atof(row[7]);
        transactions[i].status = atoi(row[8]);
        i++;
    }

    mysql_free_result(result);
    return transactions;
}

int getMaxTransactionId() {
    const char *query = "SELECT MAX(transactionId) FROM transactions";
    int maxTransactionId = 0;

    if (mysql_query(conn, query) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row && row[0]) {
                maxTransactionId = atoi(row[0]);
            }
            mysql_free_result(result);
        } else {
            fprintf(stderr, "mysql_store_result() failed\n");
        }
    } else {
        fprintf(stderr, "Query error: %s\n", mysql_error(conn));
    }

    return maxTransactionId;
}
int getMaxPersonalFinanceId() {
    const char *query = "SELECT MAX(personalFinanceId) FROM personal_finance";
    int maxPersonalFinanceId = 0;

    if (mysql_query(conn, query) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row && row[0]) {
                maxPersonalFinanceId = atoi(row[0]);
            }
            mysql_free_result(result);
        } else {
            fprintf(stderr, "mysql_store_result() failed\n");
        }
    } else {
        fprintf(stderr, "Query error: %s\n", mysql_error(conn));
    }

    return maxPersonalFinanceId;
}