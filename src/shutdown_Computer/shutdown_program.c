







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

    // Add a 5-second delay before shutting down
    printf("Shutting down the computer in 5 seconds...\n");
    #ifdef _WIN32
        Sleep(5000); // 5000 milliseconds = 5 seconds (Windows)
    #elif __linux__
        sleep(5); // 5 seconds (Linux)
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














