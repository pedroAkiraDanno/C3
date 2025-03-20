

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

int main() {
    printf("libpq tutorial\n");

    // Connect to the database
    // conninfo is a string of keywords and values separated by spaces.
    char *conninfo = "dbname=postgres user=postgres password=p0w2i8 host=localhost port=5432";

    // Create a connection
    PGconn *conn = PQconnectdb(conninfo);

    // Check if the connection is successful
    if (PQstatus(conn) != CONNECTION_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(conn));

        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
    }

    // We have successfully established a connection to the database server
    printf("Connection Established\n");
    printf("Port: %s\n", PQport(conn));
    printf("Host: %s\n", PQhost(conn));
    printf("DBName: %s\n", PQdb(conn));

    // Execute a query
    char *query = "SELECT * FROM your_table_name";

    // Submit the query and retrieve the result
    PGresult *res = PQexec(conn, query);

    // Check the status of the query result
    ExecStatusType resStatus = PQresultStatus(res);

    // Convert the status to a string and print it
    printf("Query Status: %s\n", PQresStatus(resStatus));

    // Check if the query execution was successful
    if (resStatus != PGRES_TUPLES_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));

        // Clear the result
        PQclear(res);

        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
    }

    // We have successfully executed the query
    printf("Query Executed Successfully\n");

    // Get the number of rows and columns in the query result
    int rows = PQntuples(res);
    int cols = PQnfields(res);
    printf("Number of rows: %d\n", rows);
    printf("Number of columns: %d\n", cols);

    // Print the column names
    for (int i = 0; i < cols; i++) {
        printf("%s\t", PQfname(res, i));
    }
    printf("\n");

    // Print all the rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Print the column value
            printf("%s\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    // Clear the result
    PQclear(res);

    // Finish the connection
    PQfinish(conn);

    return 0;
}
*/








/*
#include <libpq-fe.h>
#include <stdio.h>

int main() {
    const char *conninfo = "dbname=shutdown_logs user=postgres password=p0w2i8";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    printf("Connected to database successfully.\n");
    PQfinish(conn);
    return 0;
}

*/




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libpq-fe.h> // PostgreSQL C library


// Include platform-specific headers
#ifdef _WIN32
    #include <windows.h> // For Sleep() on Windows
#elif __linux__
    #include <unistd.h> // For sleep() on Linux
#endif



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
    printf("Shutdown event logged successfully.\n");
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

int main() {
    // Database connection parameters
    const char *conninfo = "dbname=shutdown_logs user=postgres password=p0w2i8 port=5432";
    //const char *conninfo = "dbname=shutdown_logs user=your_username password=your_password";  // port: 5432

    // Connect to the database
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    // Log the shutdown event
    log_shutdown_event(conn);

    // Close the database connection
    PQfinish(conn);

    // Shut down the computer
    shutdown_computer();

    return 0;
}
























/*


CREATE DATABASE shutdown_logs;

\c shutdown_logs;

CREATE TABLE shutdown_events (
    id SERIAL PRIMARY KEY,
    event_time TIMESTAMP NOT NULL
);








To include a shutdown command in the C program for both Windows and Linux, you can use platform-specific system calls. Below is the updated program that logs the shutdown event into the PostgreSQL database and then shuts down the computer.

---


---

### Explanation of Changes
1. *Platform-Specific Shutdown Command*:
   - On *Windows*, the shutdown /s /t 0 command is used to shut down the system immediately.
   - On *Linux*, the shutdown now command is used to shut down the system immediately.
   - The #ifdef _WIN32 preprocessor directive ensures the correct command is used based on the operating system.

2. *shutdown_computer() Function*:
   - This function calls the appropriate system command to shut down the computer after logging the event.

---

### Compilation and Execution

#### On Linux:
1. Compile the program:
   bash
   gcc -o shutdown_logger shutdown_logger.c -I/usr/include/postgresql -lpq
   
2. Run the program (requires root privileges for shutdown):
   bash
   sudo ./shutdown_logger
   

#### On Windows:
1. Compile the program (adjust paths as needed):
   bash
   gcc -o shutdown_logger shutdown_logger.c -I"C:\Program Files\PostgreSQL\include" -L"C:\Program Files\PostgreSQL\lib" -lpq
   
2. Run the program (requires administrator privileges for shutdown):
   bash
   shutdown_logger.exe
   

---

### Notes:
1. *Permissions*:
   - On Linux, you need root privileges to shut down the system, so run the program with sudo.
   - On Windows, you need administrator privileges to shut down the system.

2. *PostgreSQL Credentials*:
   - Replace your_username and your_password in the conninfo string with your PostgreSQL credentials.

3. *Testing*:
   - Test the program in a safe environment to avoid accidental shutdowns.

4. *Error Handling*:
   - Add more robust error handling for production use, especially for database connections and system commands.

This program will log the shutdown event into the PostgreSQL database and then shut down the computer.









*/



















/*


### Explanation of Changes
1. *Platform-Specific Shutdown Command*:
   - On *Windows*, the shutdown /s /t 0 command is used to shut down the system immediately.
   - On *Linux*, the shutdown now command is used to shut down the system immediately.
   - The #ifdef _WIN32 preprocessor directive ensures the correct command is used based on the operating system.

2. *shutdown_computer() Function*:
   - This function calls the appropriate system command to shut down the computer after logging the event.

---

### Compilation and Execution

#### On Linux:
1. Compile the program:
   bash
   gcc -o shutdown_logger shutdown_logger.c -I/usr/include/postgresql -lpq
   
2. Run the program (requires root privileges for shutdown):
   bash
   sudo ./shutdown_logger
   

#### On Windows:
1. Compile the program (adjust paths as needed):
   bash
   gcc -o shutdown_logger shutdown_logger.c -I"C:\Program Files\PostgreSQL\include" -L"C:\Program Files\PostgreSQL\lib" -lpq
   
2. Run the program (requires administrator privileges for shutdown):
   bash
   shutdown_logger.exe
   

---

### Notes:
1. *Permissions*:
   - On Linux, you need root privileges to shut down the system, so run the program with sudo.
   - On Windows, you need administrator privileges to shut down the system.

2. *PostgreSQL Credentials*:
   - Replace your_username and your_password in the conninfo string with your PostgreSQL credentials.

3. *Testing*:
   - Test the program in a safe environment to avoid accidental shutdowns.

4. *Error Handling*:
   - Add more robust error handling for production use, especially for database connections and system commands.

This program will log the shutdown event into the PostgreSQL database and then shut down the computer.





gcc -I"C:\Program Files\PostgreSQL\17\include" shutdown_program_POSTGRESQL.c -o shutdown_program_POSTGRESQL.exe -L"C:\Program Files\PostgreSQL\17\lib" -lpq



gcc -I"C:\Program Files\PostgreSQL\16\include" -L"C:\Program Files\PostgreSQL\16\lib" shutdown_program_POSTGRESQL.c -o shutdown_program_POSTGRESQL.exe -lpq 


*/









