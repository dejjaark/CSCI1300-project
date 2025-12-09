#include "DNA.h"
#include <iostream>
#include <string>

using namespace std;

 double strandSimilarity(string strand1, string strand2){
    if(strand1.length()!= strand2.length() || strand1.length() == 0){
        cout << "Strands must be same length." << endl;
        return 0.0;
    }

    int matches = 0;
    for(int i = 0; i < strand1.length(); i++){
        if(strand1[i] == strand2[i]) {
            matches++;
        }
    }

    double score = matches / (double)strand1.length();
    cout << "Similarity score: " << score << endl;
    return score;
}

int strandMatch(string input_strand, string target_strand){

    if(input_strand.length() == 0 || target_strand.length() == 0){
        cout << "Strands must not be 0" << endl;
        return -1;
    }
    if(input_strand.length() < target_strand.length()){
        cout << "Target strand can't be longer than input strand" << endl;
        return -1;
    }

    double bestScore = 1.0;
    int bestIndex = -1;
    int maxStart = input_strand.length() - target_strand.length();

    for(int i = 0; i <= maxStart; i++){
        int matches = 0;
        for(int j = 0; j < target_strand.length(); j++){
            if(input_strand[i+j] == target_strand[j]){
                matches++;
            }
        }
        double score = matches / (double)target_strand.length();
        if(i > bestScore){
            bestScore = score;
            bestIndex = i;
        }
    }
    cout << "Best match starts at Index " << bestIndex << " with similarity" << bestScore << endl;
    return bestIndex;

}

void mutations(string input_strand, string target_strand){
    int i = 0;
    int j = 0;
    while(i < input_strand.length() && j < target_strand.length()){
        if(input_strand[i] == target_strand[j]){
            i++;
            j++;
        } else {
            cout << " There was a substitution at " << i << ": " << target_strand[j] << "---" << input_strand[i] << endl;
            i++;
            j++;
        }

    }
    while (i < input_strand.length()) {
        cout << "Insertion at position " << i
             << ": extra base '" << input_strand[i] << "' in input strand.\n";
        i++;
    }

    while (j < target_strand.length()) {
        cout << "Deletion at position " << j
             << ": missing base '" << target_strand[j] << "' from input strand.\n";
        j++;
    }  
}
void transcribe(string strand) {
    cout << "RNA sequence: ";
    for (int i = 0; i < strand.length(); i++) {
        char base = strand[i];
        if (base == 'T') {
            base = 'U';
        }
        cout << base;
    }
    cout << endl;
}
