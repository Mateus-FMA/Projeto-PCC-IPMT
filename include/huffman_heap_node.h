#ifndef IPMT_HUFFMAN_HEAP_NODE_H_
#define IPMT_HUFFMAN_HEAP_NODE_H_

#include <iostream>

namespace ipmt {

struct HuffmanHeapNode {
 public:
  HuffmanHeapNode() : left(nullptr), right(nullptr), c(0), freq(0) {}
  HuffmanHeapNode(char c, int freq) : left(nullptr), right(nullptr), c(c), freq(freq) {}

  void PrintTree() { PrintNode(this, 0); }

  HuffmanHeapNode *left;
  HuffmanHeapNode *right;
  char c;
  int freq;

 private:
  void PrintNode(HuffmanHeapNode *root, int tabs) {
    if (root) {
      PrintNode(root->left, tabs + 1);
      for (int i = 0; i < tabs; ++i) std::cout << "  ";
      std::cout << "Char = " << root->c << ", Freq = " << root->freq << std::endl;
      PrintNode(root->right, tabs + 1);
    }
  }
};

}  // namespace ipmt

#endif  // IPMT_HUFFMAN_HEAP_NODE_H_

