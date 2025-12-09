#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <string>

using namespace std;

class Game{

    private:

    // Struct representing a random event loaded from file
    struct RandomEvent {
        string description; // text describing the event
        int pathType;       // which path types it applies to (if used)
        int advisor;        // advisor index/id (if used)
        int discoverPoint;  // how many discovery points are gained or lost
    };

    // Struct representing a riddle loaded from file
    struct Riddle {
        string question; // riddle text
        string answer;   // correct answer
    };

    // The game board that players move on
    Board _board;
    // The two players in this game
    Player _players[2];

    // Maximum number of events that can be stored
    static const int maxEvent = 60;
    // Maximum number of riddles that can be stored
    static const int maxRiddle = 30;
    // Maximum number of character options to load from file
    static const int maxCharacter = 10;

    // Array storing all loaded random events
    RandomEvent _event[maxEvent];
    // Actual number of events loaded
    int _eventCount;

    // Array storing all loaded riddles
    Riddle _riddles[maxRiddle];
    // Actual number of riddles loaded
    int _riddleCount;

    // Array of character templates players can choose from
    Player _characterOptions[maxCharacter];
    // Actual number of character options loaded
    int _characterCount;

    public:

    // Load character options from file
    void characterFile(const char filename[]);
    // Load random events from file
    void randomEventFile(const char filename[]);
    // Load riddles from file
    void riddleFile(const char filename[]);
    // Let each player choose which character they want to use
    void chooseCharacter();
    // Let each player choose which path they want to take (0 or 1)
    void choosePath();
    // Apply path bonuses/penalties to players based on chosen path
    void pathBonus();

    // Main game loop (movement, tile effects, etc.)
    void play();
    // Apply the effect of landing on a tile of a specific color
    void tileEffect(int player_index, char color);
    // Trigger the next random event for the player
    void triggerRandomEvent(int player_index);
    // Trigger the next riddle for the player
    void triggerRiddle(int player_index);
    // Handle DNA tasks (calls DNA functions) depending on tile color
    void dnaTask(int player_index, char color);

    // Computes final score of a player using DP + stats
    int finalScore(Player p) const;
    // Compares scores and prints the winner
    void winner() const;

    // Constructor initializes counters
    Game();
    // High-level driver: load files, choose characters, run the game
    void run();
};

#endif
