#include "OutputHandler.h"
#include <iostream>

// Default Constructor
OutputHandler::OutputHandler() {}

// Copy Constructor
OutputHandler::OutputHandler(const OutputHandler& other) {}

// Destructor
OutputHandler::~OutputHandler() {}

// Assignment Operator
OutputHandler& OutputHandler::operator=(const OutputHandler& other) {
    if (this != &other) {

    }
    return *this;
}

// Overloaded Equality Operator
bool OutputHandler::operator==(const OutputHandler& other) const {
    return true;
}

// Display Instructions
void OutputHandler::displayInstructions() const {
    std::cout << "========================================\n";
    std::cout << "          Welcome to Battleship!         \n";
    std::cout << "========================================\n";
    std::cout << "Instructions:\n";
    std::cout << "1. The game board is 10x10 (A-J rows, 0-9 columns).\n";
    std::cout << "2. Attack positions by entering coordinates like A5.\n";
    std::cout << "3. 'H' for hit, 'M' for miss.\n";
    std::cout << "4. Sink all ships to win.\n";
    std::cout << "5. Game auto-saves periodically.\n";
    std::cout << "========================================\n";
}

// Display a message
void OutputHandler::displayMessage(const std::string& message) const {
    std::cout << message << "\n";
}

// Handle Method (Placeholder)
void OutputHandler::handle() {
    std::cout << "Handling output...\n";
}
