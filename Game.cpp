#include "Game.h"
#include "DNA.h"
#include "Board.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Game::Game(){

    _eventCount = 0;
    _riddleCount = 0;
    _characterCount = 0;
}

void Game::characterFile(const char filename[]){

    ifstream myFile(filename);

    if(myFile.fail()){
        cout << "Character file couldn't be opened" << endl;
        return;
    }

    _characterCount = 0;
    string line;

    getline(myFile, line);

    while(getline(myFile, line) && _characterCount < maxCharacter){
        if (line.size() == 0) continue;

        // Find the positions of the '|' separators
        int pos1 = line.find('|');
        int pos2 = line.find('|', pos1 + 1);
        int pos3 = line.find('|', pos2 + 1);
        int pos4 = line.find('|', pos3 + 1);
        int pos5 = line.find('|', pos4 + 1);

        string name = line.substr(0, pos1);

        int exp = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));  // experience
        int acc = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));  // accuracy
        int eff = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));  // efficiency
        int ins = stoi(line.substr(pos4 + 1, pos5 - pos4 - 1));  // insight
        int dp  = stoi(line.substr(pos5 + 1));                   // discoveryPoints

         _characterOptions[_characterCount] = Player(name, exp, acc, eff, ins, dp, 0);

        // Move to the next slot in the _characterOptions array
        _characterCount++;
    }

    // Always close the file when finished
    myFile.close();

}

void Game::randomEventFile(const char filename[]){

    ifstream myFile(filename);

    if(myFile.fail()){
        cout << "Couldn't open event file." << endl;
        return;
    }
    _eventCount = 0;

    string line;

    getline(myFile, line);

    while(getline(myFile, line) && _eventCount < maxEvent){

        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        // If the line doesn't have the right number of separators, skip it
        if (p1 == -1 || p2 == -1 || p3 == -1) continue;

        // Create a new RandomEvent struct and fill it from the line
        RandomEvent e;
        e.description = line.substr(0, p1);
        e.pathType = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        e.advisor = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        e.discoverPoint = stoi(line.substr(p3 + 1));

        // Store the event in the array and increment the count
        _event[_eventCount] = e;
        _eventCount++;
    }

    myFile.close();

}

void Game::riddleFile(const char filename[]){

    ifstream myFile(filename);

    if(myFile.fail()){
        cout << "Couldn't open riddle file." << endl;
        return;
    }

    string line;

    _riddleCount = 0;
    getline(myFile, line);

    while(getline(myFile,line) && _riddleCount < maxRiddle){
         int p = line.find('|');

         Riddle r;
        r.question = line.substr(0, p);    // text before '|'
        r.answer   = line.substr(p + 1);   // text after '|'

        // Store it and increment riddle count
        _riddles[_riddleCount] = r;
        _riddleCount++;

    }

    myFile.close();

}


void Game:: chooseCharacter(){

    cout << " AVAILABLE SCIENTISTS" << endl;
    for(int i = 0; i< _characterCount;i++){
        cout << (i+1) << ". " << _characterOptions[i].getName()
        << " [Exp: " << _characterOptions[i].getExperience()
        << ", Acc: " << _characterOptions[i].getAccuracy()
        << ", Eff: " << _characterOptions[i].getEfficiency()
        << ", Ins: " << _characterOptions[i].getInsight()
        << ", DP: "  << _characterOptions[i].getDiscoverPoints()
        << "]\n";
    }

    int choice1 = 0;
    int choice2 = 0;

    cout << "Player 1, Choose your scientist (1-" << _characterCount << "): ";
    cin >> choice1;


    while (choice1 < 1 || choice1 > _characterCount) {
        cout << "Invalid choice. Try again: ";
        cin >> choice1;
    }

    cout << "Player 2: choose your scientist (1-" << _characterCount
         << "), but not " << choice1 << ": " << endl;
    cin >> choice2;

    // Validate Player 2's input
    while (choice2 < 1 || choice2 > _characterCount || choice2 == choice1) {
        cout << "Invalid choice. Try again: " << endl;
        cin >> choice2;
    }

    _players[0] = _characterOptions[choice1 - 1];  // Player 1's character
    _players[1] = _characterOptions[choice2 - 1];  // Player 2's character

    // Show final character selections
    cout << "\nPlayer 1 chose: " << _players[0].getName() << endl;
    cout << "Player 2 chose: " << _players[1].getName() << endl;

}

