#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;

class Player {
private:
    // Name of the scientist/player
    string _name;
    // Numerical representation of experience
    int _experience;
    // Stat representing accuracy
    int _accuracy;
    // Stat representing efficiency
    int _efficiency;
    // Stat representing insight
    int _insight;
    // Currency-like score used in the game
    int _discoverPoints;
    // Path chosen by the player: 0 = Fellowship Training, 1 = Direct Lab
    int _pathType;   // 0 = Fellowship Training, 1 = Direct Lab
    // Whether this player has reached the final tile
    bool _finished;  // reached final tile?

public:
    // Default constructor
    Player();

    // Parameterized constructor
    // Initializes all stats and path type with given values
    Player(string name, int experience, int accuracy,
           int efficiency, int insight, int discoverPoints, int pathType);

    // Getters
    string getName() const;
    int getExperience() const;
    int getAccuracy() const;
    int getEfficiency() const;
    int getInsight() const;
    int getDiscoverPoints() const;
    int getPathType() const;
    bool getFinished() const;

    // Setters
    void setName(string name);
    void setExperience(int experience);
    void setAccuracy(int accuracy);
    void setEfficiency(int efficiency);
    void setInsight(int insight);
    void setDiscoverPoints(int discoverPoints);
    void setPathType(int pathType);
    void setFinished(bool finished);

    // Helper functions to adjust stats
    // (increment or decrement by the `input` amount)
    void changeExperience(int input);
    void changeAccuracy(int input);
    void changeEfficiency(int input);
    void changeInsight(int input);
    void changeDiscoverPoints(int input);
};
#endif //PLAYER_H
