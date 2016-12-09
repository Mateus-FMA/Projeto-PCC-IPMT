#ifndef IPMT_HUFFMAN_H_
#define IPMT_HUFFMAN_H_

#include <string>
#include <unordered_map>

#include "dynamic_bitset.h"
#include "huffman_heap_node.h"

namespace ipmt {

std::string HuffmanDecode(const DynamicBitset &code, HuffmanHeapNode *root);
void HuffmanEncode(const std::string &text, DynamicBitset *code, HuffmanHeapNode *root);

}  // namespace ipmt

#endif  // IPMT_HUFFMAN_H_

