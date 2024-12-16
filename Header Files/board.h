#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "cell.h"
#include "ship.h"
#include "gameexception.h"
#include <array>
#include <vector>
#include <mutex>
#include <iostream>

class Board {
private:
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> grid;
    std::vector<Ship> ships;
    int shipsRemaining;
    std::mutex mtx;

public:
    // Constructors
    Board();
    Board(const Board& other); 

    // Destructor
    ~Board();

    // Assignment Operator
    Board& operator=(const Board& other);

    // Overloaded Operators
    bool operator==(const Board& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    friend std::istream& operator>>(std::istream& is, Board& board);

    // Member functions
    void initializeGrid();
    void placeAllShips();
    bool placeShip(int shipIndex, int row, int col, Orientation orientation);
    bool attack(int row, int col);
    bool allShipsSunk() const;
    void displayGrid(bool showShips) const;
    void displayHitsOnly() const;

    // Grid access functions
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> getGrid() const;
    void loadGrid(const std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE>& loadedGrid);

    // Getters
    int getShipsRemaining() const;
};

#endif // BOARD_H
