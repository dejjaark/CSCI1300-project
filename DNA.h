#ifndef DNA_H
#define DNA_H

#include <string>

using namespace std;

// Computes similarity between two equal-length DNA strands
// Returns a double between 0.0 and 1.0 and prints the score
double strandSimilarity(string strand1, string strand2);

// Finds the best alignment of target_strand within input_strand
// Returns the starting index of the best match, or -1 on error
int strandMatch(string input_strand, string target_strand);

// Compares two strands, printing substitutions/insertions/deletions
void mutations(string input_strand, string target_strand);

// Converts DNA to RNA (T -> U) and prints the RNA sequence
void transcribe(string strand);

#endif
