#ifndef IPMT_DYNAMIC_BITSET_H_
#define IPMT_DYNAMIC_BITSET_H_

#include <string>

namespace ipmt {

typedef unsigned char byte_t;

const int kDefaultBitsetCapacity = 1 << 0;

// This class contains only the needed operations of a bitset for this project, it does not
// implements all features of a real bitset.
class DynamicBitset {
 public:
  DynamicBitset()
      : data_(new byte_t[kDefaultBitsetCapacity]),
        capacity_(kDefaultBitsetCapacity),
        size_(0) {}
  DynamicBitset(const DynamicBitset &bitset);
  ~DynamicBitset() { delete[] data_; }
  
  bool operator[](int index) const;
  DynamicBitset& operator=(const DynamicBitset &bitset);

  void PushBack(bool value);
  std::string ToString();
  
  // Accessors.
  const byte_t* data() const { return data_; }  // Returns the inner container.
  int capacity() const { return capacity_; }  // Returns the inner container capacity.
  int size() const { return size_; }  // Returns the number of elements of the bitset.
    
 private:
  static const int kWordSize;
  
  void Resize();

  byte_t *data_;
  int capacity_;
  int size_;
};

}  // namespace ipmt

#endif  //IPMT_DYNAMIC_BITSET_H_

