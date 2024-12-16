#include "highscore.h"
#include <iostream>
#include <algorithm>
#include <fstream>

// Default Constructor
HighScore::HighScore()
    : scores(), highScoreFile("highscores.bin") {
    loadHighScores();
}

// Parameterized Constructor
HighScore::HighScore(const std::string& filename)
    : scores(), highScoreFile(filename) {
    loadHighScores();
}

// Load High Scores from File
void HighScore::loadHighScores() {
    scores.clear();
    std::ifstream inFile(highScoreFile, std::ios::binary);
    if (!inFile) {
        // No high scores yet
        return;
    }

    int numScores;
    inFile.read(reinterpret_cast<char*>(&numScores), sizeof(numScores));
    for (int i = 0; i < numScores; ++i) {
        HighScoreEntry entry;
        size_t nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        entry.playerName.resize(nameLength);
        inFile.read(&entry.playerName[0], nameLength);
        inFile.read(reinterpret_cast<char*>(&entry.moves), sizeof(entry.moves));
        scores.push_back(entry);
    }

    inFile.close();
}

// Save High Scores to File
void HighScore::saveHighScores() const {
    std::ofstream outFile(highScoreFile, std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cout << "Error saving high scores.\n";
        return;
    }

    int numScores = scores.size();
    outFile.write(reinterpret_cast<const char*>(&numScores), sizeof(numScores));
    for (std::vector<HighScoreEntry>::const_iterator it = scores.begin(); it != scores.end(); ++it) {
        size_t nameLength = it->playerName.size();
        outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        outFile.write(it->playerName.c_str(), nameLength);
        outFile.write(reinterpret_cast<const char*>(&it->moves), sizeof(it->moves));
    }

    outFile.close();
}

// Display High Scores
void HighScore::displayHighScores() const {
    if (scores.empty()) {
        std::cout << "\nNo high scores available.\n";
        return;
    }

    std::cout << "\n===== High Scores =====\n";
    int rank = 1;
    for (std::vector<HighScoreEntry>::const_iterator it = scores.begin(); it != scores.end(); ++it) {
        std::cout << rank++ << ". " << it->playerName << " - " << it->moves << " moves\n";
    }
    std::cout << "=======================\n";
}

// Update High Scores
void HighScore::updateHighScores(const std::string& playerName, int moves) {
    HighScoreEntry newEntry(playerName, moves);
    scores.push_back(newEntry);
    std::sort(scores.begin(), scores.end());
    if (scores.size() > MAX_HIGH_SCORES) {
        scores.pop_back();
    }
    saveHighScores();
    std::cout << "High scores updated!\n";
}
