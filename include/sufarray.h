#ifndef IPMT_INCLUDE_SUFARRAY_H_
#define IPMT_INCLUDE_SUFARRAY_H_

#include <string>
#include <vector>

namespace ipmt{

std::bool ltm(const std::string &a, const std::string &b, int m);

std::bool leqm(const std::string &a, const std::string &b, int m);

int succ1(const std::string txt,std::vector<int> sa, const std::string pat);

int pred1(const std::string txt,std::vector<int> sa,const std::string pat);

std::vector<int> sorti (std::vector<string> suff,std::vector<int> index);


}  // namespace pmt

#endif  // PMT_INCLUDE_SELLERS_H_

