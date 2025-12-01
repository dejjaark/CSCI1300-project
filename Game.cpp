#include "Game.h"
#include "DNAUtils.h"
#include <iostream>
#include <fstream>
#include <cstdlib>  // rand, srand
#include <ctime>    // time
#include <cctype>   // tolower

using namespace std;

// Helper function to lowercase a string (for riddle answers)
static string toLowerCase(const string &s) {
    string result = s;
    for (int i = 0; i < (int)result.length(); i++) {
        result[i] = static_cast<char>(tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

Game::Game() {
    _eventCount = 0;
    _riddleCount = 0;
    _characterCount = 0;
    srand(static_cast<unsigned int>(time(0)));
}

/**
 * Load scientist characters from characters.txt
 * Expected format per line (after header):
 * name|experience|accuracy|efficiency|insight|discoveryPoints
 */
void Game::loadCharactersFromFile(const char filename[]) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    _characterCount = 0;

    string line;
    // Skip header line
    getline(fin, line);

    while (getline(fin, line) && _characterCount < MAX_CHARACTERS) {
        if (line.size() == 0) continue;

        int pos1 = line.find('|');
        int pos2 = line.find('|', pos1 + 1);
        int pos3 = line.find('|', pos2 + 1);
        int pos4 = line.find('|', pos3 + 1);
        int pos5 = line.find('|', pos4 + 1);

        if (pos1 == -1 || pos2 == -1 || pos3 == -1 || pos4 == -1 || pos5 == -1) {
            continue;
        }

        string name = line.substr(0, pos1);
        int exp = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        int acc = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        int eff = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
        int ins = stoi(line.substr(pos4 + 1, pos5 - pos4 - 1));
        int dp  = stoi(line.substr(pos5 + 1));

        // pathType = 0 for now; chosen later
        _characterOptions[_characterCount] = Player(name, exp, acc, eff, ins, dp, 0);
        _characterCount++;
    }

    fin.close();

    if (_characterCount == 0) {
        cout << "Warning: no characters loaded from " << filename << endl;
    }
}

/**
 * Load random events from random_events.txt
 * Expected format per line (after header):
 * description|pathType|advisorType|dpDelta
 */
void Game::loadRandomEvents(const char filename[]) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    _eventCount = 0;

    string line;
    // Skip header
    getline(fin, line);

    while (getline(fin, line) && _eventCount < MAX_EVENTS) {
        if (line.size() == 0) continue;
        if (line[0] == '/') continue;  // allow comment lines

        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        if (p1 == -1 || p2 == -1 || p3 == -1) continue;

        RandomEvent e;
        e.description = line.substr(0, p1);
        e.pathType    = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        e.advisorType = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        e.dpDelta     = stoi(line.substr(p3 + 1));

        _events[_eventCount] = e;
        _eventCount++;
    }

    fin.close();
}

/**
 * Load riddles from riddles.txt
 * Expected format per line (after header):
 * question|answer
 */
void Game::loadRiddles(const char filename[]) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    _riddleCount = 0;

    string line;
    // Skip header
    getline(fin, line);

    while (getline(fin, line) && _riddleCount < MAX_RIDDLES) {
        if (line.size() == 0) continue;

        int p = line.find('|');
        if (p == -1) continue;

        Riddle r;
        r.question = line.substr(0, p);
        r.answer   = line.substr(p + 1);

        _riddles[_riddleCount] = r;
        _riddleCount++;
    }

    fin.close();
}

/**
 * Let each player choose their character from the loaded list.
 */
void Game::chooseCharacters() {
    if (_characterCount < 2) {
        cout << "Not enough characters in file. Using first two by default.\n";
        _players[0] = _characterOptions[0];
        _players[1] = _characterOptions[1];
        return;
    }

    cout << "\n===== AVAILABLE SCIENTISTS =====\n";
    for (int i = 0; i < _characterCount; i++) {
        cout << (i + 1) << ") " << _characterOptions[i].getName()
             << "  [Exp: " << _characterOptions[i].getExperience()
             << ", Acc: " << _characterOptions[i].getAccuracy()
             << ", Eff: " << _characterOptions[i].getEfficiency()
             << ", Ins: " << _characterOptions[i].getInsight()
             << ", DP: "  << _characterOptions[i].getDiscoverPoints()
             << "]\n";
    }

    int choice1 = 0;
    int choice2 = 0;

    // Player 1 chooses
    cout << "\nPlayer 1: choose your scientist (1-" << _characterCount << "): ";
    cin >> choice1;
    while (choice1 < 1 || choice1 > _characterCount) {
        cout << "Invalid choice. Try again: ";
        cin >> choice1;
    }

    // Player 2 chooses different scientist
    cout << "Player 2: choose your scientist (1-" << _characterCount
         << "), but not " << choice1 << ": ";
    cin >> choice2;
    while (choice2 < 1 || choice2 > _characterCount || choice2 == choice1) {
        cout << "Invalid choice. Try again: ";
        cin >> choice2;
    }

    _players[0] = _characterOptions[choice1 - 1];
    _players[1] = _characterOptions[choice2 - 1];

    cout << "\nPlayer 1 chose: " << _players[0].getName() << endl;
    cout << "Player 2 chose: " << _players[1].getName() << endl;
}

