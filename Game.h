
#ifndef GAME_H
#define GAME_H
#include "Board.h"
#include "Player.h"

class Game {
private:
    Board _board;
    Player _players[2];

    void loadCharactersFromFile(const char filename[]);
    void chooseCharacters() const;
    void choosePaths();
    void play();

public:
    Game();
    void run();
};

#endif //GAME_H
