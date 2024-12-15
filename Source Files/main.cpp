#include "game.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>

// =====================
// Global Variable Definitions
// =====================

// Ship Information
const std::string shipNames[MAX_SHIPS] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
const int shipSizes[MAX_SHIPS] = {5, 4, 3, 3, 2};

// Ship Properties Arrays
Orientation shipOrientations[MAX_SHIPS];
int shipStartRows[MAX_SHIPS];
int shipStartCols[MAX_SHIPS];
bool** shipHits = nullptr;

// Game Board
CellStatus grid[BOARD_SIZE][BOARD_SIZE];

// Player Name
char playerName[MAX_NAME_LENGTH];

// Ships Placed Counter
int shipCount = 0;

// Ships Remaining
int shipsRemaining = MAX_SHIPS;

// Mutex for thread synchronization
std::mutex mtx;

// =====================
// Function Definitions
// =====================

// Initializes the game grid by filling it with WATER
void initializeGrid() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            grid[i][j] = WATER;
        }
    }
    shipCount = 0;
    shipsRemaining = MAX_SHIPS;
}

// Allocates memory for ship hits tracking
void allocateShipHits() {
    shipHits = new bool*[MAX_SHIPS];
    for (int i = 0; i < MAX_SHIPS; ++i) {
        shipHits[i] = new bool[shipSizes[i]];
        for (int j = 0; j < shipSizes[i]; ++j) {
            shipHits[i][j] = false;
        }
    }
}

// Deallocates memory for ship hits tracking
void deallocateShipHits() {
    if (shipHits != nullptr) {
        for (int i = 0; i < MAX_SHIPS; ++i) {
            delete[] shipHits[i];
        }
        delete[] shipHits;
        shipHits = nullptr;
    }
}

// Places all ships on the grid randomly
void placeAllShips() {
    for (int i = 0; i < MAX_SHIPS; ++i) {
        bool placed = false;
        while (!placed) {
            int row = getRandomNumber(0, BOARD_SIZE - 1);
            int col = getRandomNumber(0, BOARD_SIZE - 1);
            Orientation orientation = (rand() % 2 == 0) ? HORIZONTAL : VERTICAL;
            if (placeShip(i, row, col, orientation)) {
                placed = true;
                shipOrientations[i] = orientation;
                shipStartRows[i] = row;
                shipStartCols[i] = col;
                shipCount++;
            }
        }
    }
}

// Attempts to place a single ship on the grid
bool placeShip(int shipIndex, int row, int col, Orientation orientation) {
    // Check if placement is valid
    if (!isValidPlacement(shipIndex, row, col, orientation)) {
        return false;
    }

    // Place ship on grid
    for (int i = 0; i < shipSizes[shipIndex]; ++i) {
        if (orientation == HORIZONTAL) {
            grid[row][col + i] = SHIP;
        } else { // VERTICAL
            grid[row + i][col] = SHIP;
        }
    }

    return true;
}

// Checks if the ship placement is valid (within bounds and no overlap)
bool isValidPlacement(int shipIndex, int row, int col, Orientation orientation) {
    if (orientation == HORIZONTAL) {
        if (col + shipSizes[shipIndex] > BOARD_SIZE)
            return false;
        for (int i = 0; i < shipSizes[shipIndex]; ++i) {
            if (grid[row][col + i] != WATER)
                return false;
        }
    } else { // VERTICAL
        if (row + shipSizes[shipIndex] > BOARD_SIZE)
            return false;
        for (int i = 0; i < shipSizes[shipIndex]; ++i) {
            if (grid[row + i][col] != WATER)
                return false;
        }
    }
    return true;
}

// Displays the game grid. If showShips is true, ships are visible; otherwise, they are hidden
void displayGrid(bool showShips) {
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
        std::cout << static_cast<char>('A' + i) << " | ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] == SHIP && !showShips)
                std::cout << "~ ";
            else if (grid[i][j] == HIT)
                std::cout << "H ";
            else if (grid[i][j] == MISS)
                std::cout << "M ";
            else
                std::cout << "~ ";
        }
        std::cout << "\n";
    }
}

