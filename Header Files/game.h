
#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "player.h"
#include "board.h"
#include "highscore.h"
#include "saveloadmanager.h"
#include "inputhandler.h"
#include "OutputHandler.h"      
#include "menu.h"
#include "scoremanager.h"
#include "logger.h"
#include "TestSuite.h"          
#include "gameexception.h"
#include <thread>
#include <mutex>
#include <memory>
// Game class
class Game {
private:
    Player player;
    Board board;
    ScoreManager scoreManager;
    SaveLoadManager saveLoadManager;
    InputHandler inputHandler;
    OutputHandler outputHandler;
    Menu menu;
    Logger logger;
    TestSuite testSuite;

    bool exitGame;
    std::mutex mtx; // For thread-safe operations
    std::thread autosaveThread;

    // Static member
    static int gameCount;

public:
    // Constructors
    Game();
    Game(const Game& other); // Copy Constructor

    // Destructor
    ~Game();

    // Assignment Operator
    Game& operator=(const Game& other);

    // Overloaded Operators
    bool operator==(const Game& other) const;

    // Member functions
    void run();
    void startNewGame();
    void loadGameState();
    void viewHighScores();
    void resetGame();
    void runTests();
    void play();
    void autoSave();

    // Friend classes/functions
    friend class SaveLoadManager;
};

#endif // GAME_H
