#include "cell.h"

// Default Constructor
Cell::Cell() : status(WATER) {}

// Overloaded Constructor
Cell::Cell(CellStatus initialStatus) : status(initialStatus) {}

// Get Current Status
CellStatus Cell::getStatus() const {
    return status;
}

// Set Status
void Cell::setStatus(CellStatus newStatus) {
    status = newStatus;
}

// Get Display Character
char Cell::getDisplayChar() const {
    switch (status) {
        case WATER: return '~';
        case SHIP: return 'S';
        case HIT: return 'H';
        case MISS: return 'M';
        default: return '~';
    }
}

// Set Status Based on Display Character
void Cell::setStatusFromChar(char displayChar) {
    switch (displayChar) {
        case '~': status = WATER; break;
        case 'S': status = SHIP; break;
        case 'H': status = HIT; break;
        case 'M': status = MISS; break;
        default: status = WATER; break;
    }
}

// Operator Overload for Equality
bool Cell::operator==(const Cell& other) const {
    return status == other.status;
}
