#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

class PasswordStrengthChecker {
public:
    static std::string checkStrength(const std::string& password) {
        int lengthCriteria = password.length() >= 8;  // Minimum length of 8 characters
        int digitCriteria = std::count_if(password.begin(), password.end(), ::isdigit) > 0;  // At least one digit
        int upperCriteria = std::count_if(password.begin(), password.end(), ::isupper) > 0;  // At least one uppercase letter
        int lowerCriteria = std::count_if(password.begin(), password.end(), ::islower) > 0;  // At least one lowercase letter
        int specialCriteria = std::count_if(password.begin(), password.end(), ::ispunct) > 0;  // At least one special character

        int score = lengthCriteria + digitCriteria + upperCriteria + lowerCriteria + specialCriteria;

        switch (score) {
            case 5: return "Very Strong";
            case 4: return "Strong";
            case 3: return "Average";
            case 2: return "Weak";
            default: return "Very Weak";
        }
    }
};

int main() {
    std::string password;
    std::cout << "Enter your password to check its strength: ";
    std::cin >> password;
    
    std::string strength = PasswordStrengthChecker::checkStrength(password);
    std::cout << "Your password is: " << strength << std::endl;

    return 0;
}
