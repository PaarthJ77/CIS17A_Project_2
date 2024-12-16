#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

static const std::string SHIP_NAMES[MAX_SHIPS] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
static const int SHIP_SIZES[MAX_SHIPS] = {5, 4, 3, 3, 2};

Board::Board() {
    initializeGrid();
    shipsRemaining = MAX_SHIPS;
    ships.reserve(MAX_SHIPS);
    for (int i = 0; i < MAX_SHIPS; ++i) {
        ships.push_back(Ship(SHIP_NAMES[i], SHIP_SIZES[i]));
    }
}

void Board::initializeGrid() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            grid[i][j] = WATER;
        }
    }
    shipsRemaining = MAX_SHIPS;
}

void Board::placeAllShips() {
    srand((unsigned int)time(0));
    for (int i = 0; i < MAX_SHIPS; ++i) {
        bool placed = false;
        while (!placed) {
            int row = getRandomNumber(0, BOARD_SIZE - 1);
            int col = getRandomNumber(0, BOARD_SIZE - 1);
            Orientation orientation = (rand() % 2 == 0) ? HORIZONTAL : VERTICAL;
            if (placeShip(i, row, col, orientation)) {
                placed = true;
            }
        }
    }
}

bool Board::placeShip(int shipIndex, int row, int col, Orientation orientation) {
    if (shipIndex < 0 || shipIndex >= (int)ships.size()) return false;
    Ship& ship = ships[shipIndex];
    ship.setPosition(row, col, orientation);

    // Validate placement
    if (!isValidPlacement(ship, row, col, orientation)) {
        return false;
    }

    // Mark grid cells
    for (int i = 0; i < ship.getSize(); ++i) {
        int r = row + ((orientation == VERTICAL) ? i : 0);
        int c = col + ((orientation == HORIZONTAL) ? i : 0);
        grid[r][c] = SHIP;
    }
    return true;
}

bool Board::isValidPlacement(const Ship& ship, int row, int col, Orientation orientation) {
    int size = ship.getSize();
    if (orientation == HORIZONTAL) {
        if (col + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i) {
            if (grid[row][col + i] != WATER)
                return false;
        }
    } else {
        if (row + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i) {
            if (grid[row + i][col] != WATER)
                return false;
        }
    }
    return true;
}

int Board::getRandomNumber(int min, int max) const {
    return rand() % (max - min + 1) + min;
}

bool Board::attack(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        std::cout << "Invalid coordinates. Try again.\n";
        return false;
    }

    if (grid[row][col] == HIT || grid[row][col] == MISS) {
        std::cout << "You have already attacked this position. Try again.\n";
        return false;
    }

    if (grid[row][col] == SHIP) {
        grid[row][col] = HIT;
        // Find which ship
        for (auto &ship : ships) {
            if (ship.coversCell(row, col)) {
                ship.hitCell(row, col);
                if (ship.isSunk()) {
                    std::cout << "You sunk the " << ship.getName() << "!\n";
                    shipsRemaining--;
                }
                return true;
            }
        }
    } else if (grid[row][col] == WATER) {
        grid[row][col] = MISS;
        return false;
    }

    return false;
}

void Board::displayGrid(bool showShips) const {
    std::cout << "\n    ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << i << " ";
    }
    std::cout << "\n   ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << "--";
    }
    std::cout << "\n";

    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << (char)('A' + i) << " | ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            CellStatus cell = grid[i][j];
            if (cell == SHIP && !showShips)
                std::cout << "~ ";
            else if (cell == HIT)
                std::cout << "H ";
            else if (cell == MISS)
                std::cout << "M ";
            else
                std::cout << "~ ";
        }
        std::cout << "\n";
    }
}

void Board::displayHitsOnly() const {
    // Implementation would hide all undamaged ship parts
    // For brevity, not fully implemented here.
}

void Board::loadGrid(const CellStatus loadedGrid[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            grid[i][j] = loadedGrid[i][j];
        }
    }
}

void Board::getGrid(CellStatus outGrid[BOARD_SIZE][BOARD_SIZE]) const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            outGrid[i][j] = grid[i][j];
        }
    }
}
