
#include "logger.h"
#include <iostream>
#include <ctime>

// Default Constructor
Logger::Logger()
    : logFile("battleship.log") {}

// Parameterized Constructor
Logger::Logger(const std::string& filename)
    : logFile(filename) {}

// Copy Constructor
Logger::Logger(const Logger& other)
    : logFile(other.logFile) {}

// Destructor
Logger::~Logger() {}

// Assignment Operator
Logger& Logger::operator=(const Logger& other) {
    if (this != &other) {
        logFile = other.logFile;
    }
    return *this;
}

// Overloaded Equality Operator
bool Logger::operator==(const Logger& other) const {
    return logFile == other.logFile;
}

// Log a message with timestamp
void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream outFile(logFile, std::ios::app);
    if (!outFile) {
        std::cout << "Error opening log file.\n";
        return;
    }

    // Get current time
    std::time_t now = std::time(nullptr);
    char* dt = std::ctime(&now);
    dt[strlen(dt) - 1] = '\0'; // Remove newline

    outFile << "[" << dt << "] " << message << "\n";
    outFile.close();
}
