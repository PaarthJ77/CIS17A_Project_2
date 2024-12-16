#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <string>
#include <iostream>

class OutputHandler {
public:
    // Constructors
    OutputHandler();
    OutputHandler(const OutputHandler& other); // Copy Constructor

    // Destructor
    ~OutputHandler();

    // Assignment Operator
    OutputHandler& operator=(const OutputHandler& other);

    // Overloaded Operators
    bool operator==(const OutputHandler& other) const;

    // Member functions
    void displayInstructions() const;
    void displayMessage(const std::string& message) const;
    void handle(); // Placeholder handle method
};

#endif // OUTPUTHANDLER_H