/**
 * Let each player choose a path: Training Fellowship (0) or Direct Lab (1).
 * Then apply the path bonuses/penalties.
 */
void Game::choosePaths() {
    int choice;

    for (int i = 0; i < 2; i++) {
        cout << "\nPath selection for Player " << (i + 1)
             << " (" << _players[i].getName() << ")\n";
        cout << "0 = Training Fellowship (lower starting DP, higher stats)\n";
        cout << "1 = Direct Lab Assignment (more starting DP, smaller stat boost)\n";
        cout << "Your choice: ";
        cin >> choice;
        while (choice != 0 && choice != 1) {
            cout << "Invalid choice. Enter 0 or 1: ";
            cin >> choice;
        }
        _players[i].setPathType(choice);
    }

    applyPathBonuses();
}

/**
 * Apply the path bonuses/penalties based on the project writeup.
 */
void Game::applyPathBonuses() {
    for (int i = 0; i < 2; i++) {
        if (_players[i].getPathType() == 0) {
            // Training Fellowship
            _players[i].changeDiscoverPoints(-5000);
            _players[i].changeAccuracy(500);
            _players[i].changeEfficiency(500);
            _players[i].changeInsight(1000);
            cout << _players[i].getName()
                 << " takes Training Fellowship: -5000 DP, big stat boosts.\n";
        } else {
            // Direct Lab Assignment
            _players[i].changeDiscoverPoints(5000);
            _players[i].changeAccuracy(200);
            _players[i].changeEfficiency(200);
            _players[i].changeInsight(200);
            cout << _players[i].getName()
                 << " goes Direct Lab Assignment: +5000 DP, modest stat boosts.\n";
        }
    }
}

/**
 * Main gameplay loop: players move along the board, tile effects fire, game ends
 * when both players reach the final orange tile.
 */
void Game::play() {
    _board.initializeBoard();

    bool finished[2] = {false, false};

    cout << "\n===== BEGIN JOURNEY THROUGH THE GENOME =====\n";
    _board.displayBoard();

    while (!(finished[0] && finished[1])) {
        for (int i = 0; i < 2; i++) {
            if (finished[i]) continue;

            cout << "\n--- Player " << (i + 1)
                 << " (" << _players[i].getName() << ") turn ---" << endl;
            cout << "Rolling and moving...\n";

            bool reachedEnd = _board.movePlayer(i);
            _board.displayBoard();

            int pos = _board.getPlayerPosition(i);
            char color = _board.getTileColor(i, pos);

            if (reachedEnd) {
                cout << "Player " << (i + 1)
                     << " reached the Genome Conference (final tile)!\n";
                finished[i] = true;
                _players[i].setFinished(true);
            } else {
                resolveTileEffect(i, color);
            }
        }
    }

    cout << "\nBoth players have reached the final tile!\n";
    announceWinner();
}

/**
 * Decide what happens based on the color of the tile.
 */
void Game::resolveTileEffect(int player_index, char color) {
    switch (color) {
        case 'G':  // Green: regular tile, 50% chance random event
            cout << "Green tile: Regular tile. 50% chance of random event...\n";
            if (rand() % 2 == 0) {
                triggerRandomEvent(player_index);
            } else {
                cout << "No event this time. You take a moment to plan your next move.\n";
            }
            break;

        case 'B': // Blue: DNA Task 1 (equal-length similarity)
        case 'P': // Pink: DNA Task 2 (best match)
        case 'R': // Red: DNA Task 3 (mutations)
        case 'T': // Brown: DNA Task 4 (transcription)
            handleDNATask(player_index, color);
            break;

        case 'U': // Purple: Riddle tile
            triggerRiddle(player_index);
            break;

        default:
            // Yellow (start), Orange (end), or any color we don't handle
            cout << "Nothing special on this tile.\n";
            break;
    }
}

/**
 * Trigger a random event from the loaded list and adjust Discover Points.
 */
void Game::triggerRandomEvent(int player_index) {
    if (_eventCount == 0) {
        cout << "No random events loaded.\n";
        return;
    }

    int idx = rand() % _eventCount;
    RandomEvent &e = _events[idx];

    cout << "\n--- RANDOM EVENT ---\n";
    cout << e.description << endl;

    if (e.dpDelta >= 0) {
        cout << "You gain " << e.dpDelta << " Discover Points!\n";
    } else {
        cout << "You lose " << -e.dpDelta << " Discover Points...\n";
    }

    _players[player_index].changeDiscoverPoints(e.dpDelta);
    cout << "New Discover Points: "
         << _players[player_index].getDiscoverPoints() << "\n";
}

