#ifndef DNA_H
#define DNA_H

#include <string>

using namespace std;

double strandSimilarity(string strand1, string strand2);
int strandMatch(string input_strand, string target_strand);
void mutations(string input_strand, string target_strand);
void transcribe(string strand);

#endif
