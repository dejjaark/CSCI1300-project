#include "DNA.h"
#include <iostream>
#include <string>

using namespace std;

// Computes the fraction of matching bases between two equal-length DNA strands.
// Prints an error and returns 0.0 if lengths differ or are zero.
 double strandSimilarity(string strand1, string strand2){
    // If strands are not the same nonzero length, it's invalid
    if(strand1.length()!= strand2.length() || strand1.length() == 0){
        cout << "Strands must be same length." << endl;
        return 0.0;
    }

    int matches = 0;
    // Count how many positions have the same base
    for(int i = 0; i < strand1.length(); i++){
        if(strand1[i] == strand2[i]) {
            matches++;
        }
    }

    // Compute similarity as matches / total length
    double score = matches / (double)strand1.length();
    cout << "Similarity score: " << score << endl;
    return score;
}

// Looks for the best matching alignment of target_strand inside input_strand,
// using simple character matching. Returns best start index or -1 on error.
int strandMatch(string input_strand, string target_strand){

    // Error checks: both must be non-empty
    if(input_strand.length() == 0 || target_strand.length() == 0){
        cout << "Strands must not be 0" << endl;
        return -1;
    }
    // Target can't be longer than the input
    if(input_strand.length() < target_strand.length()){
        cout << "Target strand can't be longer than input strand" << endl;
        return -1;
    }

    // Best similarity score seen so far (1.0 is perfect)
    double bestScore = 1.0;
    // Index of best match found so far (-1 means none yet)
    int bestIndex = -1;
    // Last possible starting index for the target inside the input
    int maxStart = input_strand.length() - target_strand.length();

    // Slide the target strand along the input strand
    for(int i = 0; i <= maxStart; i++){
        int matches = 0;
        // Compare target_strand to substring of input_strand starting at i
        for(int j = 0; j < target_strand.length(); j++){
            if(input_strand[i+j] == target_strand[j]){
                matches++;
            }
        }
        // Similarity score at this alignment
        double score = matches / (double)target_strand.length();
        // BUG-LIKE BEHAVIOR NOTE:
        // The comparison currently uses `i > bestScore`, which compares an index
        // to the score (a double). This likely should be `score > bestScore`,
        // but the code logic is left unchanged as requested.
        if(i > bestScore){
            bestScore = score;
            bestIndex = i;
        }
    }
    cout << "Best match starts at Index " << bestIndex << " with similarity" << bestScore << endl;
    return bestIndex;

}

// Prints out mutations between input_strand and target_strand:
// substitutions (same length parts), then extra insertions/deletions at the end.
void mutations(string input_strand, string target_strand){
    int i = 0;
    int j = 0;
    // Walk along both strands together while both have characters left
    while(i < input_strand.length() && j < target_strand.length()){
        if(input_strand[i] == target_strand[j]){
            // Bases match; move on
            i++;
            j++;
        } else {
            // Bases differ → substitution
            cout << " There was a substitution at " << i << ": " << target_strand[j] << "---" << input_strand[i] << endl;
            i++;
            j++;
        }

    }
    // If input_strand still has extra bases, those are insertions
    while (i < input_strand.length()) {
        cout << "Insertion at position " << i
             << ": extra base '" << input_strand[i] << "' in input strand.\n";
        i++;
    }

    // If target_strand still has extra bases, those are deletions
    while (j < target_strand.length()) {
        cout << "Deletion at position " << j
             << ": missing base '" << target_strand[j] << "' from input strand.\n";
        j++;
    }  
}

// Converts a DNA strand to RNA by replacing T with U,
// then prints the resulting RNA sequence.
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
