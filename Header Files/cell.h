#ifndef CELL_H
#define CELL_H

#include "common.h"

class Cell {
private:
    CellStatus status;

public:
    // Constructors
    Cell();
    Cell(CellStatus initialStatus);

    // Getters and Setters
    CellStatus getStatus() const;
    void setStatus(CellStatus newStatus);

    // Display Methods
    char getDisplayChar() const;
    void setStatusFromChar(char displayChar);

    // Operator Overloads
    bool operator==(const Cell& other) const;
};

#endif // CELL_H
