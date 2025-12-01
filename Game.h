#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include "DNAUtils.h"
#include <string>

class Game {
private:
    struct RandomEvent {
        std::string description;
        int pathType;        // 0 = Fellowship, 1 = Direct Lab (from file)
        int advisorType;     // 0 = none, 1..5 as in spec (we can ignore for now)
        int dpDelta;         // gain or loss in Discover Points
    };

    struct Riddle {
        std::string question;
        std::string answer;
    };

    Board _board;
    Player _players[2];

    static const int MAX_EVENTS = 60;
    static const int MAX_RIDDLES = 30;

    RandomEvent _events[MAX_EVENTS];
    int _eventCount;

    Riddle _riddles[MAX_RIDDLES];
    int _riddleCount;

    // helpers
    void loadCharactersFromFile(const char filename[]);
    void loadRandomEvents(const char filename[]);
    void loadRiddles(const char filename[]);

    void chooseCharacters();
    void choosePaths();
    void applyPathBonuses();  // fellowship vs direct lab

    void play();

    // tile handling
    void resolveTileEffect(int player_index, char color);
    void triggerRandomEvent(int player_index);
    void triggerRiddle(int player_index);
    void handleDNATask(int player_index, char color);

    // scoring
    int calculateFinalScore(const Player& p) const;
    void announceWinner() const;

public:
    Game();
    void run();
};

#endif
