#include "inputhandler.h"
#include "common.h"
#include <iostream>
#include <cctype>

InputHandler::InputHandler() {}

InputHandler::InputHandler(const InputHandler& other) {}

InputHandler::~InputHandler() {}

InputHandler& InputHandler::operator=(const InputHandler& other) {
    if (this != &other) {
        // No dynamic members
    }
    return *this;
}

bool InputHandler::operator==(const InputHandler& other) const {
    // Define based on criteria, currently returns true
    return true;
}

std::string InputHandler::getPlayerName() const {
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    if (name.empty()) {
        name = "Player";
    }
    return name;
}

std::pair<int, int> InputHandler::getAttackCoordinates() const {
    std::string input;
    std::cout << "Enter attack coordinates (e.g., A5): ";
    std::getline(std::cin, input);

    if (input.length() < 2) {
        std::cout << "Invalid input. Please try again.\n";
        return getAttackCoordinates();
    }

    char rowChar = std::toupper(input[0]);
    if (rowChar < 'A' || rowChar >= 'A' + BOARD_SIZE) {
        std::cout << "Invalid row. Please try again.\n";
        return getAttackCoordinates();
    }
    int row = rowChar - 'A';

    std::string colStr = input.substr(1);
    int col;
    try {
        col = std::stoi(colStr);
    } catch (...) {
        std::cout << "Invalid column. Please try again.\n";
        return getAttackCoordinates();
    }

    if (col < 0 || col >= BOARD_SIZE) {
        std::cout << "Column out of range. Please try again.\n";
        return getAttackCoordinates();
    }

    return std::make_pair(row, col);  // Correctly return the pair
}