// Processes an attack at the specified coordinates. Returns true if hit, false if miss
bool attack(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        std::cout << "Invalid coordinates. Try again.\n";
        return false;
    }

    if (grid[row][col] == SHIP) {
        grid[row][col] = HIT; // Hit
        // Determine which ship was hit
        for (int i = 0; i < MAX_SHIPS; ++i) {
            int shipRow = shipStartRows[i];
            int shipCol = shipStartCols[i];
            for (int j = 0; j < shipSizes[i]; ++j) {
                if (shipOrientations[i] == HORIZONTAL) {
                    if (shipRow == row && shipCol + j == col) {
                        shipHits[i][j] = true;
                        // Check if ship is sunk
                        bool sunk = true;
                        for (int k = 0; k < shipSizes[i]; ++k) {
                            if (!shipHits[i][k]) {
                                sunk = false;
                                break;
                            }
                        }
                        if (sunk) {
                            std::cout << "You sunk the " << shipNames[i] << "!\n";
                            shipsRemaining--;
                        }
                        return true;
                    }
                } else { // VERTICAL
                    if (shipRow + j == row && shipCol == col) {
                        shipHits[i][j] = true;
                        // Check if ship is sunk
                        bool sunk = true;
                        for (int k = 0; k < shipSizes[i]; ++k) {
                            if (!shipHits[i][k]) {
                                sunk = false;
                                break;
                            }
                        }
                        if (sunk) {
                            std::cout << "You sunk the " << shipNames[i] << "!\n";
                            shipsRemaining--;
                        }
                        return true;
                    }
                }
            }
        }
    } else if (grid[row][col] == WATER) {
        grid[row][col] = MISS; // Miss
        return false;
    } else {
        std::cout << "You have already attacked this position. Try again.\n";
        return false;
    }

    return false;
}

// Checks if all ships have been sunk
bool allShipsSunk() {
    return shipsRemaining == 0;
}

// Generates a random number between min and max (inclusive)
int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Saves the current game state to a binary file
void saveGame() {
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety

    std::ofstream outFile(SAVE_FILE, std::ios::binary);
    if (!outFile) {
        std::cout << "Error saving game.\n";
        return;
    }

    // Save player name
    size_t nameLength = strlen(playerName);
    outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName, nameLength);

    // Save ships remaining
    outFile.write(reinterpret_cast<char*>(&shipsRemaining), sizeof(shipsRemaining));

    // Save grid
    for (int i = 0; i < BOARD_SIZE; ++i) {
        outFile.write(reinterpret_cast<char*>(&grid[i][0]), BOARD_SIZE * sizeof(CellStatus));
    }

    // Save ship orientations
    outFile.write(reinterpret_cast<char*>(shipOrientations), MAX_SHIPS * sizeof(Orientation));

    // Save ship starting rows and columns
    outFile.write(reinterpret_cast<char*>(shipStartRows), MAX_SHIPS * sizeof(int));
    outFile.write(reinterpret_cast<char*>(shipStartCols), MAX_SHIPS * sizeof(int));

    // Save ship hits
    for (int i = 0; i < MAX_SHIPS; ++i) {
        outFile.write(reinterpret_cast<char*>(shipHits[i]), shipSizes[i] * sizeof(bool));
    }

    outFile.close();
    std::cout << "Game saved successfully.\n";
}

// Loads the game state from a binary file. Returns true if successful, false otherwise
bool loadGame() {
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety

    std::ifstream inFile(SAVE_FILE, std::ios::binary);
    if (!inFile) {
        std::cout << "No saved game found.\n";
        return false;
    }

    // Load player name
    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    if (nameLength >= MAX_NAME_LENGTH) {
        std::cout << "Saved player name is too long.\n";
        inFile.close();
        return false;
    }
    inFile.read(playerName, nameLength);
    playerName[nameLength] = '\0';

    // Load ships remaining
    inFile.read(reinterpret_cast<char*>(&shipsRemaining), sizeof(shipsRemaining));

    // Load grid
    for (int i = 0; i < BOARD_SIZE; ++i) {
        inFile.read(reinterpret_cast<char*>(&grid[i][0]), BOARD_SIZE * sizeof(CellStatus));
    }

    // Load ship orientations
    inFile.read(reinterpret_cast<char*>(shipOrientations), MAX_SHIPS * sizeof(Orientation));

    // Load ship starting rows and columns
    inFile.read(reinterpret_cast<char*>(shipStartRows), MAX_SHIPS * sizeof(int));
    inFile.read(reinterpret_cast<char*>(shipStartCols), MAX_SHIPS * sizeof(int));

    // Load ship hits
    for (int i = 0; i < MAX_SHIPS; ++i) {
        inFile.read(reinterpret_cast<char*>(shipHits[i]), shipSizes[i] * sizeof(bool));
    }

    inFile.close();
    return true;
}

