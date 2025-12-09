#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <string>
using namespace std;

class Game {
private:
    // Store info about a single random event read from random_events.txt
    struct RandomEvent {
        string description;
        int pathType;
        int advisorType;
        int dpDelta;
    };

    // Store info about a single riddle from riddles.txt
    struct Riddle {
        string question;
        string answer;
    };

    Board _board;        // The game board
    Player _players[2];  // Two players in the game

    // Limits for how many things we can store
    static const int MAX_EVENTS = 60;
    static const int MAX_RIDDLES = 30;
    static const int MAX_CHARACTERS = 10;

    // Arrays to hold events / riddles / character options
    RandomEvent _events[MAX_EVENTS];
    int _eventCount;

    Riddle _riddles[MAX_RIDDLES];
    int _riddleCount;

    Player _characterOptions[MAX_CHARACTERS];
    int _characterCount;

    // ----- Helper functions used inside the Game -----

    // File loaders
    void loadCharactersFromFile(const char filename[]);
    void loadRandomEvents(const char filename[]);
    void loadRiddles(const char filename[]);

    // Setup choices
    void chooseCharacters();
    void choosePaths();
    void applyPathBonuses();

    // Gameplay
    void play();
    void resolveTileEffect(int player_index, char color);
    void triggerRandomEvent(int player_index);
    void triggerRiddle(int player_index);
    void handleDNATask(int player_index, char color);

    // Scoring
    int calculateFinalScore(const Player& p) const;
    void announceWinner() const;

public:
    Game();   // constructor
    void run(); // entry point to run the whole game
};

#endif

