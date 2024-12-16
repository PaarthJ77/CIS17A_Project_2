#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "highscore.h"
#include <string>
#include "common.h"

class ScoreManager {
private:
    HighScore highScore;

public:
    // Constructors
    ScoreManager();
    ScoreManager(const ScoreManager& other); 
    // Destructor
    ~ScoreManager();

    // Assignment Operator
    ScoreManager& operator=(const ScoreManager& other);

    // Overloaded Operators
    bool operator==(const ScoreManager& other) const;

    // Member functions
    void displayHighScores() const;
    void updateHighScores(const std::string& playerName, int moves);
};

#endif // SCOREMANAGER_H
