#ifndef IPMT_LZ78_H_
#define IPMT_LZ78_H_

#include <string>
#include <vector>
#include <utility>

namespace ipmt {

std::string LZ78Decode(const std::vector<std::pair<int, char>> &code);
void LZ78Encode(const std::string &text, std::vector<std::pair<int, char>> *code);

}  // namespace ipmt

#endif  // IPMT_LZ78_H_

