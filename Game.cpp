/*
 * Game.cpp
 *
 * This file defines how the Game class actually works.
 * The Game class is responsible for:
 *  - Loading characters, random events, and riddles from files.
 *  - Letting each player pick which scientist they want to be.
 *  - Letting each player choose a path (Training Fellowship or Direct Lab).
 *  - Running the board game: moving players along the Board and
 *    triggering tile effects based on tile colors.
 *  - Calculating final scores and announcing the winner.
 *
 * NOTE: This version avoids using <cstdlib>, <ctime>, <cctype>, and static_cast
 *       to better match some CS course restrictions.
 */

#include "Game.h"      // Declaration of the Game class and its members
#include "DNAUtils.h"  // DNA-related helper functions used on certain tiles

#include <iostream>    // For cout, cin
#include <fstream>     // For ifstream (file reading)
#include <string>      // For std::string

using namespace std;   // So we don't have to write std:: everywhere

/*
 * toLowerCaseSimple:
 * -------------------
 * A small helper function that converts all uppercase letters in a string
 * to lowercase, without using <cctype> and tolower().
 *
 * We manually check each character and, if it is between 'A' and 'Z',
 * convert it to the corresponding lowercase letter by shifting its ASCII code.
 */
string toLowerCaseSimple(string s) {
    // Loop over every character in the string by index
    for (int i = 0; i < s.length(); i++) {
        // Check if s[i] is in the range 'A'..'Z'
        if (s[i] >= 'A' && s[i] <= 'Z') {
            // Convert to lowercase by subtracting 'A' and adding 'a'
            // Example: 'B' - 'A' = 1; 1 + 'a' = 'b'
            s[i] = s[i] - 'A' + 'a';
        }
    }
    // Return the modified lowercase string
    return s;
}

/*
 * Game::Game (constructor)
 * ------------------------
 * Initializes the Game object by setting all counts to zero.
 * We do NOT seed any random number generator here, because we are not using rand().
 */
Game::Game() {
    _eventCount = 0;       // No random events loaded yet
    _riddleCount = 0;      // No riddles loaded yet
    _characterCount = 0;   // No scientist characters loaded yet
}

/*
 * loadCharactersFromFile:
 * -----------------------
 * This function reads scientist characters from a text file, such as "characters.txt".
 *
 * Expected format of the file:
 *  - First line: header (ignored)
 *  - Each following line:
 *      name|experience|accuracy|efficiency|insight|discoveryPoints
 *
 * The function parses each line, creates a Player object with those stats,
 * and stores it in the _characterOptions array. It also updates _characterCount.
 */
void Game::loadCharactersFromFile(const char filename[]) {
    ifstream fin(filename);  // Create an input file stream

    // Check if the file opened successfully
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;  // Exit the function early if file can't be opened
    }

    _characterCount = 0;    // Reset the character count before reading

    string line;
    // The first line is assumed to be a header, so we read and ignore it
    getline(fin, line);

    // Read each subsequent line from the file until EOF or we hit MAX_CHARACTERS
    while (getline(fin, line) && _characterCount < MAX_CHARACTERS) {
        // Skip completely empty lines
        if (line.size() == 0) continue;

        // Find the positions of the '|' separators
        int pos1 = line.find('|');
        int pos2 = line.find('|', pos1 + 1);
        int pos3 = line.find('|', pos2 + 1);
        int pos4 = line.find('|', pos3 + 1);
        int pos5 = line.find('|', pos4 + 1);

        // If any of the separators are missing, the line is malformed; skip it
        if (pos1 == -1 || pos2 == -1 || pos3 == -1 || pos4 == -1 || pos5 == -1) {
            continue;
        }

        // Extract the name: everything from start of line up to pos1
        string name = line.substr(0, pos1);

        // Extract and convert each numerical field using stoi
        int exp = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));  // experience
        int acc = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));  // accuracy
        int eff = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));  // efficiency
        int ins = stoi(line.substr(pos4 + 1, pos5 - pos4 - 1));  // insight
        int dp  = stoi(line.substr(pos5 + 1));                   // discoveryPoints

        // Create a Player object with the read data.
        // The last parameter (0) is pathType here; the player chooses path later.
        _characterOptions[_characterCount] = Player(name, exp, acc, eff, ins, dp, 0);

        // Move to the next slot in the _characterOptions array
        _characterCount++;
    }

    // Always close the file when finished
    fin.close();

    // If no characters were loaded, warn the user
    if (_characterCount == 0) {
        cout << "Warning: no characters loaded from " << filename << endl;
    }
}

