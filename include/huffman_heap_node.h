#ifndef IPMT_HUFFMAN_HEAP_NODE_H_
#define IPMT_HUFFMAN_HEAP_NODE_H_

#include <iostream>

namespace ipmt {

struct HuffmanHeapNode {
 public:
  HuffmanHeapNode() : left(nullptr), right(nullptr), c(0), freq(0) {}
  HuffmanHeapNode(char c, int freq) : left(nullptr), right(nullptr), c(c), freq(freq) {}

  void PrintTree() const { PrintNode(this, 0); }
  int Height() const { return NodeHeight(this); }
  int NodesAmount() const { return NodesAmount(this); }

  HuffmanHeapNode *left;
  HuffmanHeapNode *right;
  char c;
  int freq;

 private:
  void PrintNode(const HuffmanHeapNode *root, int tabs) const {
    if (root) {
      PrintNode(root->left, tabs + 1);
      for (int i = 0; i < tabs; ++i) std::cout << "  ";
      std::cout << "Char = " << root->c << ", Freq = " << root->freq << std::endl;
      PrintNode(root->right, tabs + 1);
    }
  }

  int NodesAmount(const HuffmanHeapNode *root) const {
    if (!root) {
      return 0;
    } else if (!root->left && !root->right) {
      return 1;
    } else {
      return 1 + NodesAmount(root->left) + NodesAmount(root->right);
    }
  }

  int NodeHeight(const HuffmanHeapNode *root) const {
    if (!root) {
      return 0;
    } else if (!root->left && !root->right) {
      return 1;
    } else {
      int hl = NodeHeight(root->left);
      int hr = NodeHeight(root->right);
      int max = hl > hr ? hl : hr;

      return 1 + max;
    }
  }
};

}  // namespace ipmt

#endif  // IPMT_HUFFMAN_HEAP_NODE_H_

