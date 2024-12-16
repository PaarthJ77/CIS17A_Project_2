#include "player.h"

// Static Ship Details
static const std::string SHIP_NAMES[MAX_SHIPS] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
static const int SHIP_SIZES[MAX_SHIPS] = {5, 4, 3, 3, 2};

// Default Constructor
Player::Player()
    : name("Player"), ships() {
    initializeShips();
}

// Parameterized Constructor
Player::Player(const std::string& name)
    : name(name), ships() {
    initializeShips();
}

// Copy Constructor
Player::Player(const Player& other)
    : name(other.name), ships(other.ships) {}

// Destructor
Player::~Player() {}

// Assignment Operator
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;
        ships = other.ships;
    }
    return *this;
}

// Overloaded Equality Operator
bool Player::operator==(const Player& other) const {
    return (name == other.name) && (ships == other.ships);
}

// Overloaded Output Operator
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player Name: " << player.name << "\nShips:\n";
    for (const auto& ship : player.ships) {
        os << ship << "\n";
    }
    return os;
}

// Overloaded Input Operator
std::istream& operator>>(std::istream& is, Player& player) {
    std::cout << "Enter player name: ";
    std::getline(is, player.name);
    if (player.name.empty()) {
        player.name = "Player";
    }
    return is;
}

// Set Player Name
void Player::setName(const std::string& name) {
    this->name = name;
}

// Get Player Name
std::string Player::getName() const {
    return name;
}

// Initialize Ships
void Player::initializeShips() {
    for (int i = 0; i < MAX_SHIPS; ++i) {
        ships[i] = Ship(SHIP_NAMES[i], SHIP_SIZES[i]);
    }
}

// Get Ships (modifiable)
std::array<Ship, MAX_SHIPS>& Player::getShips() {
    return ships;
}

// Get Ships (const)
const std::array<Ship, MAX_SHIPS>& Player::getShips() const {
    return ships;
}