/*
 * loadRandomEvents:
 * -----------------
 * Reads random event data from a text file, such as "random_events.txt".
 *
 * Expected format of each line (after header):
 *    description|pathType|advisorType|dpDelta
 *
 * description: text describing what happens
 * pathType   : int specifying which path it applies to (may not be used fully here)
 * advisorType: int representing which advisor might protect you (not fully used here)
 * dpDelta    : change in Discover Points (can be positive or negative)
 *
 * Each event is stored in the _events[] array, and _eventCount is updated.
 */
void Game::loadRandomEvents(const char filename[]) {
    ifstream fin(filename);  // Input file stream

    // Check if file opened
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    _eventCount = 0;  // Reset event count

    string line;
    // Skip header line
    getline(fin, line);

    // Read each event line
    while (getline(fin, line) && _eventCount < MAX_EVENTS) {
        // Skip empty lines
        if (line.size() == 0) continue;
        // If we decide that lines starting with '/' are comments, skip them
        if (line[0] == '/') continue;

        // Find the '|' delimiters
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        // If the line doesn't have the right number of separators, skip it
        if (p1 == -1 || p2 == -1 || p3 == -1) continue;

        // Create a new RandomEvent struct and fill it from the line
        RandomEvent e;
        e.description = line.substr(0, p1);
        e.pathType    = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        e.advisorType = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        e.dpDelta     = stoi(line.substr(p3 + 1));

        // Store the event in the array and increment the count
        _events[_eventCount] = e;
        _eventCount++;
    }

    // Close the file after reading
    fin.close();
}

/*
 * loadRiddles:
 * ------------
 * Reads riddle questions and answers from a text file, such as "riddles.txt".
 *
 * Expected format per line (after header):
 *    question|answer
 *
 * Each riddle is stored in _riddles[], and _riddleCount is updated.
 */
void Game::loadRiddles(const char filename[]) {
    ifstream fin(filename);  // Input file stream

    // Check if file opened correctly
    if (!fin.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }

    _riddleCount = 0;  // Reset riddle count

    string line;
    // Skip header line
    getline(fin, line);

    // Read each riddle line
    while (getline(fin, line) && _riddleCount < MAX_RIDDLES) {
        // Skip empty lines
        if (line.size() == 0) continue;

        // Find the '|' between question and answer
        int p = line.find('|');
        if (p == -1) continue;  // If no '|' found, skip the line

        // Create a new Riddle struct
        Riddle r;
        r.question = line.substr(0, p);    // text before '|'
        r.answer   = line.substr(p + 1);   // text after '|'

        // Store it and increment riddle count
        _riddles[_riddleCount] = r;
        _riddleCount++;
    }

    // Close the file
    fin.close();
}

/*
 * chooseCharacters:
 * -----------------
 * Allows both players to pick which scientist character they want from the list
 * of loaded characters.
 *
 * Player 1 chooses first, then Player 2 chooses from the remaining options.
 * The chosen characters are stored in _players[0] and _players[1].
 */
