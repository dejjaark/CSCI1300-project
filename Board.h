
#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

class Board {
    private:
        // Static in this context: Belongs to the class, not each object
        // Total number of tiles in a single lane/track
        static const int _BOARD_SIZE = 52;
        // Maximum number of players supported on this board
        static const int _MAX_PLAYERS = 2;

        // Composition!
        // 2 lanes (one per player) by _BOARD_SIZE tiles each
        Tile _tiles[2][_BOARD_SIZE];

        // Number of players actually using the board
        int _player_count;
        // Current position of each player on their lane (index into the lane array)
        int _player_position[_MAX_PLAYERS];

        // Helper to fill a specific lane (0 or 1) with tile colors
        void initializeTiles(int player_index);
        // Returns true if the given player is standing on tile index `pos`
        bool isPlayerOnTile(int player_index, int pos);
        // Prints a single tile (with color and possibly player icon) to the console
        void displayTile(int player_index, int pos);

    public:
        // Default Constructor:
        // Sets up player count, positions, and initializes all lanes/tiles
        Board();

        // Re-initializes all lanes with random tiles
        void initializeBoard();
        // Prints the entire track for a single player lane
        void displayTrack(int player_index);
        // Prints both lanes (for all players)
        void displayBoard();
        // Moves the given player forward by one tile; returns true if they reach the end
        bool movePlayer(int player_index);
        // Recall we can use const for getter functions
        // Returns the current tile index for the given player, or -1 if out of bounds
        int getPlayerPosition(int player_index) const;
        // Returns the color (char) of the tile at [player_index][pos], or ' ' if invalid
        char getTileColor(int player_index, int pos) const;
};

#endif
