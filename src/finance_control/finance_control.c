

















#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    printf("1. Add Income (e.g., Salary)\n");
    printf("2. Add Expense\n");
    printf("3. View Transactions\n");
    printf("4. View Income\n");
    printf("5. View Transaction Categories\n");
    printf("6. View Income Categories\n");
    printf("7. View Payment Methods\n");
    printf("8. Exit\n");
}

// Function to get the current date in YYYY-MM-DD format
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Function to detect the operating system
void detectOS() {
    #ifdef _WIN32
    printf("Operating System: Windows\n");
    #else
    printf("Operating System: Linux\n");
    #endif
}

void addTransaction(PGconn *conn, const char *description, float amount, const char *type, int category_id, int payment_method_id, const char *company_name, const char *company_location, const char *date_record) {
    char query[512];
    if (company_name[0] == '\0') {
        // If company name is empty, insert NULL for company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, date_record) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, '%s')", 
                 description, amount, type, category_id, payment_method_id, date_record);
    } else {
        // Insert company if it doesn't exist
        char company_query[256];
        snprintf(company_query, sizeof(company_query), 
                 "INSERT INTO companies (name, location) VALUES ('%s', '%s') ON CONFLICT (name) DO NOTHING", 
                 company_name, company_location);
        PGresult *company_res = PQexec(conn, company_query);
        if (PQresultStatus(company_res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Company insertion failed: %s", PQerrorMessage(conn));
            PQclear(company_res);
            return;
        }
        PQclear(company_res);

        // Insert transaction with company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, company_id, date_record) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, (SELECT id FROM companies WHERE name = '%s'), '%s')", 
                 description, amount, type, category_id, payment_method_id, company_name, date_record);
    }

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Transaction failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Transaction added successfully!\n");
}

void addIncome(PGconn *conn, const char *description, float amount, int category_income_id, int payment_method_id, const char *date_record) {
    char query[512];
    snprintf(query, sizeof(query), 
             "INSERT INTO income (description, amount, category_income_id, payment_method_id, date_record) "
             "VALUES ('%s', %.2f, %d, %d, '%s')", 
             description, amount, category_income_id, payment_method_id, date_record);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Income insertion failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Income added successfully!\n");
}

void viewTransactions(PGconn *conn) {
    const char *query = "SELECT t.id, t.description, t.amount, t.type, c.name AS category, pm.method AS payment_method, co.name AS company, t.date_record, t.date "
                        "FROM transactions t "
                        "LEFT JOIN categories c ON t.category_id = c.id "
                        "LEFT JOIN payment_methods pm ON t.payment_method_id = pm.id "
                        "LEFT JOIN companies co ON t.company_id = co.id "
                        "ORDER BY t.date DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No transactions found.\n");
    } else {
        printf("ID | Description       | Amount  | Type    | Category      | Payment Method | Company        | Date Record | Date\n");
        printf("----------------------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-17s | %-7s | %-7s | %-13s | %-14s | %-14s | %-11s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4),
                   PQgetvalue(res, i, 5),
                   PQgetvalue(res, i, 6),
                   PQgetvalue(res, i, 7),
                   PQgetvalue(res, i, 8));
        }
    }
    PQclear(res);
}

void viewIncome(PGconn *conn) {
    const char *query = "SELECT i.id, i.description, i.amount, ci.name AS category, pm.method AS payment_method, i.date_record, i.date "
                        "FROM income i "
                        "LEFT JOIN categories_income ci ON i.category_income_id = ci.id "
                        "LEFT JOIN payment_methods pm ON i.payment_method_id = pm.id "
                        "ORDER BY i.date DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No income records found.\n");
    } else {
        printf("ID | Description       | Amount  | Category      | Payment Method | Date Record | Date\n");
        printf("------------------------------------------------------------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-17s | %-7s | %-13s | %-14s | %-11s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4),
                   PQgetvalue(res, i, 5),
                   PQgetvalue(res, i, 6));
        }
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

void viewIncomeCategories(PGconn *conn) {
    const char *query = "SELECT * FROM categories_income ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Income Category\n");
    printf("-------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1));
    }
    PQclear(res);
}