void Game::chooseCharacters() {
    // If we don't have at least 2 characters loaded, just default to the first two
    if (_characterCount < 2) {
        cout << "Not enough characters in file. Using first two by default.\n";
        _players[0] = _characterOptions[0];
        _players[1] = _characterOptions[1];
        return;
    }

    // Display all available scientists with their stats
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

    int choice1 = 0;  // Player 1's choice index (1-based)
    int choice2 = 0;  // Player 2's choice index (1-based)

    // Prompt Player 1 for a choice
    cout << "\nPlayer 1: choose your scientist (1-" << _characterCount << "): ";
    cin >> choice1;

    // Validate Player 1's input
    while (choice1 < 1 || choice1 > _characterCount) {
        cout << "Invalid choice. Try again: ";
        cin >> choice1;
    }

    // Prompt Player 2 for a choice, which must be different from Player 1's
    cout << "Player 2: choose your scientist (1-" << _characterCount
         << "), but not " << choice1 << ": ";
    cin >> choice2;

    // Validate Player 2's input
    while (choice2 < 1 || choice2 > _characterCount || choice2 == choice1) {
        cout << "Invalid choice. Try again: ";
        cin >> choice2;
    }

    // Copy the chosen characters into _players array
    _players[0] = _characterOptions[choice1 - 1];  // Player 1's character
    _players[1] = _characterOptions[choice2 - 1];  // Player 2's character

    // Show final character selections
    cout << "\nPlayer 1 chose: " << _players[0].getName() << endl;
    cout << "Player 2 chose: " << _players[1].getName() << endl;
}

/*
 * choosePaths:
 * ------------
 * Each player chooses one of two paths:
 *  - 0: Training Fellowship
 *  - 1: Direct Lab Assignment
 *
 * After choices are made, applyPathBonuses() is called to adjust stats/points.
 */
void Game::choosePaths() {
    int choice;  // Will store each player's path choice

    // Repeat for both players (index 0 and 1)
    for (int i = 0; i < 2; i++) {
        cout << "\nPath selection for Player " << (i + 1)
             << " (" << _players[i].getName() << ")\n";
        cout << "0 = Training Fellowship (lower starting DP, higher stats)\n";
        cout << "1 = Direct Lab Assignment (more starting DP, smaller stat boost)\n";
        cout << "Your choice: ";
        cin >> choice;

        // Validate input; must be 0 or 1
        while (choice != 0 && choice != 1) {
            cout << "Invalid choice. Enter 0 or 1: ";
            cin >> choice;
        }

        // Store the path type in the Player object
        _players[i].setPathType(choice);
    }

    // After both players have chosen, adjust their stats/points accordingly
    applyPathBonuses();
}

/*
 * applyPathBonuses:
 * -----------------
 * Adjusts players' stats and Discover Points based on their chosen path:
 *
 * For Training Fellowship (pathType == 0):
 *   - Discover Points: -5000
 *   - Accuracy: +500
 *   - Efficiency: +500
 *   - Insight: +1000
 *
 * For Direct Lab Assignment (pathType == 1):
 *   - Discover Points: +5000
 *   - Accuracy: +200
 *   - Efficiency: +200
 *   - Insight: +200
 *
 * This reflects the trade-off between starting points and boosted stats.
 */
void Game::applyPathBonuses() {
    // Loop over both players
    for (int i = 0; i < 2; i++) {
        // Check which path the current player chose
        if (_players[i].getPathType() == 0) {
            // Training Fellowship path adjustments
            _players[i].changeDiscoverPoints(-5000);
            _players[i].changeAccuracy(500);
            _players[i].changeEfficiency(500);
            _players[i].changeInsight(1000);

            cout << _players[i].getName()
                 << " takes Training Fellowship: -5000 DP, big stat boosts.\n";
        } else {
            // Direct Lab Assignment path adjustments
            _players[i].changeDiscoverPoints(5000);
            _players[i].changeAccuracy(200);
            _players[i].changeEfficiency(200);
            _players[i].changeInsight(200);

            cout << _players[i].getName()
                 << " goes Direct Lab Assignment: +5000 DP, modest stat boosts.\n";
        }
    }
}

/*
 * play:
 * -----
 * Main gameplay loop:
 *  - Initialize the Board and show it.
 *  - Alternate turns between Player 1 and Player 2.
 *  - On each turn:
 *      1) The Board moves the player forward (Board::movePlayer).
 *      2) The updated Board is displayed.
 *      3) We get the tile color the player landed on.
 *      4) If the final tile was reached, mark player as finished.
 *      5) Otherwise, call resolveTileEffect() to trigger tile-specific logic.
 *
 * The loop ends when BOTH players have reached the final tile.
 * Then announceWinner() is called to compute scores and declare the winner.
 */
