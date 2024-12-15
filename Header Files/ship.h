#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <vector>
#include "game.h" 

class Ship {
private:
    std::string name;
    int size;
    Orientation orientation;
    int startRow;
    int startCol;
    std::vector<bool> hits;

public:
    Ship(const std::string& name, int size);
    
    void setPosition(int row, int col, Orientation orientation);
    bool coversCell(int row, int col) const;
    bool hitCell(int row, int col);
    bool isSunk() const;

    const std::string& getName() const { return name; }
    int getSize() const { return size; }
    Orientation getOrientation() const { return orientation; }
    int getStartRow() const { return startRow; }
    int getStartCol() const { return startCol; }
};

#endif 
