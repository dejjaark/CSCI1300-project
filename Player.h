
#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;

class Player {
private:
    string _name;
    int _experience;
    int _accuracy;
    int _efficiency;
    int _insight;
    int _discoverPoints;
    int _pathType;   // 0 = Fellowship Training, 1 = Direct Lab
    bool _finished;  // reached final tile?

public:
    // Default constructor
    Player();

    // Parameterized constructor
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
    void changeExperience(int input);
    void changeAccuracy(int input);
    void changeEfficiency(int input);
    void changeInsight(int input);
    void changeDiscoverPoints(int input);
};
#endif //PLAYER_H