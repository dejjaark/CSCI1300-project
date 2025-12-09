
#ifndef DNAUTILS_H
#define DNAUTILS_H

#include <string>

double strandSimilarity(std::string strand1, std::string strand2);
int bestStrandMatch(std::string input_strand, std::string target_strand);
void identifyMutations(std::string input_strand, std::string target_strand);
void transcribeDNAtoRNA(std::string strand);

#endif