// Displays high scores from a binary file
void displayHighScores() {
    std::ifstream inFile(HIGH_SCORE_FILE, std::ios::binary);
    if (!inFile) {
        std::cout << "\nNo high scores available.\n";
        return;
    }

    int numScores;
    inFile.read(reinterpret_cast<char*>(&numScores), sizeof(numScores));

    if (numScores == 0) {
        std::cout << "\nNo high scores available.\n";
        inFile.close();
        return;
    }

    std::cout << "\n===== High Scores =====\n";
    for (int i = 0; i < numScores; ++i) {
        size_t nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        char* nameBuffer = new char[nameLength + 1];
        inFile.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0';
        std::string name = std::string(nameBuffer);
        delete[] nameBuffer;

        int moves;
        inFile.read(reinterpret_cast<char*>(&moves), sizeof(moves));

        std::cout << i + 1 << ". " << name << " - " << moves << " moves\n";
    }
    std::cout << "=======================\n";

    inFile.close();
}

// Updates the high scores by adding the new score and keeping the top 5
void updateHighScores(int moves) {
    // Read existing high scores
    std::ifstream inFile(HIGH_SCORE_FILE, std::ios::binary);
    int numScores = 0;
    std::string* names = nullptr;
    int* scores = nullptr;

    if (inFile) {
        inFile.read(reinterpret_cast<char*>(&numScores), sizeof(numScores));
        if (numScores > 0) {
            names = new std::string[numScores];
            scores = new int[numScores];
            for (int i = 0; i < numScores; ++i) {
                size_t nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                char* nameBuffer = new char[nameLength + 1];
                inFile.read(nameBuffer, nameLength);
                nameBuffer[nameLength] = '\0';
                names[i] = std::string(nameBuffer);
                delete[] nameBuffer;

                inFile.read(reinterpret_cast<char*>(&scores[i]), sizeof(scores[i]));
            }
        }
        inFile.close();
    }

    // Add the new score
    std::string newName = std::string(playerName);
    int newScore = moves;

    // Create temporary arrays to hold all scores
    std::string* tempNames = new std::string[numScores + 1];
    int* tempScores = new int[numScores + 1];

    for (int i = 0; i < numScores; ++i) {
        tempNames[i] = names[i];
        tempScores[i] = scores[i];
    }
    tempNames[numScores] = newName;
    tempScores[numScores] = newScore;
    numScores++;

    // Sort the scores (ascending order: fewer moves is better)
    for (int i = 0; i < numScores - 1; ++i) {
        for (int j = i + 1; j < numScores; ++j) {
            if (tempScores[j] < tempScores[i]) {
                // Swap scores
                int temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;

                // Swap names
                std::string tempStr = tempNames[i];
                tempNames[i] = tempNames[j];
                tempNames[j] = tempStr;
            }
        }
    }

    // Keep only top 5 scores
    if (numScores > 5) {
        numScores = 5;
    }

    // Write back the high scores
    std::ofstream outFile(HIGH_SCORE_FILE, std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cout << "Error updating high scores.\n";
        return;
    }

    outFile.write(reinterpret_cast<char*>(&numScores), sizeof(numScores));
    for (int i = 0; i < numScores; ++i) {
        size_t nameLength = tempNames[i].size();
        outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        outFile.write(tempNames[i].c_str(), nameLength);
        outFile.write(reinterpret_cast<char*>(&tempScores[i]), sizeof(tempScores[i]));
    }

    outFile.close();
    std::cout << "High scores updated!\n";

    // Clean up temporary arrays
    delete[] tempNames;
    delete[] tempScores;
    if (names != nullptr) {
        delete[] names;
        delete[] scores;
    }
}

