#include "Game.h"
#include "DNAUtils.h"
#include <iostream>
#include <fstream>

using namespace std;

Game::Game() {
    // Initialize default players
    _players[0] = Player();
    _players[1] = Player();
}

void Game::loadCharactersFromFile(const char filename[]) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    string header;
    getline(fin, header); // skip header line

    string line;
    int index = 0;
    while (getline(fin, line) && index < 2) {
        string name;
        int exp, acc, eff, ins, dp;

        int pos1 = line.find('|');
        int pos2 = line.find('|', pos1 + 1);
        int pos3 = line.find('|', pos2 + 1);
        int pos4 = line.find('|', pos3 + 1);
        int pos5 = line.find('|', pos4 + 1);

        if (pos1 == -1 || pos2 == -1 || pos3 == -1 || pos4 == -1 || pos5 == -1) {
            continue;
        }

        name = line.substr(0, pos1);
        exp = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        acc = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        eff = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
        ins = stoi(line.substr(pos4 + 1, pos5 - pos4 - 1));
        dp  = stoi(line.substr(pos5 + 1));

        _players[index] = Player(name, exp, acc, eff, ins, dp, 0);
        index++;
    }

    fin.close();
}

void Game::chooseCharacters() const {
    cout << "For now, using the default first two scientists from file as Player 1 and Player 2." << endl;
    cout << "Player 1: " << _players[0].getName() << endl;
    cout << "Player 2: " << _players[1].getName() << endl;
}

void Game::choosePaths() {
    int choice;

    for (int i = 0; i < 2; i++) {
        cout << "Player " << (i + 1) << " (" << _players[i].getName() << "):\n";
        cout << "Choose path: 0 = Fellowship Training, 1 = Direct Lab Assignment: ";
        cin >> choice;
        if (choice != 0 && choice != 1) {
            choice = 0;
        }
        _players[i].setPathType(choice);
    }
}

void Game::play() {
    _board.initializeBoard();

    bool finished[2] = {false, false};

    while (!(finished[0] && finished[1])) {
        for (int i = 0; i < 2; i++) {
            if (finished[i]) {
                continue;
            }

            cout << "\n--- Player " << (i + 1) << " turn ---" << endl;
            cout << "Press enter to roll and move...";
            cin.ignore(10000, '\n');
            cin.get();

            bool reachedEnd = _board.movePlayer(i);
            _board.displayBoard();

            if (reachedEnd) {
                cout << "Player " << (i + 1) << " reached the Genome Conference!" << endl;
                finished[i] = true;
                _players[i].setFinished(true);
            }
        }
    }

    cout << "\nBoth players reached the final tile!" << endl;
}

void Game::run() {
    loadCharactersFromFile("characters.txt");
    chooseCharacters();
    choosePaths();
    play();
}