void Game::play() {
    // Initialize the board (tiles, starting positions, etc.)
    _board.initializeBoard();

    // Track whether each player has finished the race to the final tile
    bool finished[2] = {false, false};

    cout << "\n===== BEGIN JOURNEY THROUGH THE GENOME =====\n";
    // Show the starting Board layout
    _board.displayBoard();

    // Keep looping until both players are finished
    while (!(finished[0] && finished[1])) {
        // Loop over each player index: i = 0 (Player 1), i = 1 (Player 2)
        for (int i = 0; i < 2; i++) {
            // If this player is already finished, skip their turn
            if (finished[i]) {
                continue;
            }

            cout << "\n--- Player " << (i + 1)
                 << " (" << _players[i].getName() << ") turn ---" << endl;
            cout << "Rolling and moving...\n";

            // Move the player; Board decides how far and returns true if final tile reached
            bool reachedEnd = _board.movePlayer(i);

            // Display the updated board
            _board.displayBoard();

            // Get this player's current position index on their lane
            int pos = _board.getPlayerPosition(i);

            // Get the color of the tile at that position for this player
            char color = _board.getTileColor(i, pos);

            // If the Board says final tile was reached
            if (reachedEnd) {
                cout << "Player " << (i + 1)
                     << " reached the Genome Conference (final tile)!\n";
                finished[i] = true;           // Mark this player as finished
                _players[i].setFinished(true); // Record in Player object as well
            } else {
                // If not at the end, we apply tile-specific effect
                resolveTileEffect(i, color);
            }
        }
    }

    // Once both players are done, announce the result
    cout << "\nBoth players have reached the final tile!\n";
    announceWinner();
}

/*
 * resolveTileEffect:
 * ------------------
 * Given a player index and the color of the tile they landed on, this function
 * determines what should happen.
 *
 * Tile color meanings in this game:
 *  - 'G' = Green: Regular tile, with a 50% chance of a random event.
 *  - 'B' = Blue:  DNA Task 1 (strandSimilarity for equal-length strands).
 *  - 'P' = Pink:  DNA Task 2 (bestStrandMatch for unequal-length strands).
 *  - 'R' = Red:   DNA Task 3 (identifyMutations).
 *  - 'T' = Brown: DNA Task 4 (transcribeDNAtoRNA).
 *  - 'U' = Purple: Riddle tile.
 *  - Others: no special effect.
 *
 * Since we are not using rand(), we simulate a "50% chance" for Green tiles
 * by toggling between event/no-event using a static int called greenToggle.
 */
void Game::resolveTileEffect(int player_index, char color) {
    // Static variable keeps its value between calls.
    // It starts at 0 by default the first time the function is called.
    static int greenToggle = 0;

    switch (color) {
        case 'G':
            cout << "Green tile: Regular tile. 50% chance of random event (toggled).\n";
            // If greenToggle is 0, we trigger an event; next time it will be 1, and no event
            if (greenToggle == 0) {
                triggerRandomEvent(player_index);
                greenToggle = 1;   // Next Green tile will do "no event"
            } else {
                cout << "No event this time.\n";
                greenToggle = 0;   // Flip back for next time
            }
            break;

        case 'B':  // Blue tile → DNA Task 1
        case 'P':  // Pink tile → DNA Task 2
        case 'R':  // Red tile  → DNA Task 3
        case 'T':  // Brown tile → DNA Task 4
            // Delegate DNA-related behavior to handleDNATask
            handleDNATask(player_index, color);
            break;

        case 'U':
            // Purple tile: Riddle tile
            triggerRiddle(player_index);
            break;

        default:
            // Yellow (start), Orange (end), or any unrecognized color: do nothing special
            cout << "Nothing special on this tile.\n";
            break;
    }
}

/*
 * triggerRandomEvent:
 * -------------------
 * Applies a "random" event to the specified player. Since we are not using rand(),
 * we simply cycle through the event list in order using a static index.
 *
 * Behavior:
 *  - Print the event description.
 *  - Change the player's Discover Points by dpDelta (positive or negative).
 */
