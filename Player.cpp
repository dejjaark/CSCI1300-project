#include "Player.h"

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
std::string Player::getName() const { return _name; }
int Player::getExperience() const { return _experience; }
int Player::getAccuracy() const { return _accuracy; }
int Player::getEfficiency() const { return _efficiency; }
int Player::getInsight() const { return _insight; }
int Player::getDiscoverPoints() const { return _discoverPoints; }
int Player::getPathType() const { return _pathType; }
bool Player::getFinished() const { return _finished; }

// Setters
void Player::setName(std::string name) { _name = name; }
void Player::setExperience(int experience) { _experience = experience; }
void Player::setAccuracy(int accuracy) { _accuracy = accuracy; }
void Player::setEfficiency(int efficiency) { _efficiency = efficiency; }
void Player::setInsight(int insight) { _insight = insight; }
void Player::setDiscoverPoints(int discoverPoints) { _discoverPoints = discoverPoints; }
void Player::setPathType(int pathType) { _pathType = pathType; }
void Player::setFinished(bool finished) { _finished = finished; }

// Stat adjust helpers
void Player::changeExperience(int delta) { _experience += delta; }
void Player::changeAccuracy(int delta) { _accuracy += delta; }
void Player::changeEfficiency(int delta) { _efficiency += delta; }
void Player::changeInsight(int delta) { _insight += delta; }
void Player::changeDiscoverPoints(int delta) { _discoverPoints += delta; }