// Captures the player's name using a character array
void getPlayerNameInput() {
    std::cout << "Enter your name: ";
    std::cin.getline(playerName, MAX_NAME_LENGTH);
    if (strlen(playerName) == 0) {
        strcpy(playerName, "Player");
    }
}

// Captures and parses the attack coordinates from the player
void getAttackCoordinatesInput(int* row, int* col) {
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

// Displays game instructions
void displayInstructions() {
    std::cout << "========================================\n";
    std::cout << "          Welcome to Battleship!         \n";
    std::cout << "========================================\n";
    std::cout << "Instructions:\n";
    std::cout << "1. The game board is a 10x10 grid labeled A-J for rows and 0-9 for columns.\n";
    std::cout << "2. You will attack positions by entering coordinates like A5.\n";
    std::cout << "3. A hit is marked with 'H' and a miss with 'M'.\n";
    std::cout << "4. Your goal is to sink all ships by hitting all their parts.\n";
    std::cout << "5. The game state is saved automatically after each attack and every 60 seconds.\n";
    std::cout << "========================================\n";
}

// Resets the game to start a new game
void resetGame() {
    initializeGrid();
    deallocateShipHits();
    allocateShipHits();
    placeAllShips();
    std::cout << "\nGame has been reset. All ships have been placed randomly.\n";
}

// Displays all ships on the grid (for debugging or optional viewing)
void displayAllShips() {
    displayGrid(true);
}

// Displays only the hits and misses on the grid
void displayHitsOnly() {
    CellStatus displayGridCopy[BOARD_SIZE][BOARD_SIZE];
    // Copy grid to temporary array
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            displayGridCopy[i][j] = grid[i][j];
        }
    }

    // Hide all ships that haven't been hit
    for (int i = 0; i < MAX_SHIPS; ++i) {
        for (int j = 0; j < shipSizes[i]; ++j) {
            int shipRow = shipStartRows[i];
            int shipCol = shipStartCols[i];
            if (shipOrientations[i] == HORIZONTAL) {
                shipCol += j;
            } else {
                shipRow += j;
            }
            if (!shipHits[i][j] && displayGridCopy[shipRow][shipCol] == SHIP) {
                displayGridCopy[shipRow][shipCol] = WATER;
            }
        }
    }

    // Display the modified grid
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
        std::cout << static_cast<char>('A' + i) << " | ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (displayGridCopy[i][j] == SHIP)
                std::cout << "~ ";
            else if (displayGridCopy[i][j] == HIT)
                std::cout << "H ";
            else if (displayGridCopy[i][j] == MISS)
                std::cout << "M ";
            else
                std::cout << "~ ";
        }
        std::cout << "\n";
    }
}

// Runs a series of test cases to validate game functions
void runTests() {
    std::cout << "\nRunning Test Cases...\n";

    bool placementTest = testPlacement();
    bool attackTest = testAttack();
    bool fileTest = testFileOperations();

    if (placementTest && attackTest && fileTest) {
        std::cout << "All tests passed successfully!\n";
    } else {
        std::cout << "Some tests failed. Please check the implementation.\n";
    }
}

// Test case for ship placement
bool testPlacement() {
    std::cout << "Testing Ship Placement...\n";
    // Attempt to place a ship out of bounds
    bool result = placeShip(0, BOARD_SIZE, 0, HORIZONTAL);
    if (result) {
        std::cout << "Failed: Ship placed out of bounds.\n";
        return false;
    }

    // Attempt to place a ship overlapping another ship
    // First place a valid ship
    result = placeShip(0, 0, 0, HORIZONTAL);
    if (!result) {
        std::cout << "Failed: Unable to place ship within bounds.\n";
        return false;
    }

    // Attempt to place another ship overlapping the first ship
    result = placeShip(1, 0, 0, VERTICAL);
    if (result) {
        std::cout << "Failed: Ship placed overlapping another ship.\n";
        return false;
    }

    std::cout << "Ship Placement Tests Passed.\n";
    return true;
}

