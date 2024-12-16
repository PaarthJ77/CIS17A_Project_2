#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <iostream>
#include <string>

class GameEntity {
public:
    // Virtual destructor for polymorphic behavior
    virtual ~GameEntity() {}

    // Pure virtual function for displaying entity details
    virtual void display() const = 0;

    // Getter for the name
    virtual std::string getName() const = 0;
};

#endif // GAMEENTITY_H
