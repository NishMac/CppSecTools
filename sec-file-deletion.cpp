#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

void secureDelete(const std::string& filename) {
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // Determine the file size
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.close();

    // Overwrite the file with random data
    std::ofstream overwriteFile(filename, std::ios::binary);
    if (!overwriteFile.is_open()) {
        std::cerr << "Failed to open file for overwriting." << std::endl;
        return;
    }

    srand((unsigned)time(0));  // Seed random number generator
    for (int i = 0; i < fileSize; i++) {
        char randomChar = (char)(rand() % 256);  // Generate a random byte
        overwriteFile << randomChar;
    }

    overwriteFile.close();

    // Delete the file
    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Error deleting the file." << std::endl;
    } else {
        std::cout << "File securely deleted." << std::endl;
    }
}

int main() {
    std::string filename;
    std::cout << "Enter the filename to securely delete: ";
    std::cin >> filename;
    secureDelete(filename);

    return 0;
}
