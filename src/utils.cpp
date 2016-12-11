#include "utils.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <glob.h>

#include "dynamic_bitset.h"
#include "huffman.h"
#include "lz78.h"

namespace ipmt {
namespace {

const std::string kANSIRedColor = "\033[31m";
const std::string kANSIResetAll = "\033[0m";

void SplitFilename(const std::string &pathname, std::string *filename, std::string *dir) {
  size_t index = pathname.find_last_of("/\\");

  *filename = index == std::string::npos ? pathname : pathname.substr(index + 1);
  *dir = index == std::string::npos ? "" : pathname.substr(0, index);
}

std::string GetBasenameFromFilename(const std::string &filename) {
  size_t index = filename.find_last_of(".");
  return index == std::string::npos ? filename : filename.substr(0, index);
}

void WriteBitset(std::ofstream &writer, const DynamicBitset &code) {
  int quotient = code.size() / DynamicBitset::kWordSize;
  int bytes = code.size() % DynamicBitset::kWordSize > 0 ? quotient + 1 : quotient;
  writer.write(reinterpret_cast<const char*>(&bytes), sizeof(int));
  writer.write(reinterpret_cast<const char*>(code.data()), bytes);
}

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

void ReadIndexFile(const std::string &index_filename, std::string *text,
                   std::vector<int> *suffix_array) {

}

void WriteIndexFile(const std::string &pathname, const std::vector<int> &suffix_array,
                    const std::string &text, const CompressionType &type) {
  std::string filename, dir;

  SplitFilename(pathname, &filename, &dir);

  std::string index_path = dir + GetBasenameFromFilename(filename) + ".idx";
  std::ofstream writer(index_path, std::ofstream::binary);
  
  // Stringify suffix array content.
  std::string suff_array_string;
  size_t sa_size = suffix_array.size();
  //writer.write(reinterpret_cast<const char*>(&sa_size), sizeof(size_t));
  suff_array_string.append(reinterpret_cast<const char*>(&sa_size), sizeof(size_t));

  for (size_t i = 0; i < suffix_array.size(); ++i) {
    //writer.write(reinterpret_cast<const char*>(&suffix_array[i]), sizeof(int));
    suff_array_string.append(reinterpret_cast<const char*>(&suffix_array[i]), sizeof(int));
  }

  // Write which compression algorithm was used.
  if (type == CompressionType::kHuffman) {
    writer << "huffman" << std::endl;

    ipmt::DynamicBitset code;
    ipmt::CodeTable code_table;
    ipmt::HuffmanEncode(text + suff_array_string, &code, &code_table);

    // Write code table.
    short code_table_size = static_cast<short>(code_table.size());
    writer.write(reinterpret_cast<const char*>(&code_table_size), sizeof(short));
    for (auto it = code_table.begin(); it != code_table.end(); ++it) {
      writer.write(reinterpret_cast<const char*>(&it->first), sizeof(char));
      WriteBitset(writer, it->second);
    }

    // Write encoded text.
    WriteBitset(writer, code);
  } else {  // type == CompressionType::kLZ78.
    writer << "lz78" << std::endl;

    std::vector<std::pair<int, char>> code;
    ipmt::LZ78Encode(text + suff_array_string, &code);

    // Write encoded text.
    size_t code_size = code.size();
    writer.write(reinterpret_cast<const char*>(&code_size), sizeof(size_t));
    for (size_t i = 0; i < code_size; ++i) {
      writer.write(reinterpret_cast<const char*>(&code[i].first), sizeof(int));
      writer.write(reinterpret_cast<const char*>(&code[i].second), sizeof(char));
    }
  }
}

}  // namespace ipmt

