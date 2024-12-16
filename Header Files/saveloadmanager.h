#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include "common.h"
#include "player.h"
#include "board.h"
#include "gameexception.h"
#include <string>

// SaveLoadManager class
class SaveLoadManager {
private:
    std::string saveFile;

public:
    // Constructors
    SaveLoadManager();
    SaveLoadManager(const std::string& filename);

    // Member functions
    bool saveGame(const Player& player, const Board& board, const std::string& playerName);
    bool loadGame(Player& player, Board& board, std::string& playerName);
};

#endif // SAVELOADMANAGER_H