void Game::triggerRandomEvent(int player_index) {
    // If we have no events loaded, we can't do anything
    if (_eventCount == 0) {
        cout << "No random events loaded.\n";
        return;
    }

    // static index that cycles: 0,1,2,...,_eventCount-1,0,1,2,...
    static int nextEventIndex = 0;

    int idx = nextEventIndex;        // Use the current index
    nextEventIndex = nextEventIndex + 1;     // Move to the next one
    if (nextEventIndex >= _eventCount) {
        nextEventIndex = 0;          // Wrap around to 0 if we reach the end
    }

    // Reference to the chosen event
    RandomEvent &e = _events[idx];

    cout << "\n--- RANDOM EVENT ---\n";
    cout << e.description << endl;

    // Show whether the player gains or loses Discover Points
    if (e.dpDelta >= 0) {
        cout << "You gain " << e.dpDelta << " Discover Points!\n";
    } else {
        cout << "You lose " << -e.dpDelta << " Discover Points...\n";
    }

    // Apply dpDelta to player's Discover Points
    _players[player_index].changeDiscoverPoints(e.dpDelta);

    // Show updated Discover Points
    cout << "New Discover Points: "
         << _players[player_index].getDiscoverPoints() << "\n";
}

/*
 * handleDNATask:
 * --------------
 * Based on the tile color:
 *  - 'B' (Blue):  DNA Task 1 → strandSimilarity (equal-length strands)
 *  - 'P' (Pink):  DNA Task 2 → bestStrandMatch (unequal-length strands)
 *  - 'R' (Red):   DNA Task 3 → identifyMutations
 *  - 'T' (Brown): DNA Task 4 → transcribeDNAtoRNA
 *
 * After performing the DNA tasks, we also grant certain stat bonuses to the player.
 */
void Game::handleDNATask(int player_index, char color) {
    string s1, s2;  // s1 and s2 will store DNA strand inputs

    cout << "\n--- DNA TASK ---\n";

    // BLUE TILE: DNA Task 1
    if (color == 'B') {
        cout << "Blue tile: DNA Task 1 - Similarity (Equal-Length)\n";
        cout << "Enter first DNA strand: ";
        cin >> s1;
        cout << "Enter second DNA strand (same length): ";
        cin >> s2;

        // strandSimilarity returns a double between 0 and 1
        double score = strandSimilarity(s1, s2);

        // Convert similarity score to an Accuracy bonus (up to +200)
        // We use a C-style cast to int to avoid static_cast
        int bonus = (int)(score * 200.0);
        _players[player_index].changeAccuracy(bonus);

        cout << "Accuracy increased by " << bonus << " points.\n";
    }
    // PINK TILE: DNA Task 2
    else if (color == 'P') {
        cout << "Pink tile: DNA Task 2 - Best Strand Match (Unequal-Length)\n";
        cout << "Enter input strand: ";
        cin >> s1;
        cout << "Enter target strand: ";
        cin >> s2;

        // bestStrandMatch returns the index of the best matching substring
        int idx = bestStrandMatch(s1, s2);

        // If idx is not -1, we assume the operation succeeded and reward Efficiency
        if (idx != -1) {
            _players[player_index].changeEfficiency(150);
            cout << "Efficiency increased by 150 points.\n";
        }
    }
    // RED TILE: DNA Task 3
    else if (color == 'R') {
        cout << "Red tile: DNA Task 3 - Mutation Identification\n";
        cout << "Enter input strand: ";
        cin >> s1;
        cout << "Enter target strand: ";
        cin >> s2;

        // identifyMutations prints information about differences between strands
        identifyMutations(s1, s2);

        _players[player_index].changeInsight(150);
        cout << "Insight increased by 150 points.\n";
    }
    // BROWN TILE: DNA Task 4
    else if (color == 'T') {
        cout << "Brown tile: DNA Task 4 - Transcribe DNA to RNA\n";
        cout << "Enter DNA strand: ";
        cin >> s1;

        // transcribeDNAtoRNA prints the RNA sequence (T → U)
        transcribeDNAtoRNA(s1);

        // Small boosts to Accuracy and Efficiency
        _players[player_index].changeAccuracy(50);
        _players[player_index].changeEfficiency(50);

        cout << "Accuracy and Efficiency increased by 50 points each.\n";
    }

    // Show updated stats after completing the DNA task
    cout << "Current stats - Accuracy: " << _players[player_index].getAccuracy()
         << ", Efficiency: " << _players[player_index].getEfficiency()
         << ", Insight: " << _players[player_index].getInsight() << "\n";
}

