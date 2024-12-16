
#include "scoremanager.h"
#include <iostream>

// Default Constructor
ScoreManager::ScoreManager()
    : highScore() {}

// Copy Constructor
ScoreManager::ScoreManager(const ScoreManager& other)
    : highScore(other.highScore) {}

// Destructor
ScoreManager::~ScoreManager() {}

// Assignment Operator
ScoreManager& ScoreManager::operator=(const ScoreManager& other) {
    if (this != &other) {
        highScore = other.highScore;
    }
    return *this;
}

// Overloaded Equality Operator
bool ScoreManager::operator==(const ScoreManager& other) const {
    // Define based on criteria, currently returns true
    return true;
}

// Display High Scores
void ScoreManager::displayHighScores() const {
    highScore.displayHighScores();
}

// Update High Scores
void ScoreManager::updateHighScores(const std::string& playerName, int moves) {
    highScore.updateHighScores(playerName, moves);
}
