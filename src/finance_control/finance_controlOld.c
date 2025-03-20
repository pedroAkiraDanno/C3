
















#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <libpq-fe.h>

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void printMenu() {
    printf("1. Add Income\n");
    printf("2. Add Expense\n");
    printf("3. View Transactions\n");
    printf("4. View Categories\n");
    printf("5. View Accounts\n");
    printf("6. View Budgets\n");
    printf("7. View Goals\n");
    printf("8. Exit\n");
}

void addTransaction(PGconn *conn, int user_id, const char *description, float amount, const char *type, int account_id, int category_id) {
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO transactions (user_id, account_id, category_id, description, amount, type) VALUES (%d, %d, %d, '%s', %.2f, '%s')", user_id, account_id, category_id, description, amount, type);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Transaction failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Transaction added successfully!\n");
}

void viewTransactions(PGconn *conn, int user_id) {
    char query[512];
    snprintf(query, sizeof(query), "SELECT t.id, t.description, t.amount, t.type, c.name AS category, a.name AS account, t.date "
                                   "FROM transactions t "
                                   "LEFT JOIN categories c ON t.category_id = c.id "
                                   "LEFT JOIN accounts a ON t.account_id = a.id "
                                   "WHERE t.user_id = %d "
                                   "ORDER BY t.date DESC", user_id);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Description       | Amount  | Type    | Category      | Account          | Date\n");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %-17s | %-7s | %-7s | %-13s | %-15s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1),
               PQgetvalue(res, i, 2),
               PQgetvalue(res, i, 3),
               PQgetvalue(res, i, 4),
               PQgetvalue(res, i, 5),
               PQgetvalue(res, i, 6));
    }
    PQclear(res);
}

void viewCategories(PGconn *conn) {
    const char *query = "SELECT * FROM categories ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Category Name\n");
    printf("-----------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1));
    }
    PQclear(res);
}

void viewAccounts(PGconn *conn, int user_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM accounts WHERE user_id = %d ORDER BY id", user_id);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Account Name       | Balance\n");
    printf("---------------------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %-18s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 2),
               PQgetvalue(res, i, 3));
    }
    PQclear(res);
}

int main() {
    const char *conninfo = "dbname=finances user=postgres password=p0w2i8 hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    int user_id = 1; // Assume the user is logged in as user_id = 1
    int choice;
    char description[100];
    float amount;
    int account_id, category_id;

    while (1) {
        clearScreen();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewAccounts(conn, user_id);
                printf("Enter account ID: ");
                scanf("%d", &account_id);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                addTransaction(conn, user_id, description, amount, "income", account_id, category_id);
                break;
            case 2:
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewAccounts(conn, user_id);
                printf("Enter account ID: ");
                scanf("%d", &account_id);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                addTransaction(conn, user_id, description, amount, "expense", account_id, category_id);
                break;
            case 3:
                viewTransactions(conn, user_id);
                break;
            case 4:
                viewCategories(conn);
                break;
            case 5:
                viewAccounts(conn, user_id);
                break;
            case 6:
                // Implement viewBudgets function
                printf("View Budgets feature not implemented yet.\n");
                break;
            case 7:
                // Implement viewGoals function
                printf("View Goals feature not implemented yet.\n");
                break;
            case 8:
                PQfinish(conn);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Press Enter to continue...");
        getchar(); getchar(); // Wait for Enter key
    }

    PQfinish(conn);
    return 0;
}

















/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <libpq-fe.h>

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void printMenu() {
    printf("1. Add Income\n");
    printf("2. Add Expense\n");
    printf("3. View Transactions\n");
    printf("4. View Categories\n");
    printf("5. Exit\n");
}

void addTransaction(PGconn *conn, const char *description, float amount, const char *type, int category_id) {
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO transactions (description, amount, type, category_id) VALUES ('%s', %.2f, '%s', %d)", description, amount, type, category_id);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Transaction failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Transaction added successfully!\n");
}

void viewTransactions(PGconn *conn) {
    const char *query = "SELECT t.id, t.description, t.amount, t.type, c.name AS category, t.date "
                        "FROM transactions t "
                        "LEFT JOIN categories c ON t.category_id = c.id "
                        "ORDER BY t.date DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Description       | Amount  | Type    | Category      | Date\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %-17s | %-7s | %-7s | %-13s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1),
               PQgetvalue(res, i, 2),
               PQgetvalue(res, i, 3),
               PQgetvalue(res, i, 4),
               PQgetvalue(res, i, 5));
    }
    PQclear(res);
}

void viewCategories(PGconn *conn) {
    const char *query = "SELECT * FROM categories ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Category Name\n");
    printf("-----------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1));
    }
    PQclear(res);
}

int main() {
    const char *conninfo = "dbname=finances user=postgres password=p0w2i8 hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    int choice;
    char description[100];
    float amount;
    int category_id;

    while (1) {
        clearScreen();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                addTransaction(conn, description, amount, "income", category_id);
                break;
            case 2:
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                addTransaction(conn, description, amount, "expense", category_id);
                break;
            case 3:
                viewTransactions(conn);
                break;
            case 4:
                viewCategories(conn);
                break;
            case 5:
                PQfinish(conn);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Press Enter to continue...");
        getchar(); getchar(); // Wait for Enter key
    }

    PQfinish(conn);
    return 0;
}




*/












/*

DB: finances


CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    type VARCHAR(10) CHECK (type IN ('income', 'expense')) NOT NULL,
    date DATE DEFAULT CURRENT_DATE,
    category_id INT REFERENCES categories(id) ON DELETE SET NULL
);


INSERT INTO categories (name) VALUES
('Food'),
('Bills'),
('Health'),
('Home'),
('Transport'),
('Entertainment'),
('Others');













gcc -o finance_control finance_control.c -I "C:\Program Files\PostgreSQL\<version>\include" -L "C:\Program Files\PostgreSQL\<version>\lib" -lpq



gcc -o finance_control finance_control.c -I "C:\Program Files\PostgreSQL\16\include" -L "C:\Program Files\PostgreSQL\16\lib" -lpq




psql -d database -U user


psql -d shutdown_logs -U postgres
SELECT * FROM shutdown_events;











*/








