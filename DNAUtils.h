
#ifndef DNAUTILS_H
#define DNAUTILS_H

#include <string>

void countNucleotides(std::string strand);
double calculateGCContent(std::string strand);
void compareDNASequences(std::string input_strand, std::string target_strand);
void identifyMutations(std::string input_strand, std::string target_strand);
void transcribeDNAtoRNA(std::string strand);


#endif //DNAUTILS_H
