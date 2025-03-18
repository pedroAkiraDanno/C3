

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include platform-specific headers
#ifdef _WIN32
    #include <windows.h> // For Sleep(), CreateThread, GetCurrentThreadId, and GetCurrentProcessId on Windows
#elif __linux__
    #include <unistd.h> // For sleep(), pthread, and getpid on Linux
    #include <pthread.h> // For threading on Linux
    #include <sys/types.h> // For gettid()
    #define _GNU_SOURCE // Required for gettid()
    #include <sys/syscall.h> // For syscall()
#endif

// Function to detect the operating system at runtime
const char* detect_os() {
    #ifdef _WIN32
        return "Windows";
    #elif __linux__
        return "Linux";
    #else
        return "Unknown";
    #endif
}

// Function to handle the shutdown process
#ifdef _WIN32
    DWORD WINAPI shutdown_thread(LPVOID lpParam) {
#elif __linux__
    void* shutdown_thread(void* arg) {
#endif
    int delay_minutes = *(int*)lpParam;

    // Set the thread name
    const char* thread_name = "ShutdownThread";
    #ifdef _WIN32
        printf("Thread name: %s\n", thread_name);
    #elif __linux__
        if (pthread_setname_np(pthread_self(), thread_name) != 0) {
            printf("Failed to set thread name.\n");
        } else {
            printf("Thread name: %s\n", thread_name);
        }
    #endif

    // Get the thread PID (TID on Linux, Thread ID on Windows)
    #ifdef _WIN32
        DWORD thread_id = GetCurrentThreadId();
        printf("Thread PID: %lu\n", thread_id);
    #elif __linux__
        pid_t tid = syscall(SYS_gettid);
        printf("Thread PID: %d\n", tid);
    #endif

    // Convert minutes to seconds for Linux or milliseconds for Windows
    #ifdef _WIN32
        int delay_milliseconds = delay_minutes * 60 * 1000; // Convert minutes to milliseconds
        printf("Waiting for %d minutes (%d milliseconds)...\n", delay_minutes, delay_milliseconds);
        Sleep(delay_milliseconds); // Delay in milliseconds (Windows)
    #elif __linux__
        int delay_seconds = delay_minutes * 60; // Convert minutes to seconds
        printf("Waiting for %d minutes (%d seconds)...\n", delay_minutes, delay_seconds);
        sleep(delay_seconds); // Delay in seconds (Linux)
    #endif

    // Shutdown the computer based on the detected OS
    const char* os = detect_os();
    if (strcmp(os, "Windows") == 0) {
        printf("Executing shutdown command...\n");
        system("shutdown /s /t 0"); // Shutdown command for Windows
    } else if (strcmp(os, "Linux") == 0) {
        printf("Executing shutdown command...\n");
        system("shutdown -h now"); // Shutdown command for Linux
    }

    #ifdef _WIN32
        return 0;
    #elif __linux__
        return NULL;
    #endif
}

int main() {
    // Detect the operating system at runtime
    const char* os = detect_os();
    printf("Detected OS: %s\n", os);

    // Get the program PID
    #ifdef _WIN32
        DWORD program_pid = GetCurrentProcessId();
        printf("Program PID: %lu\n", program_pid);
    #elif __linux__
        pid_t program_pid = getpid();
        printf("Program PID: %d\n", program_pid);
    #endif

    // Prompt the user to enter the delay time in minutes
    int delay_minutes;
    printf("Enter the delay time in minutes (e.g., 5 or 25): ");
    scanf("%d", &delay_minutes);

    // Validate the input
    if (delay_minutes <= 0) {
        printf("Invalid delay time. Please enter a positive number.\n");
        return 1;
    }

    // Create a thread to handle the shutdown process
    #ifdef _WIN32
        HANDLE thread = CreateThread(NULL, 0, shutdown_thread, &delay_minutes, 0, NULL);
        if (thread == NULL) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        CloseHandle(thread);
    #elif __linux__
        pthread_t thread;
        if (pthread_create(&thread, NULL, shutdown_thread, &delay_minutes) != 0) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        pthread_detach(thread);
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    printf("Shutdown process started. The computer will shut down in %d minutes.\n", delay_minutes);

    // Pause the program to allow the user to see the output
    #ifdef _WIN32
        system("pause"); // Pause for Windows
    #elif __linux__
        printf("Press Enter to exit...\n");
        getchar(); // Pause for Linux
    #endif

    return 0;
}














/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include platform-specific headers
#ifdef _WIN32
    #include <windows.h> // For Sleep(), CreateThread, and GetCurrentThreadId on Windows
#elif __linux__
    #include <unistd.h> // For sleep() and pthread on Linux
    #include <pthread.h> // For threading on Linux
    #include <sys/types.h> // For gettid()
    #define _GNU_SOURCE // Required for gettid()
    #include <sys/syscall.h> // For syscall()
#endif

// Function to detect the operating system at runtime
const char* detect_os() {
    #ifdef _WIN32
        return "Windows";
    #elif __linux__
        return "Linux";
    #else
        return "Unknown";
    #endif
}

// Function to handle the shutdown process
#ifdef _WIN32
    DWORD WINAPI shutdown_thread(LPVOID lpParam) {
#elif __linux__
    void* shutdown_thread(void* arg) {
#endif
    int delay_minutes = *(int*)lpParam;

    // Set the thread name
    const char* thread_name = "ShutdownThread";
    #ifdef _WIN32
        printf("Thread name: %s\n", thread_name);
    #elif __linux__
        if (pthread_setname_np(pthread_self(), thread_name) != 0) {
            printf("Failed to set thread name.\n");
        } else {
            printf("Thread name: %s\n", thread_name);
        }
    #endif

    // Get the thread PID (TID on Linux, Thread ID on Windows)
    #ifdef _WIN32
        DWORD thread_id = GetCurrentThreadId();
        printf("Thread PID: %lu\n", thread_id);
    #elif __linux__
        pid_t tid = syscall(SYS_gettid);
        printf("Thread PID: %d\n", tid);
    #endif

    // Convert minutes to seconds for Linux or milliseconds for Windows
    #ifdef _WIN32
        int delay_milliseconds = delay_minutes * 60 * 1000; // Convert minutes to milliseconds
        Sleep(delay_milliseconds); // Delay in milliseconds (Windows)
    #elif __linux__
        int delay_seconds = delay_minutes * 60; // Convert minutes to seconds
        sleep(delay_seconds); // Delay in seconds (Linux)
    #endif

    // Shutdown the computer based on the detected OS
    const char* os = detect_os();
    if (strcmp(os, "Windows") == 0) {
        system("shutdown /s /t 0"); // Shutdown command for Windows
    } else if (strcmp(os, "Linux") == 0) {
        system("shutdown -h now"); // Shutdown command for Linux
    }

    #ifdef _WIN32
        return 0;
    #elif __linux__
        return NULL;
    #endif
}

int main() {
    // Detect the operating system at runtime
    const char* os = detect_os();
    printf("Detected OS: %s\n", os);

    // Prompt the user to enter the delay time in minutes
    int delay_minutes;
    printf("Enter the delay time in minutes (e.g., 5 or 25): ");
    scanf("%d", &delay_minutes);

    // Validate the input
    if (delay_minutes <= 0) {
        printf("Invalid delay time. Please enter a positive number.\n");
        return 1;
    }

    // Create a thread to handle the shutdown process
    #ifdef _WIN32
        HANDLE thread = CreateThread(NULL, 0, shutdown_thread, &delay_minutes, 0, NULL);
        if (thread == NULL) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        CloseHandle(thread);
    #elif __linux__
        pthread_t thread;
        if (pthread_create(&thread, NULL, shutdown_thread, &delay_minutes) != 0) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        pthread_detach(thread);
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    printf("Shutdown process started. The computer will shut down in %d minutes.\n", delay_minutes);

    // Pause the program to allow the user to see the output
    #ifdef _WIN32
        system("pause"); // Pause for Windows
    #elif __linux__
        printf("Press Enter to exit...\n");
        getchar(); // Pause for Linux
    #endif

    return 0;
}
*/









/*

Compilation and Execution
Compile the Program:

On Windows:

bash
Copy
gcc -o shutdown_program shutdown_program.c
On Linux:

bash
Copy
gcc -o shutdown_program shutdown_program.c -pthread
Run the Program:

On Windows:

bash
Copy
shutdown_program.exe
On Linux:

bash
Copy
./shutdown_program


*/







/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include platform-specific headers
#ifdef _WIN32
    #include <windows.h> // For Sleep() and CreateThread on Windows
#elif __linux__
    #include <unistd.h> // For sleep() and pthread on Linux
    #include <pthread.h> // For threading on Linux
#endif

// Function to detect the operating system at runtime
const char* detect_os() {
    #ifdef _WIN32
        return "Windows";
    #elif __linux__
        return "Linux";
    #else
        return "Unknown";
    #endif
}

// Function to handle the shutdown process
#ifdef _WIN32
    DWORD WINAPI shutdown_thread(LPVOID lpParam) {
#elif __linux__
    void* shutdown_thread(void* arg) {
#endif
    int delay_minutes = *(int*)lpParam;

    // Convert minutes to seconds for Linux or milliseconds for Windows
    #ifdef _WIN32
        int delay_milliseconds = delay_minutes * 60 * 1000; // Convert minutes to milliseconds
        Sleep(delay_milliseconds); // Delay in milliseconds (Windows)
    #elif __linux__
        int delay_seconds = delay_minutes * 60; // Convert minutes to seconds
        sleep(delay_seconds); // Delay in seconds (Linux)
    #endif

    // Shutdown the computer based on the detected OS
    const char* os = detect_os();
    if (strcmp(os, "Windows") == 0) {
        system("shutdown /s /t 0"); // Shutdown command for Windows
    } else if (strcmp(os, "Linux") == 0) {
        system("shutdown -h now"); // Shutdown command for Linux
    }

    #ifdef _WIN32
        return 0;
    #elif __linux__
        return NULL;
    #endif
}

int main() {
    // Detect the operating system at runtime
    const char* os = detect_os();
    printf("Detected OS: %s\n", os);

    // Prompt the user to enter the delay time in minutes
    int delay_minutes;
    printf("Enter the delay time in minutes (e.g., 5 or 25): ");
    scanf("%d", &delay_minutes);

    // Validate the input
    if (delay_minutes <= 0) {
        printf("Invalid delay time. Please enter a positive number.\n");
        return 1;
    }

    // Create a thread to handle the shutdown process
    #ifdef _WIN32
        HANDLE thread = CreateThread(NULL, 0, shutdown_thread, &delay_minutes, 0, NULL);
        if (thread == NULL) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        CloseHandle(thread);
    #elif __linux__
        pthread_t thread;
        if (pthread_create(&thread, NULL, shutdown_thread, &delay_minutes) != 0) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        pthread_detach(thread);
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    printf("Shutdown process started. The computer will shut down in %d minutes.\n", delay_minutes);

    // Pause the program to allow the user to see the output
    #ifdef _WIN32
        system("pause"); // Pause for Windows
    #elif __linux__
        printf("Press Enter to exit...\n");
        getchar(); // Pause for Linux
    #endif

    return 0;
}

*/












