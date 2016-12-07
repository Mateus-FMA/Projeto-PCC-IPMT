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
  std::cout << "Usage: ipmt <mode> [options] pattern indexfile [indexfile ...], where: \n\n\t- \""
            << "<mode>\" specifies a feature implemented by this tool. Supported modes\n\tare"
            << " \"index\" and \"search\". To see the options supported by each mode,\n\ttype"
            << " \"ipmt <mode> -h\" or \"ipmt <mode> --help\".\n\n\t- \"pattern\" is the input"
            << " pattern to be found on text.\n\n\t- \"indexfile\" is the index which represents"
            << " the compressed text. More\n\tthan one index file may be specified on search mode."
            << " Wildcards are also\n\tsupported on this mode." << std::endl;
}

void PrintIndexModeHelp() {
  std::cout << "Index mode options:\n\n    -c --compression\tSpecifies the compression algorithm"
            << " used to build the\n\t\t\tindex file.\n    " << std::setw(16) << std::left
            << "-i --indextype" << "\tDetermines the index structure to represent the text."
            << std::endl;
}

void PrintSearchModeHelp() {
  std::cout << "Search mode options:\n\n    " << std::setw(12) << std::left << "-c --count"
            << "\tPrint only the number of occurrences of the pattern(s)\n\t\t\tin the text.\n    "
            << "-p --pattern\tIf this option is enabled, then the \"pattern\" argument\n\t\t\twill"
            << " be interpreted as a text file containing all the\n\t\t\tpatterns to be found in"
            << " the text." << std::endl;
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

