#include "DNAUtils.h"
#include <iostream>

using namespace std;

// Blue tiles: equal-length similarity
double strandSimilarity(string strand1, string strand2) {
    if (strand1.length() != strand2.length() || strand1.length() == 0) {
        cout << "Strands must be the same non-zero length.\n";
        return 0.0;
    }

    int matches = 0;
    for (int i = 0; i < strand1.length(); i++) {
        if (strand1[i] == strand2[i]) {
            matches++;
        }
    }

    double score = matches / static_cast<double>(strand1.length());
    cout << "Similarity score: " << score << endl;
    return score;
}

// Pink tiles: unequal-length best match
int bestStrandMatch(string input_strand, string target_strand) {
    if (input_strand.length() == 0 || target_strand.length() == 0) {
        cout << "Strands must be non-empty.\n";
        return -1;
    }
    if (target_strand.length() > input_strand.length()) {
        cout << "Target strand cannot be longer than input strand.\n";
        return -1;
    }

    double bestScore = -1.0;
    int bestIndex = -1;
    int maxStart = input_strand.length() - target_strand.length();

    for (int start = 0; start <= maxStart; start++) {
        int matches = 0;
        for (int j = 0; j < target_strand.length(); j++) {
            if (input_strand[start + j] == target_strand[j]) {
                matches++;
            }
        }
        double score = matches / static_cast<double>(target_strand.length());
        if (score > bestScore) {
            bestScore = score;
            bestIndex = start;
        }
    }

    cout << "Best match starts at index " << bestIndex
         << " with similarity " << bestScore << endl;
    return bestIndex;
}

// Red tiles: mutation identification (simple version)
void identifyMutations(string input_strand, string target_strand) {
    cout << "Comparing input vs target for mutations...\n";

    int i = 0;
    int j = 0;
    while (i < input_strand.length() && j < target_strand.length()) {
        if (input_strand[i] == target_strand[j]) {
            i++;
            j++;
        } else {
            // Simple heuristic: treat as substitution
            cout << "Substitution at position " << i
                 << ": " << target_strand[j] << " -> " << input_strand[i] << endl;
            i++;
            j++;
        }
    }

    // Extra bases in input_strand
    while (i < input_strand.length()) {
        cout << "Insertion at position " << i
             << ": extra base '" << input_strand[i] << "' in input strand.\n";
        i++;
    }

    // Missing bases in input_strand (extra in target)
    while (j < target_strand.length()) {
        cout << "Deletion at position " << j
             << ": missing base '" << target_strand[j] << "' from input strand.\n";
        j++;
    }
}

// Brown tiles: DNA -> RNA transcription
void transcribeDNAtoRNA(string strand) {
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