/*
 * triggerRiddle:
 * --------------
 * Trigger a riddle tile for the player. Since we are not using rand(),
 * we cycle through the riddles in order using a static index.
 *
 * Steps:
 *  - Pick the next riddle from the list.
 *  - Print the question.
 *  - Read the player's answer using getline (can include spaces).
 *  - Convert both the answer and correct text to lowercase.
 *  - If they match exactly, the player gains +500 Insight.
 */
void Game::triggerRiddle(int player_index) {
    // If there are no riddles, we cannot do anything
    if (_riddleCount == 0) {
        cout << "No riddles loaded.\n";
        return;
    }

    // static index cycles through riddle list
    static int nextRiddleIndex = 0;

    int idx = nextRiddleIndex;        // Current riddle index
    nextRiddleIndex = nextRiddleIndex + 1;
    if (nextRiddleIndex >= _riddleCount) {
        nextRiddleIndex = 0;          // Wrap around when reaching the end
    }

    // Reference a riddle from the array
    Riddle &r = _riddles[idx];

    cout << "\n--- RIDDLE TILE ---\n";
    cout << r.question << endl;
    cout << "Your answer: ";

    string answer;

    // Clear leftover newline in input buffer once
    // so that getline reads the user's full answer correctly.
    cin.ignore(1, '\n');

    // Read a full line for the player's answer (can include spaces)
    getline(cin, answer);

    // Convert both player's answer and correct answer to lowercase
    string userAns = toLowerCaseSimple(answer);
    string correct = toLowerCaseSimple(r.answer);

    // Compare the two lowercase strings
    if (userAns == correct) {
        cout << "Correct! Insight +500.\n";
        _players[player_index].changeInsight(500);
    } else {
        cout << "Incorrect. The correct answer was: " << r.answer << endl;
    }

    cout << "Insight is now " << _players[player_index].getInsight() << "\n";
}

/*
 * calculateFinalScore:
 * --------------------
 * Computes a player's final score using the rules:
 *
 *  finalScore =
 *      current Discover Points
 *    + (Accuracy / 100)  * 1000
 *    + (Efficiency / 100) * 1000
 *    + (Insight / 100)    * 1000
 *
 * We use integer division by 100, so only full sets of 100 in each stat count.
 */
int Game::calculateFinalScore(const Player& p) const {
    // Start with the player's Discover Points
    int total = p.getDiscoverPoints();

    // For every full 100 points of Accuracy, add 1000 to total
    total += (p.getAccuracy()   / 100) * 1000;
    // For every full 100 points of Efficiency, add 1000 to total
    total += (p.getEfficiency() / 100) * 1000;
    // For every full 100 points of Insight, add 1000 to total
    total += (p.getInsight()    / 100) * 1000;

    return total;
}

/*
 * announceWinner:
 * ---------------
 * Calls calculateFinalScore for both players, prints their scores,
 * and announces whether Player 1 wins, Player 2 wins, or if it's a tie.
 */
void Game::announceWinner() const {
    // Compute final scores using the formula in calculateFinalScore
    int score1 = calculateFinalScore(_players[0]);
    int score2 = calculateFinalScore(_players[1]);

    cout << "\n===== FINAL SCORES =====\n";
    cout << _players[0].getName() << ": " << score1 << " total points\n";
    cout << _players[1].getName() << ": " << score2 << " total points\n";

    // Compare scores and announce result
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

/*
 * run:
 * ----
 * High-level "driver" function for the Game class.
 *
 * Steps:
 *  1) Load characters, random events, and riddles from their files.
 *  2) Let players choose characters.
 *  3) Let players choose their paths (Training or Direct Lab).
 *  4) Run the main gameplay loop via play().
 */
void Game::run() {
    // 1) Load all data files needed
    loadCharactersFromFile("characters.txt");
    loadRandomEvents("random_events.txt");
    loadRiddles("riddles.txt");

    // 2) Allow each player to choose a scientist character
    chooseCharacters();

    // 3) Allow each player to choose their path
    choosePaths();

    // 4) Run the main game loop
    play();
}
