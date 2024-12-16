#ifndef SHIP_H
#define SHIP_H

#include "common.h"
#include "gameentity.h"
#include <string>
#include <vector>
#include <iostream>

class Ship : public GameEntity {
private:
    std::string name;
    int size;
    Orientation orientation;
    int startRow;
    int startCol;
    std::vector<bool> hits;

public:
    // Constructors
    Ship();
    Ship(const std::string& name, int size);
    Ship(const Ship& other); // Copy Constructor

    // Getters
    std::string getName() const override;
    int getSize() const;
    Orientation getOrientation() const;
    int getStartRow() const;
    int getStartCol() const;
    std::vector<bool> getHits() const;

    // Setters
    void setPosition(int row, int col, Orientation orient);
    void setHits(const std::vector<bool>& newHits);

    // Member functions
    bool coversCell(int row, int col) const;
    bool hitCell(int row, int col);
    bool isSunk() const;

    // Display function (override)
    void display() const override;

    // Operator Overloading
    bool operator==(const Ship& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Ship& ship);
    friend std::istream& operator>>(std::istream& is, Ship& ship);
};

#endif // SHIP_H
