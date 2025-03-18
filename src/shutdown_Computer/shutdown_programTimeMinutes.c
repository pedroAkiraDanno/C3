









#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include platform-specific headers
#ifdef _WIN32
    #include <windows.h> // For Sleep() on Windows
#elif __linux__
    #include <unistd.h> // For sleep() on Linux
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

int main() {
    // Detect the operating system at runtime
    const char* os = detect_os();
    printf("Detected OS: %s\n", os);

    // Prompt the user to enter the delay time in minutes
    int delay_minutes;
    printf("Enter the delay time in minutes (e.g., 5 or 20): ");
    scanf("%d", &delay_minutes);

    // Validate the input
    if (delay_minutes <= 0) {
        printf("Invalid delay time. Please enter a positive number.\n");
        return 1; // Exit with an error code
    }

    // Convert minutes to seconds for Linux or milliseconds for Windows
    #ifdef _WIN32
        int delay_milliseconds = delay_minutes * 60 * 1000; // Convert minutes to milliseconds
    #elif __linux__
        int delay_seconds = delay_minutes * 60; // Convert minutes to seconds
    #endif

    // Display the countdown message
    printf("Shutting down the computer in %d minutes...\n", delay_minutes);

    // Add the delay before shutting down
    #ifdef _WIN32
        Sleep(delay_milliseconds); // Delay in milliseconds (Windows)
    #elif __linux__
        sleep(delay_seconds); // Delay in seconds (Linux)
    #else
        printf("Unsupported operating system.\n");
        return 1; // Exit with an error code
    #endif

    // Shutdown the computer based on the detected OS
    if (strcmp(os, "Windows") == 0) {
        system("shutdown /s /t 0"); // Shutdown command for Windows
    } else if (strcmp(os, "Linux") == 0) {
        system("shutdown -h now"); // Shutdown command for Linux
    } else {
        printf("Unsupported operating system.\n");
        return 1; // Exit with an error code
    }

    return 0;
}






