#include "dynamic_bitset.h"

#include <algorithm>

namespace ipmt {

const int DynamicBitset::kWordSize = 8;

DynamicBitset::DynamicBitset(const DynamicBitset &bitset)
    : data_(new byte_t[bitset.capacity()]),
      capacity_(bitset.capacity()),
      size_(bitset.size()) {
  std::copy_n(bitset.data(), bitset.capacity(), data_);
}

void DynamicBitset::Resize() {
  int new_capacity = capacity_ << 1;
  byte_t *new_data = new byte_t[new_capacity];

  std::copy_n(data_, capacity_, new_data);

  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

bool DynamicBitset::operator[](int index) const {
  int word_index = index / kWordSize;
  int bit_index = index % kWordSize;

  return (data_[word_index]) & (1 << bit_index);
}

DynamicBitset& DynamicBitset::operator=(const DynamicBitset &bitset) {
  if (this != &bitset) {
    if (capacity_ < bitset.capacity()) {  // Reallocate inner container.
      delete[] data_;
      data_ = new byte_t[bitset.capacity()];
      capacity_ = bitset.capacity();
    }

    std::copy_n(bitset.data(), bitset.capacity(), data_);
    size_ = bitset.size();
  }

  return *this;
}

void DynamicBitset::Flush() {
  std::fill_n(data_, capacity_, 0);
  size_ = 0;
}

void DynamicBitset::PushBack(bool value) {
  if (size_ == kWordSize * capacity_) Resize();

  int word_index = size_ / kWordSize;
  int bit_index = size_ % kWordSize;
  ++size_;

  if (value) data_[word_index] |= 1 << bit_index;
  else data_[word_index] &= ~(1 << bit_index);
}

std::string DynamicBitset::ToString() const {
  std::string result;
  result.reserve(size_);
  
  int bit_index = 0;
  int word_index = 0;

  for (int i = 0; i < size_; ++i) {
    char masked_bit = ((data_[word_index] & (1 << bit_index)) >> bit_index);
    result.push_back(masked_bit + '0');
    ++bit_index;

    if (bit_index == kWordSize) {
      bit_index = 0;
      ++word_index;
    }    
  }

  return result;
}

}  // namespace ipmt