/**
 * Handle DNA tasks based on tile color.
 * B: strandSimilarity
 * P: bestStrandMatch
 * R: identifyMutations
 * T: transcribeDNAtoRNA
 */
void Game::handleDNATask(int player_index, char color) {
    string s1, s2;

    cout << "\n--- DNA TASK ---\n";

    if (color == 'B') {
        cout << "Blue tile: DNA Task 1 - Similarity (Equal-Length)\n";
        cout << "Enter first DNA strand: ";
        cin >> s1;
        cout << "Enter second DNA strand (same length): ";
        cin >> s2;

        double score = strandSimilarity(s1, s2);
        int bonus = static_cast<int>(score * 200); // up to +200
        _players[player_index].changeAccuracy(bonus);
        cout << "Accuracy increased by " << bonus << " points.\n";
    }
    else if (color == 'P') {
        cout << "Pink tile: DNA Task 2 - Best Strand Match (Unequal-Length)\n";
        cout << "Enter input strand: ";
        cin >> s1;
        cout << "Enter target strand: ";
        cin >> s2;

        int idx = bestStrandMatch(s1, s2);
        if (idx != -1) {
            _players[player_index].changeEfficiency(150);
            cout << "Efficiency increased by 150 points.\n";
        }
    }
    else if (color == 'R') {
        cout << "Red tile: DNA Task 3 - Mutation Identification\n";
        cout << "Enter input strand: ";
        cin >> s1;
        cout << "Enter target strand: ";
        cin >> s2;

        identifyMutations(s1, s2);
        _players[player_index].changeInsight(150);
        cout << "Insight increased by 150 points.\n";
    }
    else if (color == 'T') {
        cout << "Brown tile: DNA Task 4 - Transcribe DNA to RNA\n";
        cout << "Enter DNA strand: ";
        cin >> s1;

        transcribeDNAtoRNA(s1);
        _players[player_index].changeAccuracy(50);
        _players[player_index].changeEfficiency(50);
        cout << "Accuracy and Efficiency increased by 50 points each.\n";
    }

    cout << "Current stats - Accuracy: " << _players[player_index].getAccuracy()
         << ", Efficiency: " << _players[player_index].getEfficiency()
         << ", Insight: " << _players[player_index].getInsight() << "\n";
}

/**
 * Trigger a riddle from riddles.txt. Correct answer → +500 Insight.
 * (Assumes answers are one word or short phrase; uses case-insensitive comparison.)
 */
void Game::triggerRiddle(int player_index) {
    if (_riddleCount == 0) {
        cout << "No riddles loaded.\n";
        return;
    }

    int idx = rand() % _riddleCount;
    Riddle &r = _riddles[idx];

    cout << "\n--- RIDDLE TILE ---\n";
    cout << r.question << endl;
    cout << "Your answer (one word or short phrase): ";

    string answer;
    // clear leftover newline if any
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, answer);

    string userAns = toLowerCase(answer);
    string correct = toLowerCase(r.answer);

    if (userAns == correct) {
        cout << "Correct! Insight +500.\n";
        _players[player_index].changeInsight(500);
    } else {
        cout << "Incorrect. The correct answer was: " << r.answer << endl;
    }

    cout << "Insight is now " << _players[player_index].getInsight() << "\n";
}

/**
 * Compute final score: base Discover Points plus
 * (Acc / 100 + Eff / 100 + Ins / 100) * 1000.
 */
int Game::calculateFinalScore(const Player& p) const {
    int total = p.getDiscoverPoints();

    total += (p.getAccuracy()   / 100) * 1000;
    total += (p.getEfficiency() / 100) * 1000;
    total += (p.getInsight()    / 100) * 1000;

    return total;
}

/**
 * Announce winner based on final computed scores.
 */
void Game::announceWinner() const {
    int score1 = calculateFinalScore(_players[0]);
    int score2 = calculateFinalScore(_players[1]);

    cout << "\n===== FINAL SCORES =====\n";
    cout << _players[0].getName() << ": " << score1 << " total points\n";
    cout << _players[1].getName() << ": " << score2 << " total points\n";

    if (score1 > score2) {
        cout << "Winner: " << _players[0].getName()
             << " is the new Lead Genomicist!\n";
    } else if (score2 > score1) {
        cout << "Winner: " << _players[1].getName()
             << " is the new Lead Genomicist!\n";
    } else {
        cout << "It's a tie! Both players become Co-Lead Genomicists!\n";
    }
}

/**
 * High-level driver for the game.
 */
void Game::run() {
    loadCharactersFromFile("characters.txt");
    loadRandomEvents("random_events.txt");
    loadRiddles("riddles.txt");

    chooseCharacters();
    choosePaths();
    play();
}

