
#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"
#include "gameexception.h"
#include <string>
#include <fstream>
#include <mutex>

class Logger {
private:
    std::string logFile;
    std::mutex mtx;

public:
    // Constructors
    Logger();
    Logger(const std::string& filename);
    Logger(const Logger& other); // Copy Constructor

    // Destructor
    ~Logger();

    // Assignment Operator
    Logger& operator=(const Logger& other);

    // Overloaded Operators
    bool operator==(const Logger& other) const;

    // Member functions
    void log(const std::string& message);
};

#endif // LOGGER_H
