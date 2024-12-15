#ifndef GAME_H
#define GAME_H

#include <string>

// Enumerations
enum Orientation { HORIZONTAL, VERTICAL };
enum CellStatus { WATER, SHIP, HIT, MISS };

// Constants
const int BOARD_SIZE = 10;
const int MAX_SHIPS = 5;
const int MAX_NAME_LENGTH = 50;
const std::string SAVE_FILE = "battleship_save.bin";
const std::string HIGH_SCORE_FILE = "highscores.bin";

class Board; // Forward declaration

#include <mutex>

class Game {
private:
    std::string playerName;
    Board* board;
    bool exitGame;
    std::mutex mtx; // Mutex for thread-safe operations

public:
    Game();
    ~Game();

    void run();
    void startNewGame();
    void loadGameState();
    void viewHighScores();
    void resetGame();
    void runTests();

    void displayInstructions();
    void getPlayerNameInput();
    void getAttackCoordinatesInput(int* row, int* col);

    void saveGame();
    bool loadGame();

    void updateHighScores(int moves);
    void displayHighScores();

private:
    void play();
    int getMenuChoice();
    void autoSaveThread();
};

#endif 