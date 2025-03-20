








#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep function
#include <libpq-fe.h> // PostgreSQL C library

// Function to insert shutdown event into the database
void log_shutdown_event(PGconn *conn) {
    // Get the current timestamp
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Prepare the SQL query
    char query[128];
    snprintf(query, sizeof(query), "INSERT INTO shutdown_events (event_time) VALUES ('%s')", timestamp);

    // Execute the query
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error inserting record: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);
    printf("Shutdown event logged successfully in PostgreSQL.\n");
}

// Function to shut down the computer
void shutdown_computer() {
    #ifdef _WIN32
        // Windows shutdown command
        system("shutdown /s /t 0");
    #else
        // Linux shutdown command
        system("shutdown now");
    #endif
}

// Function to get the shutdown delay in minutes from the user
int get_shutdown_delay() {
    int minutes;
    printf("Enter the shutdown delay in minutes: ");
    scanf("%d", &minutes);
    return minutes;
}

// Function to display messages with a delay
void display_messages_with_delay(int minutes) {
    int seconds = minutes * 60;
    while (seconds > 0) {
        printf("Shutting down in %d seconds...\n", seconds);
        sleep(10); // Delay for 10 seconds
        seconds -= 10;
    }
}

// Function to detect the operating system
void detect_os() {
    #ifdef _WIN32
        printf("Operating System: Windows\n");
    #else
        printf("Operating System: Linux\n");
    #endif
}

int main() {
    // Detect and display the operating system
    detect_os();

    // Add a 1-second delay before displaying further information
    //sleep(1);

    // Database connection parameters
    const char *conninfo = "dbname=shutdown_logs user=postgres password=p0w2i8 port=5432";

    // Connect to the database
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    // Get the shutdown delay from the user
    int shutdown_delay_minutes = get_shutdown_delay();
    int shutdown_delay_seconds = shutdown_delay_minutes * 60;

    // Log the shutdown event
    log_shutdown_event(conn);

    // Display messages with a delay
    display_messages_with_delay(shutdown_delay_minutes);

    // Close the database connection
    PQfinish(conn);

    // Shut down the computer
    shutdown_computer();

    return 0;
}
























/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep function
#include <libpq-fe.h> // PostgreSQL C library

// Function to insert shutdown event into the database
void log_shutdown_event(PGconn *conn) {
    // Get the current timestamp
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Prepare the SQL query
    char query[128];
    snprintf(query, sizeof(query), "INSERT INTO shutdown_events (event_time) VALUES ('%s')", timestamp);

    // Execute the query
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error inserting record: %s", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);
    printf("Shutdown event logged successfully in PostgreSQL.\n");
}

// Function to shut down the computer
void shutdown_computer() {
    #ifdef _WIN32
        // Windows shutdown command
        system("shutdown /s /t 0");
    #else
        // Linux shutdown command
        system("shutdown now");
    #endif
}

// Function to get the shutdown delay in minutes from the user
int get_shutdown_delay() {
    int minutes;
    printf("Enter the shutdown delay in minutes: ");
    scanf("%d", &minutes);
    return minutes;
}

// Function to display messages with a delay
void display_messages_with_delay(int minutes) {
    int seconds = minutes * 60;
    while (seconds > 0) {
        printf("Shutting down in %d seconds...\n", seconds);
        sleep(5); // Delay for 5 seconds
        seconds -= 5;
    }
}







int main() {
    // Database connection parameters
    const char *conninfo = "dbname=shutdown_logs user=postgres password=p0w2i8 port=5432";

    // Connect to the database
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    // Get the shutdown delay from the user
    int shutdown_delay_minutes = get_shutdown_delay();
    int shutdown_delay_seconds = shutdown_delay_minutes * 60;

    // Log the shutdown event
    log_shutdown_event(conn);

    // Display messages with a delay
    display_messages_with_delay(shutdown_delay_minutes);

    // Close the database connection
    PQfinish(conn);

    // Shut down the computer
    shutdown_computer();

    return 0;
}
*/














































/*

CREATE DATABASE shutdown_logs;

\c shutdown_logs;

CREATE TABLE shutdown_events (
    id SERIAL PRIMARY KEY,
    event_time TIMESTAMP NOT NULL
);








gcc shutdown_program_Time_POSTGRESQL.c -o shutdown_program_Time_POSTGRESQL.exe -I "C:\Program Files\PostgreSQL\17\include" -L "C:\Program Files\PostgreSQL\17\lib" -lpq

gcc shutdown_program_Time_POSTGRESQL.c -o shutdown_program_Time_POSTGRESQL.exe -I "C:\Program Files\PostgreSQL\16\include" -L "C:\Program Files\PostgreSQL\16\lib" -lpq






psql -d database -U user


psql -d shutdown_logs -U postgres
SELECT * FROM shutdown_events;







*/