// Test case for attack functionality
bool testAttack() {
    std::cout << "Testing Attack Functionality...\n";
    // Place a ship for testing
    initializeGrid();
    placeShip(0, 0, 0, HORIZONTAL);

    // Attack a ship position
    bool hit = attack(0, 0);
    if (!hit || grid[0][0] != HIT) {
        std::cout << "Failed: Attack did not register as hit.\n";
        return false;
    }

    // Attack a water position
    bool miss = attack(1, 1);
    if (miss) {
        std::cout << "Failed: Attack incorrectly registered as hit.\n";
        return false;
    }

    // Attack the same position again
    bool repeat = attack(0, 0);
    if (repeat) {
        std::cout << "Failed: Repeated attack incorrectly registered as hit.\n";
        return false;
    }

    std::cout << "Attack Functionality Tests Passed.\n";
    return true;
}

// Test case for file operations
bool testFileOperations() {
    std::cout << "Testing File Operations...\n";
    // Set up a test game state
    strcpy(playerName, "TestPlayer");
    shipsRemaining = MAX_SHIPS;
    initializeGrid();
    placeAllShips();

    // Save the game
    saveGame();

    // Modify the game state
    strcpy(playerName, "ModifiedPlayer");
    shipsRemaining = 0;
    initializeGrid();

    // Load the game
    bool loaded = loadGame();
    if (!loaded) {
        std::cout << "Failed: Unable to load saved game.\n";
        return false;
    }

    // Check if the loaded data matches the saved data
    if (strcmp(playerName, "TestPlayer") != 0 || shipsRemaining != MAX_SHIPS) {
        std::cout << "Failed: Loaded game data does not match saved data.\n";
        return false;
    }

    std::cout << "File Operations Tests Passed.\n";
    return true;
}

// Simulates automatic saving in the background using a separate thread
void autoSaveThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(60)); // Auto-save every 60 seconds
        saveGame();
    }
}

// =====================
// Main Function
// =====================

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    // Display game instructions
    displayInstructions();

    // Capture player's name
    getPlayerNameInput();

    // Initialize game grid
    initializeGrid();

    // Allocate memory for ship hits tracking
    allocateShipHits();

    // Place all ships randomly on the grid
    placeAllShips();

    // Start auto-save thread for Input/Output Simultaneous
    std::thread autosave(autoSaveThread);

    // Main game loop variables
    int choice;
    bool exitGame = false;

    // Main menu loop
    while (!exitGame) {
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
        std::cin >> choice;
        std::cin.ignore(); // Clear newline character from input buffer

        switch (choice) {
            case 1:
                // Start New Game
                resetGame();
                shipsRemaining = MAX_SHIPS;
                std::cout << "\nStarting a new game...\n";
                break;
            case 2:
                // Load Game
                if (loadGame()) {
                    std::cout << "\nGame loaded successfully. Welcome back, " << playerName << "!\n";
                } else {
                    std::cout << "\nFailed to load game. Starting a new game.\n";
                    resetGame();
                }
                break;
            case 3:
                // View High Scores
                displayHighScores();
                break;
            case 4:
                // Reset Game
                resetGame();
                shipsRemaining = MAX_SHIPS;
                std::cout << "\nGame has been reset.\n";
                break;
            case 5:
                // Run Tests
                runTests();
                break;
            case 6:
                // Exit
                std::cout << "\nExiting the game. Goodbye!\n";
                exitGame = true;
                break;
            default:
                std::cout << "\nInvalid choice. Please select a valid option.\n";
        }

        // If game is not exited, proceed to gameplay
        if (!exitGame && choice >=1 && choice <=5) {
            bool gameOver = false;
            int moves = 0;

            while (!gameOver) {
                displayGrid(false);
                int row, col;
                getAttackCoordinatesInput(&row, &col);
                moves++;

                if (attack(row, col)) {
                    std::cout << "Hit!\n";
                    if (allShipsSunk()) {
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
    }

    deallocateShipHits();

    if (autosave.joinable()) {
        autosave.detach();
    }

    return 0;
}
