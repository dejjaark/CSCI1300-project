// Source file calling the header file
#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

// Each of the following defines a macro
// Essentially nicenames to use instead of the corresponding escape sequence ('\') 
#define ORANGE "\033[48;2;230;115;0m"
#define GREY "\033[48;2;128;128;128m"
#define GREEN "\033[48;2;34;139;34m"
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define RED "\033[48;2;230;10;10m"
#define PURPLE "\033[48;2;128;0;128m"
#define RESET "\033[0m"

using namespace std;

// =========================== Constructor ===========================

Board::Board() {
    // Creates two players
    _player_count = _MAX_PLAYERS;

    // Initialize player's position
    // All players start at tile index 0
    for (int i = 0; i < _player_count; i++) {
        _player_position[i] = 0;
    }

    // Fill both lanes
    initializeBoard();
    for(int i = 0; i<2; i++){

        initializeTiles(i);
    }
    // Note: initializeBoard() already calls initializeTiles on both lanes,
    // then the for-loop calls initializeTiles again.
    // So each lane is effectively initialized twice.
}

// =========================== Private Member Functions ===========================

void Board::initializeTiles(int player_index) {
    Tile tile;
    int green_count = 0;
    // Recall 52 from header file
    int total_tiles = _BOARD_SIZE;

    // Loop over all tile indices in the lane for this player
    for (int i = 0; i < total_tiles; i++) {
        // Set the last tile as Orange for the finish line
        if (i == total_tiles - 1) {
            tile.color = 'O';
        } 
        // Set the first tile as Grey for the starting line
        else if (i == 0) {
            tile.color = 'Y';
        } 
        // Hard-coded target of 30 green tiles
        // Probablisitic method to spread out the green tiles randomly
        else if (green_count < 30 && (rand() % (total_tiles - i) < 30 - green_count)) {
            tile.color = 'G';
            green_count++;
        }
        // Randomly assign one of the other colors: Blue, Pink, Brown, Red, Purple
        else {
            int color_choice = rand() % 5;
            switch (color_choice) {
                case 0:
                    tile.color = 'B'; // Blue
                    break;
                case 1:
                    tile.color = 'P'; // Pink
                    break;
                case 2:
                    tile.color = 'T'; // Brown
                    break;
                case 3:
                    tile.color = 'R'; // Red
                    break;
                case 4:
                    tile.color = 'U'; // Purple
                    break;
            }
        }

        // Assign the tile to the board for the specified lane/player 1 or 2
        // Recall i refers to tile 0 to 51
        _tiles[player_index][i] = tile;
    }
}

bool Board::isPlayerOnTile(int player_index, int pos) {
    // Returns true if this player's position matches the tile index `pos`
    if (_player_position[player_index] == pos) {
        return true;
    }
    return false;
}

void Board::displayTile(int player_index, int pos) {
    string color = "";
    // `player` is used as a boolean: 1 if player is on this tile, 0 otherwise
    int player = isPlayerOnTile(player_index, pos);

    // Using the defined nicenames above
    // Choose which color escape code to print based on tile color char
    switch(_tiles[player_index][pos].color) {
        case 'O': color = ORANGE; break;
        case 'Y': color = GREY; break;
        case 'G': color = GREEN; break;
        case 'B': color = BLUE; break;
        case 'P': color = PINK; break;
        case 'T': color = BROWN; break;
        case 'R': color = RED; break;
        case 'U': color = PURPLE; break;
    }

    // Template for displaying a tile: <line filler space> <color start> |<player symbol or blank space>| <reset color> <line filler space> <endl>
    if (player == true) {
        // If the player is on this tile, print their number (1 or 2) between the bars
        cout << color << "|" << (player_index + 1) << "|" << RESET;
    }
    else {
        // Otherwise, show an empty tile
        cout << color << "| |" << RESET;
    }
}

// =========================== Public Member Functions ===========================

void Board::initializeBoard() {
    // Initialize both lanes (0 and 1)
    for (int i = 0; i < 2; i++) {
        // This ensures each lane (or each player) has a unique tile distribution
        initializeTiles(i);
    }
}

void Board::displayTrack(int player_index) {
    // Print all tiles for one lane in a single row
    for (int i = 0; i < _BOARD_SIZE; i++) {
        displayTile(player_index, i);
    }
    cout << endl;
}

void Board::displayBoard() {
    // Print both player tracks one after another
    for (int i = 0; i < 2; i++) {
        displayTrack(i);
        if (i == 0) {
            cout << endl; // Add an extra line between the two lanes
        }
    }
}

bool Board::movePlayer(int player_index) {
    // Increment player position by 1
    _player_position[player_index]++;

    // Player reached last tile
    if (_player_position[player_index] == _BOARD_SIZE - 1) {
        return true;
    }

    return false;
}

int Board::getPlayerPosition(int player_index) const {
    // Ensure index is within valid range
    if (player_index >= 0 && player_index <= _player_count) {
        return _player_position[player_index];
    }
    // Invalid player index → sentinel -1
    return -1;
}

char Board::getTileColor(int player_index, int pos) const {
    // Bounds check for player index and tile position
    if (player_index >= 0 && player_index < _player_count &&
        pos >= 0 && pos < _BOARD_SIZE) {
        // Return the stored tile color
        return _tiles[player_index][pos].color;
    }
    // Return a space if invalid
    return ' ';
}
