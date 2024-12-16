#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

const int MAX_HIGH_SCORES = 10;

// Struct to hold high score entries
struct HighScoreEntry {
    std::string playerName;
    int moves;

    // Constructor to initialize playerName and moves
    HighScoreEntry(const std::string& name = "", int m = 0)
        : playerName(name), moves(m) {}

    // Operator < for sorting
    bool operator<(const HighScoreEntry& other) const {
        return moves < other.moves;
    }
};

// HighScore class
class HighScore {
private:
    std::vector<HighScoreEntry> scores;
    std::string highScoreFile;

    void loadHighScores();
    void saveHighScores() const;

public:
    // Constructors
    HighScore();
    HighScore(const std::string& filename);

    // Member functions
    void displayHighScores() const;
    void updateHighScores(const std::string& playerName, int moves);
};

#endif // HIGHSCORE_H
