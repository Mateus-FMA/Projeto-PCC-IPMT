#include "lz78.h"

#include <iostream>
#include <unordered_map>

namespace ipmt {

std::string LZ78Decode(const std::vector<std::pair<int, char>> &code) {
  std::string text;
  std::unordered_map<int, std::string> dict;
  int d = 1;

  dict[0] = "";

  for (size_t j = 0; j < code.size(); ++j) {
    int dict_index = code[j].first;
    char decoded_char = code[j].second;

    std::string next_entry = dict[dict_index] + decoded_char;
    dict[d++] = next_entry;
    text += next_entry;
  }

  return text;
}

void LZ78Encode(const std::string &text, std::vector<std::pair<int, char>> *code) {
  std::unordered_map<std::string, int> dict;
  int d = 1;
  std::string dict_entry;
  
  dict[""] = 0;

  for (size_t i = 0; i < text.size(); ++i) {
    auto it = dict.find(dict_entry + text[i]);

    if (it != dict.end()) {
      dict_entry += text[i];
    } else {
      code->push_back(std::make_pair(dict[dict_entry], text[i]));
      dict[dict_entry + text[i]] = d++;
      dict_entry.clear();
    }
  }
}

}  // namespace ipmt

