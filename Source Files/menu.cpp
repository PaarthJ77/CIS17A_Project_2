#include "menu.h"
#include <iostream>

Menu::Menu() {}

Menu::Menu(const Menu& other) {}

Menu::~Menu() {}

Menu& Menu::operator=(const Menu& other) {
    if (this != &other) {
        // No dynamic members
    }
    return *this;
}

bool Menu::operator==(const Menu& other) const {
    return true;
}

int Menu::displayMainMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "               Battleship               \n";
    std::cout << "========================================\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. View High Scores\n";
    std::cout << "4. Reset Game\n";
    std::cout << "5. Run Tests\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();  // Clear newline character
    return choice;
}