void viewPaymentMethods(PGconn *conn) {
    const char *query = "SELECT * FROM payment_methods ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Payment Method\n");
    printf("------------------\n");
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

    // Detect and display the operating system
    detectOS();

    int choice;
    char description[100];
    float amount;
    int category_id, category_income_id, payment_method_id;
    char company_name[100], company_location[100];
    char date_record[11]; // YYYY-MM-DD format

    while (1) {
        clearScreen();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Add Income
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewIncomeCategories(conn);
                printf("Enter income category ID: ");
                scanf("%d", &category_income_id);
                viewPaymentMethods(conn);
                printf("Enter payment method ID (1 for Cash, 2 for Credit Card, 3 for Debit Card): ");
                scanf("%d", &payment_method_id);
                printf("Enter date record (YYYY-MM-DD): ");
                scanf(" %[^\n]", date_record);
                addIncome(conn, description, amount, category_income_id, payment_method_id, date_record);
                break;
            case 2: // Add Expense
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                viewPaymentMethods(conn);
                printf("Enter payment method ID (1 for Cash, 2 for Credit Card, 3 for Debit Card): ");
                scanf("%d", &payment_method_id);
                printf("Enter company name (or leave blank): ");
                scanf(" %[^\n]", company_name);
                if (company_name[0] != '\0') {
                    printf("Enter company location (or leave blank): ");
                    scanf(" %[^\n]", company_location);
                } else {
                    company_location[0] = '\0';
                }
                printf("Enter date record (YYYY-MM-DD): ");
                scanf(" %[^\n]", date_record);
                addTransaction(conn, description, amount, "expense", category_id, payment_method_id, company_name, company_location, date_record);
                break;
            case 3: // View Transactions
                viewTransactions(conn);
                break;
            case 4: // View Income
                viewIncome(conn);
                break;
            case 5: // View Transaction Categories
                viewCategories(conn);
                break;
            case 6: // View Income Categories
                viewIncomeCategories(conn);
                break;
            case 7: // View Payment Methods
                viewPaymentMethods(conn);
                break;
            case 8: // Exit
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

-- Categories for transactions
CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- Categories for income
CREATE TABLE categories_income (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- Payment methods
CREATE TABLE payment_methods (
    id SERIAL PRIMARY KEY,
    method VARCHAR(50) NOT NULL UNIQUE
);

-- Companies
CREATE TABLE companies (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    location VARCHAR(100)
);

-- Transactions table (for expenses)
CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    type VARCHAR(10) CHECK (type IN ('income', 'expense')) NOT NULL,
    date_record DATE NOT NULL, -- Manually input by the user
    date DATE DEFAULT CURRENT_DATE, -- Automatically set by the database
    category_id INT REFERENCES categories(id) ON DELETE SET NULL,
    payment_method_id INT REFERENCES payment_methods(id) ON DELETE SET NULL,
    company_id INT REFERENCES companies(id) ON DELETE SET NULL
);

-- Income table
CREATE TABLE income (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    date_record DATE NOT NULL, -- Manually input by the user
    date DATE DEFAULT CURRENT_DATE, -- Automatically set by the database
    category_income_id INT REFERENCES categories_income(id) ON DELETE SET NULL,
    payment_method_id INT REFERENCES payment_methods(id) ON DELETE SET NULL
);

-- Insert initial data for transaction categories
INSERT INTO categories (name) VALUES
('Food'),
('Bills'),
('Car'),
('Clothing'),
('Family'),
('Leisure'),
('Payments'),
('School'),
('Services'),
('Health'),
('Home'),
('Transport'),
('Entertainment'),
('Others');

-- Insert initial data for income categories
INSERT INTO categories_income (name) VALUES
('Payments'),
('Commission'),
('Recurring'),
('Sales'),
('Savings');

-- Insert initial data for payment methods
INSERT INTO payment_methods (method) VALUES
('Cash'),
('Credit Card'),
('Debit Card');


*/



















/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    printf("1. Add Income (e.g., Salary)\n");
    printf("2. Add Expense\n");
    printf("3. View Transactions\n");
    printf("4. View Income\n");
    printf("5. View Transaction Categories\n");
    printf("6. View Income Categories\n");
    printf("7. View Payment Methods\n");
    printf("8. Exit\n");
}

// Function to get the current date in YYYY-MM-DD format
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Function to detect the operating system
void detectOS() {
    #ifdef _WIN32
    printf("Operating System: Windows\n");
    #else
    printf("Operating System: Linux\n");
    #endif
}

