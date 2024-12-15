#ifndef GAME_H
#define GAME_H

#include <string>
#include <mutex>

// =====================
// Enumerations
// =====================

// Enumeration for ship orientation
enum Orientation { HORIZONTAL, VERTICAL };

// Enumeration for cell status
enum CellStatus { WATER, SHIP, HIT, MISS };

// =====================
// Constants
// =====================

const int BOARD_SIZE = 10;
const int MAX_SHIPS = 5;
const int MAX_NAME_LENGTH = 50;
const std::string SAVE_FILE = "battleship_save.bin";
const std::string HIGH_SCORE_FILE = "highscores.bin";

// =====================
// External Global Variables
// =====================

// Ship Information
extern const std::string shipNames[MAX_SHIPS];
extern const int shipSizes[MAX_SHIPS];

// Ship Properties Arrays
extern Orientation shipOrientations[MAX_SHIPS];
extern int shipStartRows[MAX_SHIPS];
extern int shipStartCols[MAX_SHIPS];
extern bool** shipHits;

// Game Board
extern CellStatus grid[BOARD_SIZE][BOARD_SIZE];

// Player Name
extern char playerName[MAX_NAME_LENGTH];

// Ships Placed Counter
extern int shipCount;

// Ships Remaining
extern int shipsRemaining;

// Mutex for thread synchronization
extern std::mutex mtx;

// =====================
// Function Prototypes
// =====================

// Initialization functions
void initializeGrid();
void allocateShipHits();
void deallocateShipHits();
void placeAllShips();

// Ship placement functions
bool placeShip(int shipIndex, int row, int col, Orientation orientation);

// Display functions
void displayGrid(bool showShips);

// Gameplay functions
bool attack(int row, int col);
bool allShipsSunk();

// Utility functions
int getRandomNumber(int min, int max);
void saveGame();
bool loadGame();

// High Score functions
void displayHighScores();
void updateHighScores(int moves);

// Input functions
void getPlayerNameInput();
void getAttackCoordinatesInput(int* row, int* col);

// Helper functions
bool isValidPlacement(int shipIndex, int row, int col, Orientation orientation);
void markShipHit(int shipIndex, int hitIndex);
void displayInstructions();

// Abstract Data Type Functions
void resetGame();
void displayAllShips();
void displayHitsOnly();

// Testing Framework Functions
void runTests();
bool testPlacement();
bool testAttack();
bool testFileOperations();

// Input/Output Simultaneous Functions
void autoSaveThread();

#endif // GAME_H
