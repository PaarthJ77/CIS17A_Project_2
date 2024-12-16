#include "TestSuite.h"
#include "ship.h"
#include "board.h"
#include "player.h"
#include "highscore.h"
#include <iostream>

// Default Constructor
TestSuite::TestSuite() {}

// Copy Constructor
TestSuite::TestSuite(const TestSuite& other) {}

// Destructor
TestSuite::~TestSuite() {}

// Assignment Operator
TestSuite& TestSuite::operator=(const TestSuite& other) {
    if (this != &other) {
        // No dynamic members
    }
    return *this;
}

// Overloaded Equality Operator
bool TestSuite::operator==(const TestSuite& other) const {
    // Define based on criteria, currently returns true
    return true;
}

// Run All Tests
void TestSuite::runAllTests() {
    std::cout << "\nRunning Test Suite...\n";
    testShip();
    testBoard();
    testPlayer();
    testHighScore();
    std::cout << "All tests completed.\n";
}

// Test Ship Class
void TestSuite::testShip() {
    std::cout << "Testing Ship Class...\n";
    Ship ship1("Destroyer", 2);
    ship1.setPosition(0, 0, Orientation::HORIZONTAL);

    if (!ship1.coversCell(0, 0) || !ship1.coversCell(0, 1)) {
        std::cout << "Ship coverage test failed.\n";
    } else {
        std::cout << "Ship coverage test passed.\n";
    }

    if (ship1.hitCell(0, 0) && ship1.hitCell(0, 1)) {
        if (ship1.isSunk()) {
            std::cout << "Ship sunk test passed.\n";
        } else {
            std::cout << "Ship sunk test failed.\n";
        }
    } else {
        std::cout << "Ship hit test failed.\n";
    }
}

// Test Board Class
void TestSuite::testBoard() {
    std::cout << "Testing Board Class...\n";
    Board board;
    board.initializeGrid();
    try {
        bool placed = board.placeShip(0, 0, 0, Orientation::HORIZONTAL);
        if (placed) {
            std::cout << "Ship placement test passed.\n";
        } else {
            std::cout << "Ship placement test failed.\n";
        }

        bool hit = board.attack(0, 0);
        if (hit && board.getShipsRemaining() == MAX_SHIPS) {
            std::cout << "Board attack test passed.\n";
        } else {
            std::cout << "Board attack test failed.\n";
        }
    } catch (const GameException& e) {
        std::cout << "Exception during board test: " << e.what() << "\n";
    }
}

// Test Player Class
void TestSuite::testPlayer() {
    std::cout << "Testing Player Class...\n";
    Player player("Tester");
    if (player.getName() == "Tester") {
        std::cout << "Player name test passed.\n";
    } else {
        std::cout << "Player name test failed.\n";
    }
}

// Test HighScore Class
void TestSuite::testHighScore() {
    std::cout << "Testing HighScore Class...\n";
    HighScore highScore;
    highScore.updateHighScores("Tester", 10);
    highScore.displayHighScores();
    std::cout << "HighScore test completed.\n";
}