void addTransaction(PGconn *conn, const char *description, float amount, const char *type, int category_id, int payment_method_id, const char *company_name, const char *company_location, const char *date_record) {
    char query[512];
    if (company_name[0] == '\0') {
        // If company name is empty, insert NULL for company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, date_record) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, '%s')", 
                 description, amount, type, category_id, payment_method_id, date_record);
    } else {
        // Insert company if it doesn't exist
        char company_query[256];
        snprintf(company_query, sizeof(company_query), 
                 "INSERT INTO companies (name, location) VALUES ('%s', '%s') ON CONFLICT (name) DO NOTHING", 
                 company_name, company_location);
        PGresult *company_res = PQexec(conn, company_query);
        if (PQresultStatus(company_res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Company insertion failed: %s", PQerrorMessage(conn));
            PQclear(company_res);
            return;
        }
        PQclear(company_res);

        // Insert transaction with company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, company_id, date_record) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, (SELECT id FROM companies WHERE name = '%s'), '%s')", 
                 description, amount, type, category_id, payment_method_id, company_name, date_record);
    }

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Transaction failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Transaction added successfully!\n");
}

void addIncome(PGconn *conn, const char *description, float amount, int category_income_id, int payment_method_id, const char *date_record) {
    char query[512];
    snprintf(query, sizeof(query), 
             "INSERT INTO income (description, amount, category_income_id, payment_method_id, date_record) "
             "VALUES ('%s', %.2f, %d, %d, '%s')", 
             description, amount, category_income_id, payment_method_id, date_record);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Income insertion failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);
    printf("Income added successfully!\n");
}

void viewTransactions(PGconn *conn) {
    const char *query = "SELECT t.id, t.description, t.amount, t.type, c.name AS category, pm.method AS payment_method, co.name AS company, t.date_record "
                        "FROM transactions t "
                        "LEFT JOIN categories c ON t.category_id = c.id "
                        "LEFT JOIN payment_methods pm ON t.payment_method_id = pm.id "
                        "LEFT JOIN companies co ON t.company_id = co.id "
                        "ORDER BY t.date_record DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No transactions found.\n");
    } else {
        printf("ID | Description       | Amount  | Type    | Category      | Payment Method | Company        | Date\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-17s | %-7s | %-7s | %-13s | %-14s | %-14s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4),
                   PQgetvalue(res, i, 5),
                   PQgetvalue(res, i, 6),
                   PQgetvalue(res, i, 7));
        }
    }
    PQclear(res);
}

void viewIncome(PGconn *conn) {
    const char *query = "SELECT i.id, i.description, i.amount, ci.name AS category, pm.method AS payment_method, i.date_record "
                        "FROM income i "
                        "LEFT JOIN categories_income ci ON i.category_income_id = ci.id "
                        "LEFT JOIN payment_methods pm ON i.payment_method_id = pm.id "
                        "ORDER BY i.date_record DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No income records found.\n");
    } else {
        printf("ID | Description       | Amount  | Category      | Payment Method | Date\n");
        printf("-----------------------------------------------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-17s | %-7s | %-13s | %-14s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4),
                   PQgetvalue(res, i, 5));
        }
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

void viewIncomeCategories(PGconn *conn) {
    const char *query = "SELECT * FROM categories_income ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Income Category\n");
    printf("-------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1));
    }
    PQclear(res);
}

