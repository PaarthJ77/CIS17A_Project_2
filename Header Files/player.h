#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "ship.h"
#include <string>
#include <array>
#include <iostream>

class Player {
private:
    std::string name;
    std::array<Ship, MAX_SHIPS> ships;

public:
    // Constructors
    Player();
    Player(const std::string& name);
    Player(const Player& other); // Copy Constructor

    // Destructor
    ~Player();

    // Assignment Operator
    Player& operator=(const Player& other);

    // Overloaded Operators
    bool operator==(const Player& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    friend std::istream& operator>>(std::istream& is, Player& player);

    // Member functions
    void setName(const std::string& name);
    std::string getName() const;

    // Ship Management
    void initializeShips();
    std::array<Ship, MAX_SHIPS>& getShips();
    const std::array<Ship, MAX_SHIPS>& getShips() const;
};

#endif // PLAYER_H
