#ifndef IPMT_UTILS_H_
#define IPMT_UTILS_H_

#include <string>
#include <vector>

#include <getopt.h>

namespace ipmt {

typedef option Option;

void PrintHelp();
void PrintIndexModeHelp();
void PrintSearchModeHelp();

std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             int pattern_length);
std::vector<std::string> GetFilenames(const std::string &regex);

}  // namespace ipmt

#endif  // IPMT_UTILS_H_
