
#include "saveloadmanager.h"
#include "gameexception.h"
#include <fstream>
#include <iostream>

// Default Constructor
SaveLoadManager::SaveLoadManager()
    : saveFile("battleship_save.bin") {}

// Parameterized Constructor
SaveLoadManager::SaveLoadManager(const std::string& filename)
    : saveFile(filename) {}

// Save Game State
bool SaveLoadManager::saveGame(const Player& player, const Board& board, const std::string& playerName) {
    std::ofstream outFile(saveFile, std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cout << "Error saving game.\n";
        return false;
    }

    // Save player name
    size_t nameLength = playerName.size();
    outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName.c_str(), nameLength);

    // Save Board Grid
    auto grid = board.getGrid();
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            CellStatus status = cell.getStatus();
            outFile.write(reinterpret_cast<const char*>(&status), sizeof(status));
        }
    }

    // Save Ships
    const auto& ships = player.getShips();
    for (const auto& ship : ships) {
        // Save ship name length and name
        size_t shipNameLength = ship.getName().size();
        outFile.write(reinterpret_cast<const char*>(&shipNameLength), sizeof(shipNameLength));
        outFile.write(ship.getName().c_str(), shipNameLength);

        // Save ship size
        int shipSize = ship.getSize();
        outFile.write(reinterpret_cast<const char*>(&shipSize), sizeof(shipSize));

        // Save ship orientation
        Orientation orientation = ship.getOrientation();
        outFile.write(reinterpret_cast<const char*>(&orientation), sizeof(orientation));

        // Save ship position
        int startRow = ship.getStartRow();
        int startCol = ship.getStartCol();
        outFile.write(reinterpret_cast<const char*>(&startRow), sizeof(startRow));
        outFile.write(reinterpret_cast<const char*>(&startCol), sizeof(startCol));

        // Save ship hits
        std::vector<bool> hits = ship.getHits();
        size_t hitsSize = hits.size();
        outFile.write(reinterpret_cast<const char*>(&hitsSize), sizeof(hitsSize));
        for (bool hit : hits) {
            char hitChar = hit ? 1 : 0;
            outFile.write(&hitChar, sizeof(hitChar));
        }
    }

    outFile.close();
    std::cout << "Game saved successfully.\n";
    return true;
}

// Load Game State
bool SaveLoadManager::loadGame(Player& player, Board& board, std::string& playerName) {
    std::ifstream inFile(saveFile, std::ios::binary);
    if (!inFile) {
        std::cout << "No saved game found.\n";
        return false;
    }

    try {
        // Load player name
        size_t nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        if (nameLength > 100) // Arbitrary limit to prevent excessive memory allocation
            throw GameException("Saved player name is too long.");

        playerName.resize(nameLength);
        inFile.read(&playerName[0], nameLength);
        player.setName(playerName);

        // Load Board Grid
        std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> loadedGrid;
        for (auto& row : loadedGrid) {
            for (auto& cell : row) {
                CellStatus status;
                inFile.read(reinterpret_cast<char*>(&status), sizeof(status));
                cell.setStatus(status);
            }
        }
        board.loadGrid(loadedGrid);

        // Load Ships
        auto& ships = player.getShips();
        for (auto& ship : ships) {
            // Load ship name length and name
            size_t shipNameLength;
            inFile.read(reinterpret_cast<char*>(&shipNameLength), sizeof(shipNameLength));
            if (shipNameLength > 100)
                throw GameException("Saved ship name is too long.");
            std::string shipName(shipNameLength, ' ');
            inFile.read(&shipName[0], shipNameLength);

            // Load ship size
            int shipSize;
            inFile.read(reinterpret_cast<char*>(&shipSize), sizeof(shipSize));

            // Load ship orientation
            Orientation orientation;
            inFile.read(reinterpret_cast<char*>(&orientation), sizeof(orientation));

            // Load ship position
            int startRow, startCol;
            inFile.read(reinterpret_cast<char*>(&startRow), sizeof(startRow));
            inFile.read(reinterpret_cast<char*>(&startCol), sizeof(startCol));

            // Load ship hits
            size_t hitsSize;
            inFile.read(reinterpret_cast<char*>(&hitsSize), sizeof(hitsSize));
            std::vector<bool> hits(hitsSize, false);
            for (size_t i = 0; i < hitsSize; ++i) {
                char hitChar;
                inFile.read(&hitChar, sizeof(hitChar));
                hits[i] = (hitChar == 1);
            }

            // Reconstruct Ship
            Ship loadedShip(shipName, shipSize);
            loadedShip.setPosition(startRow, startCol, orientation);
            loadedShip.setHits(hits);

            ship = loadedShip;
        }

        inFile.close();
        std::cout << "Game loaded successfully.\n";
        return true;
    } catch (const GameException& e) {
        std::cout << "Error loading game: " << e.what() << "\n";
        inFile.close();
        return false;
    } catch (...) {
        std::cout << "Unknown error occurred while loading the game.\n";
        inFile.close();
        return false;
    }
}
