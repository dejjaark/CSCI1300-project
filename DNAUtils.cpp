#include "DNAUtils.h"
#include <iostream>

using namespace std;

void countNucleotides(string strand) {
    int countA = 0, countC = 0, countG = 0, countT = 0;

    for (int i = 0; i < strand.length(); i++) {
        char base = strand[i];
        if (base == 'A') countA++;
        else if (base == 'C') countC++;
        else if (base == 'G') countG++;
        else if (base == 'T') countT++;
    }

    cout << "A: " << countA << ", C: " << countC
         << ", G: " << countG << ", T: " << countT << endl;
}

double calculateGCContent(string strand) {
    int gcCount = 0;

    for (int i = 0; i < strand.length(); i++) {
        char base = strand[i];
        if (base == 'G' || base == 'C') {
            gcCount++;
        }
    }

    if (strand.length() == 0) {
        return 0.0;
    }

    double gcContent = (gcCount * 100.0) / strand.length();
    cout << "GC content: " << gcContent << "%" << endl;
    return gcContent;
}

void compareDNASequences(string input_strand, string target_strand) {
    int minLen = input_strand.length();
    if (target_strand.length() < minLen) {
        minLen = target_strand.length();
    }

    int matches = 0;
    for (int i = 0; i < minLen; i++) {
        if (input_strand[i] == target_strand[i]) {
            matches++;
        }
    }

    cout << "Matched bases: " << matches << " out of " << minLen << endl;
}

void identifyMutations(string input_strand, string target_strand) {
    int minLen = input_strand.length();
    if (target_strand.length() < minLen) {
        minLen = target_strand.length();
    }

    for (int i = 0; i < minLen; i++) {
        if (input_strand[i] != target_strand[i]) {
            cout << "Mutation at position " << i
                 << ": " << target_strand[i]
                 << " -> " << input_strand[i] << endl;
        }
    }

    if (input_strand.length() > target_strand.length()) {
        cout << "Extra bases in input strand starting at index "
             << target_strand.length() << endl;
    } else if (target_strand.length() > input_strand.length()) {
        cout << "Missing bases in input strand starting at index "
             << input_strand.length() << endl;
    }
}

void transcribeDNAtoRNA(string strand) {
    for (int i = 0; i < strand.length(); i++) {
        char base = strand[i];
        if (base == 'T') {
            base = 'U';
        }
        cout << base;
    }
    cout << endl;
}
