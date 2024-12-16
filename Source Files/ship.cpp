#include "ship.h"
#include <iostream>

// Default Constructor
Ship::Ship() : name("Unnamed"), size(0), orientation(HORIZONTAL), startRow(0), startCol(0), hits() {}

// Parameterized Constructor
Ship::Ship(const std::string& name, int size)
    : name(name), size(size), orientation(HORIZONTAL), startRow(0), startCol(0), hits(size, false) {}

// Copy Constructor
Ship::Ship(const Ship& other)
    : name(other.name), size(other.size), orientation(other.orientation),
      startRow(other.startRow), startCol(other.startCol), hits(other.hits) {}

// Getters
std::string Ship::getName() const {
    return name;
}

int Ship::getSize() const {
    return size;
}

Orientation Ship::getOrientation() const {
    return orientation;
}

int Ship::getStartRow() const {
    return startRow;
}

int Ship::getStartCol() const {
    return startCol;
}

std::vector<bool> Ship::getHits() const {
    return hits;
}

// Set Position
void Ship::setPosition(int row, int col, Orientation orient) {
    startRow = row;
    startCol = col;
    orientation = orient;
}

// Set Hits
void Ship::setHits(const std::vector<bool>& newHits) {
    hits = newHits;
}

// Check if the ship covers a specific cell
bool Ship::coversCell(int row, int col) const {
    for (int i = 0; i < size; ++i) {
        int r = startRow + (orientation == VERTICAL ? i : 0);
        int c = startCol + (orientation == HORIZONTAL ? i : 0);
        if (r == row && c == col) {
            return true;
        }
    }
    return false;
}

// Register a hit on a specific cell
bool Ship::hitCell(int row, int col) {
    for (int i = 0; i < size; ++i) {
        int r = startRow + (orientation == VERTICAL ? i : 0);
        int c = startCol + (orientation == HORIZONTAL ? i : 0);
        if (r == row && c == col) {
            hits[i] = true;
            return true;
        }
    }
    return false;
}

// Check if the ship is sunk
bool Ship::isSunk() const {
    for (bool hit : hits) {
        if (!hit) {
            return false;
        }
    }
    return true;
}

// Display function (override)
void Ship::display() const {
    std::cout << name << " (Size: " << size << ")";
    std::cout << " Orientation: " << (orientation == HORIZONTAL ? "Horizontal" : "Vertical");
}

// Overloaded Equality Operator
bool Ship::operator==(const Ship& other) const {
    return name == other.name &&
           size == other.size &&
           orientation == other.orientation &&
           startRow == other.startRow &&
           startCol == other.startCol &&
           hits == other.hits;
}

// Overloaded Output Operator
std::ostream& operator<<(std::ostream& os, const Ship& ship) {
    os << ship.name << " (Size: " << ship.size << ")";
    os << " Orientation: " << (ship.orientation == HORIZONTAL ? "Horizontal" : "Vertical");
    return os;
}

