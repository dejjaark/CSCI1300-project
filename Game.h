#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <string>

using namespace std;

class Game{

    private:

    struct RandomEvent {
        string description;
        int pathType;
        int advisor;
        int discoverPoint;
    };

    struct Riddle {
        string question;
        string answer;
    };

    Board _board;
    Player _players[2];

    static const int maxEvent = 60;
    static const int maxRiddle = 30;
    static const int maxCharacter = 10;

    RandomEvent _event[maxEvent];
    int _eventCount;

    Riddle _riddles[maxRiddle];
    int _riddleCount;

    Player _characterOptions[maxCharacter];
    int _characterCount;

    public:

    void characterFile(const char filename[]);
    void randomEventFile(const char filename[]);
    void riddleFile(const char filename[]);
    void chooseCharacter();
    void choosePath();
    void pathBonus();

    void play();
    void tileEffect(int player_index, char color);
    void triggerRandomEvent(int player_index);
    void triggerRiddle(int player_index);
    void dnaTask(int player_index, char color);

    int finalScore(Player p) const;
    void winner() const;

    Game();
    void run();
};

#endif
