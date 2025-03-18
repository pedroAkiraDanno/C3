




















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
    const char* os = (const char*)lpParam;

    // Add a 5-second delay before shutting down
    printf("Shutting down the computer in 5 seconds...\n");
    #ifdef _WIN32
        Sleep(10000); // 5000 milliseconds = 5 seconds (Windows)
    #elif __linux__
        sleep(10); // 5 seconds (Linux)
    #endif

    // Shutdown the computer based on the detected OS
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

    // Create a thread to handle the shutdown process
    #ifdef _WIN32
        HANDLE thread = CreateThread(NULL, 0, shutdown_thread, (LPVOID)os, 0, NULL);
        if (thread == NULL) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Set a name for the thread (Windows workaround)
        const char* thread_name = "ShutdownThread";
        printf("Thread name: %s\n", thread_name);
        // Detach the thread so it continues running even if the main program exits
        CloseHandle(thread);
    #elif __linux__
        pthread_t thread;
        if (pthread_create(&thread, NULL, shutdown_thread, (void*)os) != 0) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Set a name for the thread (Linux)
        const char* thread_name = "ShutdownThread";
        if (pthread_setname_np(thread, thread_name) != 0) {
            printf("Failed to set thread name.\n");
        } else {
            printf("Thread name: %s\n", thread_name);
        }
        // Detach the thread so it continues running even if the main program exits
        pthread_detach(thread);
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    printf("Shutdown process started. You can close this program.\n");

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
    const char* os = (const char*)lpParam;

    // Add a 5-second delay before shutting down
    printf("Shutting down the computer in 5 seconds...\n");
    #ifdef _WIN32
        Sleep(5000); // 5000 milliseconds = 5 seconds (Windows)
    #elif __linux__
        sleep(5); // 5 seconds (Linux)
    #endif

    // Shutdown the computer based on the detected OS
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

    // Create a thread to handle the shutdown process
    #ifdef _WIN32
        HANDLE thread = CreateThread(NULL, 0, shutdown_thread, (LPVOID)os, 0, NULL);
        if (thread == NULL) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        CloseHandle(thread);
    #elif __linux__
        pthread_t thread;
        if (pthread_create(&thread, NULL, shutdown_thread, (void*)os) != 0) {
            printf("Failed to create thread.\n");
            return 1;
        }
        // Detach the thread so it continues running even if the main program exits
        pthread_detach(thread);
    #else
        printf("Unsupported operating system.\n");
        return 1;
    #endif

    printf("Shutdown process started. You can close this program.\n");

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









