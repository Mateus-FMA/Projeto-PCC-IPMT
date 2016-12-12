#include "utils.h"

#include <algorithm>
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
  *dir = index == std::string::npos ? "" : pathname.substr(0, index) + "/";
}

std::string GetBasenameFromFilename(const std::string &filename) {
  size_t index = filename.find_last_of(".");
  return index == std::string::npos ? filename : filename.substr(0, index);
}

DynamicBitset ReadBitset(std::ifstream &reader) {
  DynamicBitset bitset;
  int bits;
  reader.read(reinterpret_cast<char*>(&bits), sizeof(int));

  int quotient = bits / DynamicBitset::kWordSize;
  int bytes = bits % DynamicBitset::kWordSize > 0 ? quotient + 1 : quotient;
  int curr_bit = 0;

  for (int i = 0; i < bytes; ++i) {
    byte_t word;
    reader.read(reinterpret_cast<char*>(&word), sizeof(byte_t));

    for (int j = DynamicBitset::kWordSize - 1; j >= 0; --j) {
      if (curr_bit < bits) {
        bitset.PushBack(word & (1 << j));
        ++curr_bit;
      }
    }
  }

  return bitset;
}

void WriteBitset(std::ofstream &writer, const DynamicBitset &code) {
  int quotient = code.size() / DynamicBitset::kWordSize;
  int bytes = code.size() % DynamicBitset::kWordSize > 0 ? quotient + 1 : quotient;
  int bits = code.size();

  writer.write(reinterpret_cast<const char*>(&bits), sizeof(int));
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

std::vector<int> GetOccurrences(const std::string &pattern, const std::string &text,
                                const std::vector<int> &suffix_array) {
  auto leqm = [&text] (int i, const std::string &pattern) -> bool {
    return text.compare(i, pattern.size(), pattern) < 0;
  };

  auto geqm = [&text] (const std::string &pattern, int i) -> bool {
    return text.compare(i, pattern.size(), pattern) > 0;
  };

  std::vector<int> occurrences;
  auto l = lower_bound(suffix_array.begin(), suffix_array.end(), pattern, leqm);
  auto r = upper_bound(l, suffix_array.end(), pattern, geqm);
  occurrences.reserve(r - l);

  for (auto it = l; it != r; ++it) {
    occurrences.push_back(*it);
  }

  std::sort(occurrences.begin(), occurrences.end());

  return occurrences;
}

std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             size_t pattern_length) {
  std::ostringstream oss;
  size_t curr_pos = 0;
  size_t j = 0;

  while (curr_pos != std::string::npos && j < occurrences.size()) {
    size_t lf_index = text.find_first_of("\n", curr_pos);
    size_t occ = occurrences[j];

    // Print all occurrences of the specified pattern in the current line.
    if (curr_pos <= occ && occ < lf_index) {
      while (true) {
        oss << text.substr(curr_pos, occ - curr_pos) << kANSIRedColor
            << text.substr(occ, pattern_length) << kANSIResetAll;
        if (j + 1 == occurrences.size()) break;

        size_t next_occ = occurrences[++j];
        if (occ + pattern_length > next_occ || next_occ >= lf_index) break;

        curr_pos = occ + pattern_length;
        occ = next_occ;
      }

      oss << text.substr(occ + pattern_length, lf_index - (occ + pattern_length)) << std::endl;
    }
    
    curr_pos = lf_index != std::string::npos ? lf_index + 1 : lf_index;
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

int ReadIndexFile(const std::string &index_filename, std::string *text,
                   std::vector<int> *suffix_array) {
  std::ifstream reader(index_filename, std::ifstream::binary);
  if (!reader) {  // Cannot open file.
    return -1;
  }

  // Build suffix array.
  size_t suff_array_size;
  reader.read(reinterpret_cast<char*>(&suff_array_size), sizeof(size_t));
  suffix_array->reserve(suff_array_size);

  for (size_t i = 0; i < suff_array_size; ++i) {
    int suff_array_entry;
    reader.read(reinterpret_cast<char*>(&suff_array_entry), sizeof(int));
    suffix_array->push_back(suff_array_entry);
  }

  std::string compression_type;
  std::string decoded_text;
  std::getline(reader, compression_type);

  if (!compression_type.compare("huffman")) {
    // Read code table.
    ipmt::CodeTable code_table;
    size_t table_size;
    reader.read(reinterpret_cast<char*>(&table_size), sizeof(size_t));

    for (size_t i = 0; i < table_size; ++i) {
      char key;
      reader.read(&key, sizeof(char));
      DynamicBitset codeword = ReadBitset(reader);

      code_table[key] = codeword;
    }  

    DynamicBitset code = ReadBitset(reader);
    reader.close();

    // Build tree from code table and decode index file's text.
    ipmt::HuffmanHeapNode *root = ipmt::BuildTreeFromTable(code_table);
    *text = ipmt::HuffmanDecode(code, root);
    delete root;
  } else if (!compression_type.compare("lz78")) {
    // Read code size.
    size_t code_size;
    reader.read(reinterpret_cast<char*>(&code_size), sizeof(size_t));

    std::vector<std::pair<int, char>> code;
    code.reserve(code_size);

    for (size_t i = 0; i < code_size; ++i) {
      int j;
      char c;

      reader.read(reinterpret_cast<char*>(&j), sizeof(int));
      reader.read(&c, sizeof(char));

      code.push_back(std::make_pair(j, c));
    }

    // Decode text.
    *text = ipmt::LZ78Decode(code);
  } else {  // Invalid compression type.
    return -2;
  }

  return 0;
}

void WriteIndexFile(const std::string &pathname, const std::vector<int> &suffix_array,
                    const std::string &text, const CompressionType &type) {
  std::string filename, dir;

  SplitFilename(pathname, &filename, &dir);

  std::string index_path = dir + GetBasenameFromFilename(filename) + ".idx";
  std::ofstream writer(index_path, std::ofstream::binary);

  // Write suffix array content to index file.
  size_t suff_array_size = suffix_array.size();
  writer.write(reinterpret_cast<const char*>(&suff_array_size), sizeof(size_t));

  for (size_t i = 0; i < suff_array_size; ++i) {
    writer.write(reinterpret_cast<const char*>(&suffix_array[i]), sizeof(int));
  }

  // Write which compression algorithm was used.
  if (type == CompressionType::kHuffman) {
    writer << "huffman" << std::endl;

    ipmt::DynamicBitset code;
    ipmt::CodeTable code_table;
    ipmt::HuffmanEncode(text, &code, &code_table);

    // Write code table.
    size_t code_table_size = code_table.size();
    writer.write(reinterpret_cast<const char*>(&code_table_size), sizeof(size_t));

    for (auto it = code_table.begin(); it != code_table.end(); ++it) {
      writer.write(&it->first, sizeof(char));
      //std::cout << "  Writing codeword " << it->second.ToString() << std::endl;
      WriteBitset(writer, it->second);
    }

    // Write encoded text.
    WriteBitset(writer, code);
  } else {  // type == CompressionType::kLZ78.
    writer << "lz78" << std::endl;

    std::vector<std::pair<int, char>> code;
    ipmt::LZ78Encode(text, &code);

    // Write encoded text.
    size_t code_size = code.size();
    writer.write(reinterpret_cast<const char*>(&code_size), sizeof(size_t));

    for (size_t i = 0; i < code_size; ++i) {
      writer.write(reinterpret_cast<const char*>(&code[i].first), sizeof(int));
      writer.write(&code[i].second, sizeof(char));
    }
  }
}

}  // namespace ipmt

