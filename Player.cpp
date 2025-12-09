#include "Player.h"

// Default constructor initializes all numeric stats and flags to 0/false.
Player::Player() {
    _name = "";
    _experience = 0;
    _accuracy = 0;
    _efficiency = 0;
    _insight = 0;
    _discoverPoints = 0;
    _pathType = 0;
    _finished = false;
}

// Parameterized constructor sets up a new player with given stats and path type.
Player::Player(std::string name, int experience, int accuracy,
               int efficiency, int insight, int discoverPoints, int pathType) {
    _name = name;
    _experience = experience;
    _accuracy = accuracy;
    _efficiency = efficiency;
    _insight = insight;
    _discoverPoints = discoverPoints;
    _pathType = pathType;
    _finished = false;
}

// Getters
// Return the player's name
std::string Player::getName() const { return _name; }
// Return experience stat
int Player::getExperience() const { return _experience; }
// Return accuracy stat
int Player::getAccuracy() const { return _accuracy; }
// Return efficiency stat
int Player::getEfficiency() const { return _efficiency; }
// Return insight stat
int Player::getInsight() const { return _insight; }
// Return current Discover Points
int Player::getDiscoverPoints() const { return _discoverPoints; }
// Return chosen path type (0 or 1)
int Player::getPathType() const { return _pathType; }
// Return whether player has finished the board
bool Player::getFinished() const { return _finished; }

// Setters
// Set the player's name
void Player::setName(std::string name) { _name = name; }
// Set experience stat
void Player::setExperience(int experience) { _experience = experience; }
// Set accuracy stat
void Player::setAccuracy(int accuracy) { _accuracy = accuracy; }
// Set efficiency stat
void Player::setEfficiency(int efficiency) { _efficiency = efficiency; }
// Set insight stat
void Player::setInsight(int insight) { _insight = insight; }
// Set Discover Points
void Player::setDiscoverPoints(int discoverPoints) { _discoverPoints = discoverPoints; }
// Set path type stat
void Player::setPathType(int pathType) { _pathType = pathType; }
// Mark player as finished or not
void Player::setFinished(bool finished) { _finished = finished; }

// Stat adjust helpers
// Add `input` to experience
void Player::changeExperience(int input) { _experience += input; }
// Add `input` to accuracy
void Player::changeAccuracy(int input) { _accuracy += input; }
// Add `input` to efficiency
void Player::changeEfficiency(int input) { _efficiency += input; }
// Add `input` to insight
void Player::changeInsight(int input) { _insight += input; }
// Add `input` to Discover Points
void Player::changeDiscoverPoints(int input) { _discoverPoints += input; }