void viewPaymentMethods(PGconn *conn) {
    const char *query = "SELECT * FROM payment_methods ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Payment Method\n");
    printf("------------------\n");
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

    // Detect and display the operating system
    detectOS();

    int choice;
    char description[100];
    float amount;
    int category_id, category_income_id, payment_method_id;
    char company_name[100], company_location[100];
    char date_record[11]; // YYYY-MM-DD format

    while (1) {
        clearScreen();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Add Income
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewIncomeCategories(conn);
                printf("Enter income category ID: ");
                scanf("%d", &category_income_id);
                viewPaymentMethods(conn);
                printf("Enter payment method ID (1 for Cash, 2 for Credit Card, 3 for Debit Card): ");
                scanf("%d", &payment_method_id);
                printf("Enter date (YYYY-MM-DD) or leave blank for today: ");
                scanf(" %[^\n]", date_record);
                if (date_record[0] == '\0') {
                    getCurrentDate(date_record); // Automatically set today's date
                }
                addIncome(conn, description, amount, category_income_id, payment_method_id, date_record);
                break;
            case 2: // Add Expense
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                viewPaymentMethods(conn);
                printf("Enter payment method ID (1 for Cash, 2 for Credit Card, 3 for Debit Card): ");
                scanf("%d", &payment_method_id);
                printf("Enter company name (or leave blank): ");
                scanf(" %[^\n]", company_name);
                if (company_name[0] != '\0') {
                    printf("Enter company location (or leave blank): ");
                    scanf(" %[^\n]", company_location);
                } else {
                    company_location[0] = '\0';
                }
                printf("Enter date (YYYY-MM-DD) or leave blank for today: ");
                scanf(" %[^\n]", date_record);
                if (date_record[0] == '\0') {
                    getCurrentDate(date_record); // Automatically set today's date
                }
                addTransaction(conn, description, amount, "expense", category_id, payment_method_id, company_name, company_location, date_record);
                break;
            case 3: // View Transactions
                viewTransactions(conn);
                break;
            case 4: // View Income
                viewIncome(conn);
                break;
            case 5: // View Transaction Categories
                viewCategories(conn);
                break;
            case 6: // View Income Categories
                viewIncomeCategories(conn);
                break;
            case 7: // View Payment Methods
                viewPaymentMethods(conn);
                break;
            case 8: // Exit
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

-- Categories for transactions
CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- Categories for income
CREATE TABLE categories_income (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- Payment methods
CREATE TABLE payment_methods (
    id SERIAL PRIMARY KEY,
    method VARCHAR(50) NOT NULL UNIQUE
);

-- Companies
CREATE TABLE companies (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    location VARCHAR(100)
);

-- Transactions table (for expenses)
CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    type VARCHAR(10) CHECK (type IN ('income', 'expense')) NOT NULL,
    date_record DATE DEFAULT CURRENT_DATE,
    category_id INT REFERENCES categories(id) ON DELETE SET NULL,
    payment_method_id INT REFERENCES payment_methods(id) ON DELETE SET NULL,
    company_id INT REFERENCES companies(id) ON DELETE SET NULL
);

-- Income table
CREATE TABLE income (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    date_record DATE DEFAULT CURRENT_DATE,
    category_income_id INT REFERENCES categories_income(id) ON DELETE SET NULL,
    payment_method_id INT REFERENCES payment_methods(id) ON DELETE SET NULL
);

-- Insert initial data for transaction categories
INSERT INTO categories (name) VALUES
('Food'),
('Bills'),
('Car'),
('Clothing'),
('Family'),
('Leisure'),
('Payments'),
('School'),
('Services'),
('Health'),
('Home'),
('Transport'),
('Entertainment'),
('Others');

-- Insert initial data for income categories
INSERT INTO categories_income (name) VALUES
('Payments'),
('Commission'),
('Recurring'),
('Sales'),
('Savings');

-- Insert initial data for payment methods
INSERT INTO payment_methods (method) VALUES
('Cash'),
('Credit Card'),
('Debit Card');







ALTER TABLE transactions ADD COLUMN date DATE DEFAULT CURRENT_DATE;






*/


























// -------------------------------------------------------------------------------------------------------------------------------------------------------------



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
    printf("5. View Payment Methods\n");
    printf("6. View Companies\n");
    printf("7. Exit\n");
}

void addTransaction(PGconn *conn, const char *description, float amount, const char *type, int category_id, int payment_method_id, const char *company_name, const char *company_location, const char *date) {
    char query[512];
    if (company_name[0] == '\0') {
        // If company name is empty, insert NULL for company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, date) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, '%s')", 
                 description, amount, type, category_id, payment_method_id, date);
    } else {
        // Insert company if it doesn't exist
        char company_query[256];
        snprintf(company_query, sizeof(company_query), 
                 "INSERT INTO companies (name, location) VALUES ('%s', '%s') ON CONFLICT (name) DO NOTHING", 
                 company_name, company_location);
        PGresult *company_res = PQexec(conn, company_query);
        if (PQresultStatus(company_res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Company insertion failed: %s", PQerrorMessage(conn));
            PQclear(company_res);
            return;
        }
        PQclear(company_res);

        // Insert transaction with company_id
        snprintf(query, sizeof(query), 
                 "INSERT INTO transactions (description, amount, type, category_id, payment_method_id, company_id, date) "
                 "VALUES ('%s', %.2f, '%s', %d, %d, (SELECT id FROM companies WHERE name = '%s'), '%s')", 
                 description, amount, type, category_id, payment_method_id, company_name, date);
    }

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
    const char *query = "SELECT t.id, t.description, t.amount, t.type, c.name AS category, pm.method AS payment_method, co.name AS company, t.date "
                        "FROM transactions t "
                        "LEFT JOIN categories c ON t.category_id = c.id "
                        "LEFT JOIN payment_methods pm ON t.payment_method_id = pm.id "
                        "LEFT JOIN companies co ON t.company_id = co.id "
                        "ORDER BY t.date DESC";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No transactions found.\n");
    } else {
        printf("ID | Description       | Amount  | Type    | Category      | Payment Method | Company        | Date\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-17s | %-7s | %-7s | %-13s | %-14s | %-14s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4),
                   PQgetvalue(res, i, 5),
                   PQgetvalue(res, i, 6),
                   PQgetvalue(res, i, 7));
        }
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

