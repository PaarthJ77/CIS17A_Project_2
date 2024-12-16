#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include <utility>

class InputHandler {
public:
    // Constructors
    InputHandler();
    InputHandler(const InputHandler& other); // Copy Constructor

    // Destructor
    ~InputHandler();

    // Assignment Operator
    InputHandler& operator=(const InputHandler& other);

    // Overloaded Operators
    bool operator==(const InputHandler& other) const;

    // Member functions
    std::string getPlayerName() const;
    std::pair<int, int> getAttackCoordinates() const;
};

#endif // INPUTHANDLER_H
