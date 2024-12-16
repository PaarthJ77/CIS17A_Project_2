#include "ship.h"

Ship::Ship(const std::string& name, int size) : name(name), size(size) {
    hits.resize(size, false);
}

void Ship::setPosition(int row, int col, Orientation orientation) {
    this->startRow = row;
    this->startCol = col;
    this->orientation = orientation;
}

bool Ship::coversCell(int row, int col) const {
    for (int i = 0; i < size; ++i) {
        int r = startRow + ((orientation == VERTICAL) ? i : 0);
        int c = startCol + ((orientation == HORIZONTAL) ? i : 0);
        if (r == row && c == col) {
            return true;
        }
    }
    return false;
}

bool Ship::hitCell(int row, int col) {
    for (int i = 0; i < size; ++i) {
        int r = startRow + ((orientation == VERTICAL) ? i : 0);
        int c = startCol + ((orientation == HORIZONTAL) ? i : 0);
        if (r == row && c == col) {
            hits[i] = true;
            return true;
        }
    }
    return false;
}

bool Ship::isSunk() const {
    for (bool h : hits) {
        if (!h) return false;
    }
    return true;
}
