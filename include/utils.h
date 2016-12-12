#ifndef IPMT_UTILS_H_
#define IPMT_UTILS_H_

#include <string>
#include <vector>

#include <getopt.h>

#include "compression_type.h"

namespace ipmt {

typedef option Option;

void PrintHelp();
void PrintIndexModeHelp();
void PrintSearchModeHelp();

std::vector<int> GetOccurrences(const std::string &pattern, const std::string &text,
                                const std::vector<int> &suffix_array);
std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             size_t pattern_length);
std::vector<std::string> GetFilenames(const std::string &regex);
int ReadIndexFile(const std::string &index_path, std::string *text,
                  std::vector<int> *suffix_array);
void WriteIndexFile(const std::string &pathname, const std::vector<int> &suffix_array,
                    const std::string &text, const CompressionType &type);

}  // namespace ipmt

#endif  // IPMT_UTILS_H_
