#include "game.h"
#include "board.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <ctime>

Game::Game() : exitGame(false) {
    board = new Board();
}

Game::~Game() {
    delete board;
}

void Game::displayInstructions() {
    std::cout << "========================================\n";
    std::cout << "          Welcome to Battleship!         \n";
    std::cout << "========================================\n";
    std::cout << "Instructions:\n";
    std::cout << "1. The game board is 10x10 (A-J rows, 0-9 columns).\n";
    std::cout << "2. Attack positions by entering coordinates like A5.\n";
    std::cout << "3. 'H' for hit, 'M' for miss.\n";
    std::cout << "4. Sink all ships to win.\n";
    std::cout << "5. Game auto-saves periodically.\n";
    std::cout << "========================================\n";
}

void Game::getPlayerNameInput() {
    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) {
        name = "Player";
    }
    playerName = name;
}

void Game::getAttackCoordinatesInput(int* row, int* col) {
    char input[10];
    std::cout << "Enter attack coordinates (e.g., A5): ";
    std::cin.getline(input, 10);

    if (strlen(input) < 2) {
        std::cout << "Invalid input. Please try again.\n";
        getAttackCoordinatesInput(row, col);
        return;
    }

    char rowChar = toupper(input[0]);
    if (rowChar < 'A' || rowChar >= 'A' + BOARD_SIZE) {
        std::cout << "Invalid row. Please try again.\n";
        getAttackCoordinatesInput(row, col);
        return;
    }
    *row = rowChar - 'A';

    char* endPtr;
    int column = strtol(&input[1], &endPtr, 10);
    if (*endPtr != '\0' || column < 0 || column >= BOARD_SIZE) {
        std::cout << "Invalid column. Please try again.\n";
        getAttackCoordinatesInput(row, col);
        return;
    }
    *col = column;
}

void Game::run() {
    displayInstructions();
    getPlayerNameInput();
    board->initializeGrid();
    board->placeAllShips();

    while (!exitGame) {
        int choice = getMenuChoice();
        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadGameState();
                break;
            case 3:
                viewHighScores();
                break;
            case 4:
                resetGame();
                break;
            case 5:
                runTests();
                break;
            case 6:
                std::cout << "\nExiting the game. Goodbye!\n";
                exitGame = true;
                break;
            default:
                std::cout << "\nInvalid choice. Please select a valid option.\n";
        }

        // If a game is started or loaded, proceed to gameplay
        if (!exitGame && choice >= 1 && choice <= 5) {
            play();
        }
    }
}

int Game::getMenuChoice() {
    std::cout << "\n========================================\n";
    std::cout << "         Battleship Main Menu           \n";
    std::cout << "========================================\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. View High Scores\n";
    std::cout << "4. Reset Game\n";
    std::cout << "5. Run Tests\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    return choice;
}

void Game::startNewGame() {
    resetGame();
    std::cout << "\nStarting a new game...\n";
}

void Game::loadGameState() {
    if (loadGame()) {
        std::cout << "\nGame loaded successfully. Welcome back, " << playerName << "!\n";
    } else {
        std::cout << "\nFailed to load game. Starting a new game.\n";
        resetGame();
    }
}

void Game::viewHighScores() {
    displayHighScores();
}

void Game::resetGame() {
    board->initializeGrid();
    board->placeAllShips();
    std::cout << "\nGame has been reset.\n";
}

void Game::runTests() {
    std::cout << "\nRunning tests (dummy)...\n";
    std::cout << "Tests completed.\n";
}

void Game::play() {
    bool gameOver = false;
    int moves = 0;

    while (!gameOver) {
        board->displayGrid(false);
        int row, col;
        getAttackCoordinatesInput(&row, &col);
        moves++;

        if (board->attack(row, col)) {
            std::cout << "Hit!\n";
            if (board->allShipsSunk()) {
                std::cout << "Congratulations! You have sunk all the ships in " << moves << " moves!\n";
                updateHighScores(moves);
                gameOver = true;
            }
        } else {
            std::cout << "Miss!\n";
        }

        saveGame();
    }
}

void Game::saveGame() {
    std::lock_guard<std::mutex> lock(mtx);

    std::ofstream outFile(SAVE_FILE, std::ios::binary);
    if (!outFile) {
        std::cout << "Error saving game.\n";
        return;
    }

    // Save player name
    size_t nameLength = playerName.size();
    outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName.c_str(), nameLength);

    int shipsRemaining = board->getShipsRemaining();
    outFile.write(reinterpret_cast<char*>(&shipsRemaining), sizeof(shipsRemaining));

    CellStatus tmpGrid[BOARD_SIZE][BOARD_SIZE];
    board->getGrid(tmpGrid);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        outFile.write(reinterpret_cast<char*>(&tmpGrid[i][0]), BOARD_SIZE * sizeof(CellStatus));
    }

    // Saving ship details (orientation, positions, hits) would be done here if needed
    // For brevity, not implemented here.

    outFile.close();
    std::cout << "Game saved successfully.\n";
}

bool Game::loadGame() {
    std::lock_guard<std::mutex> lock(mtx);

    std::ifstream inFile(SAVE_FILE, std::ios::binary);
    if (!inFile) {
        std::cout << "No saved game found.\n";
        return false;
    }

    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    if (nameLength >= MAX_NAME_LENGTH) {
        std::cout << "Saved player name is too long.\n";
        inFile.close();
        return false;
    }
    char tempName[MAX_NAME_LENGTH];
    inFile.read(tempName, nameLength);
    tempName[nameLength] = '\0';
    playerName = tempName;

    int shipsRemaining;
    inFile.read(reinterpret_cast<char*>(&shipsRemaining), sizeof(shipsRemaining));
    board->setShipsRemaining(shipsRemaining);

    CellStatus loadedGrid[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        inFile.read(reinterpret_cast<char*>(&loadedGrid[i][0]), BOARD_SIZE * sizeof(CellStatus));
    }
    board->loadGrid(loadedGrid);

    inFile.close();
    return true;
}

void Game::updateHighScores(int moves) {
    // Dummy implementation
    std::cout << "High scores updated (dummy implementation).\n";
}

void Game::displayHighScores() {
    // Dummy implementation
    std::cout << "No high scores available (dummy implementation).\n";
}

void Game::autoSaveThread() {
    while (!exitGame) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
        saveGame();
    }
}
