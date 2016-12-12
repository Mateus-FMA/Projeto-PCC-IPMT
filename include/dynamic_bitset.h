#ifndef IPMT_DYNAMIC_BITSET_H_
#define IPMT_DYNAMIC_BITSET_H_

#include <string>

namespace ipmt {

typedef unsigned char byte_t;

const int kDefaultBitsetCapacity = 1 << 3;

// This class contains only the needed operations of a bitset for this project, it does not
// implements all features of a real bitset.
class DynamicBitset {
 public:
  DynamicBitset()
      : data_(new byte_t[kDefaultBitsetCapacity]),
        capacity_(kDefaultBitsetCapacity),
        size_(0) { Flush(); }
  DynamicBitset(const DynamicBitset &bitset);
  ~DynamicBitset() { delete[] data_; }
    
  static const int kWordSize;

  bool operator[](int index) const;
  DynamicBitset& operator=(const DynamicBitset &bitset);

  void Append(byte_t word);
  void Append(const DynamicBitset &bitset);
  DynamicBitset GetSubsetFromInterval(int start, int end) const;
  void PushBack(bool value);
  byte_t ReadWord(int index) const;
  std::string ToString() const;
  
  // Accessors.
  const byte_t* data() const { return data_; }  // Returns the inner container.
  int capacity() const { return capacity_; }  // Returns the inner container capacity.
  int size() const { return size_; }  // Returns the number of elements of the bitset.
    
 private:  
  void Flush();
  void Resize();

  byte_t *data_;
  int capacity_;
  int size_;
};

}  // namespace ipmt

#endif  //IPMT_DYNAMIC_BITSET_H_

