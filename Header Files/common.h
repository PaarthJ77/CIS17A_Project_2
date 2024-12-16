#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <array>

// Enumerations
enum Orientation { HORIZONTAL, VERTICAL };
enum CellStatus { WATER, SHIP, HIT, MISS };

// Constants
const int BOARD_SIZE = 10;
const int MAX_SHIPS = 5;

// Forward declarations to avoid circular dependencies
class Ship;
class Player;
class Board;
class Game;

#endif // COMMON_H
