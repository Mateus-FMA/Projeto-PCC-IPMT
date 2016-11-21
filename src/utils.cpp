#include "utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <glob.h>

namespace ipmt {
namespace {

const std::string kANSIRedColor = "\033[31m";
const std::string kANSIResetAll = "\033[0m";

}  // namespace

void PrintHelp() {
  std::cout << "Usage: ipmt <mode> [options] pattern textfile [textfile ...]" << std::endl;
}

std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             int pattern_length) {
  std::ostringstream oss;
  int curr_pos = 0;

  for (size_t j = 0; j < occurrences.size(); ++j) {
    oss << text.substr(curr_pos, occurrences[j] - curr_pos) << kANSIRedColor
        << text.substr(occurrences[j], pattern_length) << kANSIResetAll;
    curr_pos = occurrences[j] + pattern_length;
  }

  return oss.str();
}

std::vector<std::string> GetFilenames(const std::string &pattern) {
  glob_t glob_results;
  std::vector<std::string> filenames;

  glob(pattern.c_str(), GLOB_NOSORT, nullptr, &glob_results);
  filenames.reserve(glob_results.gl_pathc);

  for (size_t i = 0; i < glob_results.gl_pathc; ++i) {
    filenames.push_back(glob_results.gl_pathv[i]);
  }

  globfree(&glob_results);

  return filenames;
}

}  // namespace ipmt

