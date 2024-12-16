#include "game.h"
#include <iostream>
#include <chrono>
#include <stdexcept>

// Initialize static member
int Game::gameCount = 0;

// Default Constructor
Game::Game()
    : player(), board(), scoreManager(),
      saveLoadManager(), inputHandler(),
      outputHandler(), menu(), logger(),
      testSuite(), exitGame(false), mtx() {
    gameCount++;
    logger.log("Game instance created.");
}

// Copy Constructor
Game::Game(const Game& other)
    : player(other.player), board(other.board),
      scoreManager(other.scoreManager),
      saveLoadManager(other.saveLoadManager),
      inputHandler(other.inputHandler),
      outputHandler(other.outputHandler),
      menu(other.menu), logger(other.logger),
      testSuite(other.testSuite),
      exitGame(other.exitGame), mtx() {
    gameCount++;
    logger.log("Game instance copied.");
}

// Destructor
Game::~Game() {
    exitGame = true;
    gameCount--;
    logger.log("Game instance destroyed.");
}

// Assignment Operator
Game& Game::operator=(const Game& other) {
    if (this != &other) {
        player = other.player;
        board = other.board;
        scoreManager = other.scoreManager;
        saveLoadManager = other.saveLoadManager;
        inputHandler = other.inputHandler;
        outputHandler = other.outputHandler;
        menu = other.menu;
        logger = other.logger;
        testSuite = other.testSuite;
        exitGame = other.exitGame;
    }
    return *this;
}

// Load Game State
void Game::loadGameState() {
    std::string playerName = player.getName();
    if (saveLoadManager.loadGame(player, board, playerName)) {
        std::cout << "\nGame loaded successfully. Welcome back, " << playerName << "!\n";
        logger.log("Game loaded.");
    } else {
        std::cout << "\nFailed to load game. Starting a new game.\n";
        startNewGame();
    }
}

// Start a New Game
void Game::startNewGame() {
    board.initializeGrid();
    player = Player();
    scoreManager = ScoreManager();
    std::cout << "New game started. Good luck, " << player.getName() << "!\n";
    logger.log("New game started.");
}

// Run the Game Loop
void Game::run() {
    outputHandler.displayInstructions();
    int choice = menu.displayMainMenu();

    switch (choice) {
        case 1:
            startNewGame();
            play();
            break;
        case 2:
            loadGameState();
            play();
            break;
        case 3:
            scoreManager.displayHighScores();
            break;
        case 4:
            resetGame();
            break;
        case 5:
            runTests();
            break;
        case 6:
            exitGame = true;
            std::cout << "Exiting the game. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Exiting the game.\n";
            exitGame = true;
            break;
    }
}

// Play the Game
void Game::play() {
    bool gameOver = false;
    int moves = 0;
    auto lastSaveTime = std::chrono::steady_clock::now();

    while (!gameOver && !exitGame) {
        board.displayGrid(false);

        std::pair<int, int> attackCoords;
        try {
            attackCoords = inputHandler.getAttackCoordinates();
        } catch (const std::exception& e) {
            std::cout << "Error reading attack coordinates: " << e.what() << "\n";
            continue;
        }

        int row = attackCoords.first;
        int col = attackCoords.second;
        moves++;

        try {
            if (board.attack(row, col)) {
                std::cout << "Hit!\n";
                logger.log("Player " + player.getName() + " hit at (" + std::to_string(row) + ", " + std::to_string(col) + ").");
                if (board.allShipsSunk()) {
                    std::cout << "Congratulations! You have sunk all the ships in " << moves << " moves!\n";
                    scoreManager.updateHighScores(player.getName(), moves);
                    logger.log("Player " + player.getName() + " won the game.");
                    gameOver = true;
                }
            } else {
                std::cout << "Miss!\n";
                logger.log("Player " + player.getName() + " missed at (" + std::to_string(row) + ", " + std::to_string(col) + ").");
            }
        } catch (const GameException& e) {
            std::cout << "Error during attack: " << e.what() << "\n";
            logger.log("Error during attack: " + std::string(e.what()));
            moves--; // Do not count invalid attacks
            continue;
        }

        // Periodic autosave every 60 seconds
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastSaveTime).count() >= 60) {
            saveLoadManager.saveGame(player, board, player.getName());
            lastSaveTime = currentTime;
            std::cout << "Game autosaved.\n";
            logger.log("Game autosaved.");
        }
    }
}

// Run Tests
void Game::runTests() {
    std::cout << "Running all tests...\n";
    testSuite.runAllTests();
    logger.log("All tests run.");
}

// Reset Game
void Game::resetGame() {
    board.initializeGrid();
    player = Player();
    scoreManager = ScoreManager();
    std::cout << "Game has been reset.\n";
    logger.log("Game reset.");
}