void Game::choosePath() { 

    int choice = 0;

    for (int i = 0; i < 2; i++){
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

        _players[i].setPathType(choice);
    }

    pathBonus();

}


void Game::pathBonus(){

    for(int i = 0; i< 2; i++){
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

void Game:: dnaTask(int player_index, char color) {
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
        int idx = strandMatch(s1, s2);

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
        mutations(s1, s2);

        _players[player_index].changeInsight(150);
        cout << "Insight increased by 150 points.\n";
    }
    // BROWN TILE: DNA Task 4
    else if (color == 'T') {
        cout << "Brown tile: DNA Task 4 - Transcribe DNA to RNA\n";
        cout << "Enter DNA strand: ";
        cin >> s1;

        // transcribeDNAtoRNA prints the RNA sequence (T → U)
        transcribe(s1);

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
    RandomEvent &e = _event[idx];

    cout << "\n--- RANDOM EVENT ---\n";
    cout << e.description << endl;

    // Show whether the player gains or loses Discover Points
    if (e.discoverPoint >= 0) {
        cout << "You gain " << e.discoverPoint << " Discover Points!\n";
    } else {
        cout << "You lose " << -e.discoverPoint << " Discover Points...\n";
    }
}

void Game::triggerRiddle(int player_index){
    if(_riddleCount == 0) {
        cout << "No riddles loaded" << endl;
        return;
    }

    static int riddleIndex = 0;
    int index = riddleIndex;

    riddleIndex++;
    if(riddleIndex >= _riddleCount) {
        riddleIndex = 0;
    }
    Riddle &r = _riddles[index];

    cout << "\n--- RIDDLE TILE ---\n";
    cout << r.question << endl;
    cout << "Your answer: ";

    string answer;

    string playerAnswer = "";
    string correctAnswer = "";

    if(playerAnswer == correctAnswer){
        cout << "Correct, Insight +500 " << endl;
        _players[player_index].changeInsight(500);
    }else {cout << "Incorrect, the correct answer is: " << r.answer << endl;}
    
    cout << "Insight is now " << _players[player_index].getInsight() <<endl;

}


void Game::tileEffect(int player_index, char color){

    static int greenEvent;

    switch(color) {
        case 'G':
            cout << "Green tile: 50$ chance of random event" << endl;
            if(greenEvent == 0){
                triggerRandomEvent(player_index);
                greenEvent = 1;
            } else {
                cout << "Green tile: no event." << endl;
                greenEvent = 0;
            }
            break;
        case 'B': 
        case 'P':
        case 'R':
        case 'T':

                dnaTask(player_index, color);
                break;
        case 'U':
                triggerRiddle(player_index);
                break;
        default:
            cout << "nothing on this tile" << endl;
            break;

    }
}

int Game::finalScore(Player p) const {

    int total = p.getDiscoverPoints();

    total += (p.getAccuracy()/100)*1000;
    total += (p.getEfficiency()/100)*1000;
    total += (p.getInsight()/100)*1000;
    return total;
}

void Game::winner() const {

    int score1 = finalScore(_players[0]);
    int score2 = finalScore(_players[1]);

    cout << _players[0].getName() << ": " << score1 << " total points" << endl;
    cout << _players[1].getName() << ": " << score2 << " total points" << endl;

    if(score1 > score2){

        cout << "Winner: " << _players[0].getName();
    }else if(score1 < score2) { 
        cout << "Winner: " << _players[1].getName();
    }else { cout << "Its a tie!" << endl;}
    
}

void Game::play() { 

    _board.initializeBoard();

    bool finish[2] = {false, false};


    while(!(finish[0] &&finish[1])){
        for (int i = 0; i < 2; i++) {

         cout << "\n--- Player " << (i + 1)
                 << " (" << _players[i].getName() << ") turn ---" << endl;
            cout << "Rolling and moving...\n";


        bool endReached = _board.movePlayer(i);

        _board.displayBoard();

        int position = _board.getPlayerPosition(i);

        char color = _board.getTileColor(i, position);


        if(endReached){

            cout << "player: " << (i+1) << " Reached the finish." << endl;
            finish[i] = true;
            _players[i].setFinished(true);
        }else { tileEffect(i, color);}

    }
}

cout << "Both Players have finished." << endl;
winner();
}


void Game::run() { 

    characterFile("characters.txt");
    randomEventFile("random_events.txt");
    riddleFile("riddles.txt");


    chooseCharacter();
    choosePath();
    play();
}