void viewPaymentMethods(PGconn *conn) {
    const char *query = "SELECT * FROM payment_methods ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("ID | Payment Method\n");
    printf("------------------\n");
    for (int i = 0; i < rows; i++) {
        printf("%s | %s\n",
               PQgetvalue(res, i, 0),
               PQgetvalue(res, i, 1));
    }
    PQclear(res);
}

void viewCompanies(PGconn *conn) {
    const char *query = "SELECT * FROM companies ORDER BY id";
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No companies found.\n");
    } else {
        printf("ID | Company Name       | Location\n");
        printf("----------------------------------\n");
        for (int i = 0; i < rows; i++) {
            printf("%s | %-18s | %s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2));
        }
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
    int category_id, payment_method_id;
    char company_name[100], company_location[100];
    char date[11]; // YYYY-MM-DD format

    while (1) {
        clearScreen();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            case 2:
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter amount: ");
                scanf("%f", &amount);
                viewCategories(conn);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                viewPaymentMethods(conn);
                printf("Enter payment method ID (1 for Cash, 2 for Credit Card, 3 for Debit Card): ");
                scanf("%d", &payment_method_id);
                printf("Enter company name (or leave blank): ");
                scanf(" %[^\n]", company_name);
                if (company_name[0] != '\0') {
                    printf("Enter company location (or leave blank): ");
                    scanf(" %[^\n]", company_location);
                } else {
                    company_location[0] = '\0';
                }
                printf("Enter date (YYYY-MM-DD): ");
                scanf("%s", date);
                addTransaction(conn, description, amount, choice == 1 ? "income" : "expense", category_id, payment_method_id, company_name, company_location, date);
                break;
            case 3:
                viewTransactions(conn);
                break;
            case 4:
                viewCategories(conn);
                break;
            case 5:
                viewPaymentMethods(conn);
                break;
            case 6:
                viewCompanies(conn);
                break;
            case 7:
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

CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE payment_methods (
    id SERIAL PRIMARY KEY,
    method VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE companies (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    location VARCHAR(100)
);

CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    description VARCHAR(100) NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    type VARCHAR(10) CHECK (type IN ('income', 'expense')) NOT NULL,
    date DATE NOT NULL,
    category_id INT REFERENCES categories(id) ON DELETE SET NULL,
    payment_method_id INT REFERENCES payment_methods(id) ON DELETE SET NULL,
    company_id INT REFERENCES companies(id) ON DELETE SET NULL
);

-- Insert initial data for payment methods
INSERT INTO payment_methods (method) VALUES
('Cash'),
('Credit Card'),
('Debit Card');

-- Insert initial data for categories
INSERT INTO categories (name) VALUES
('Food'),
('Bills'),
('Car'),
('Clothing'),
('Family'),
('Leisure'),
('Payments'),
('School'),
('Services'),
('Health'),
('Home'),
('Transport'),
('Entertainment'),
('Others');






*/













// -------------------------------------------------------------------------------------------------------------------------------------------------------------


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
('Car'),
('Clothing'),
('Family'),
('Leisure'),
('Payments'),
('School'),
('Services'),
('Health'),
('Home'),
('Transport'),
('Entertainment'),
('Others');





-- Add payment_method column to transactions table
--ALTER TABLE transactions ADD COLUMN payment_method VARCHAR(20) CHECK (payment_method IN ('cash', 'credit_card', 'debit_card'));







gcc -o finance_control finance_control.c -I "C:\Program Files\PostgreSQL\<version>\include" -L "C:\Program Files\PostgreSQL\<version>\lib" -lpq



gcc -o finance_control finance_control.c -I "C:\Program Files\PostgreSQL\16\include" -L "C:\Program Files\PostgreSQL\16\lib" -lpq




psql -d database -U user


psql -d shutdown_logs -U postgres
SELECT * FROM shutdown_events;











*/








