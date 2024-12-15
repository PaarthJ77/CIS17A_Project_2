#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <mutex>
#include "game.h"
#include "ship.h"

class Board {
private:
    CellStatus grid[BOARD_SIZE][BOARD_SIZE];
    std::vector<Ship> ships;
    int shipsRemaining;
    std::mutex mtx;

public:
    Board();
    void initializeGrid();
    void placeAllShips();
    bool placeShip(int shipIndex, int row, int col, Orientation orientation);

    bool attack(int row, int col);
    bool allShipsSunk() const { return shipsRemaining == 0; }

    void displayGrid(bool showShips) const;
    void displayHitsOnly() const; // Not fully implemented here

    const std::vector<Ship>& getShips() const { return ships; }
    void setShipsRemaining(int remaining) { shipsRemaining = remaining; }
    int getShipsRemaining() const { return shipsRemaining; }

    void loadGrid(const CellStatus loadedGrid[BOARD_SIZE][BOARD_SIZE]);
    void getGrid(CellStatus outGrid[BOARD_SIZE][BOARD_SIZE]) const;

private:
    bool isValidPlacement(const Ship& ship, int row, int col, Orientation orientation);
    int getRandomNumber(int min, int max) const;
};

#endif 
