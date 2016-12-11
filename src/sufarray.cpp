#include "sufarray.h"

#include <algorithm>

namespace ipmt {

// TODO(Mateus/Valdemir): we do not use LCP's info on this implementation, so it's not as efficient
// as it could be on search stage.
// Manber and Myers algorithm, 1991.
std::vector<int> BuildSuffixArray(const std::string &text) {
  int n = static_cast<int>(text.size());

  std::vector<int> pos(n);  // Final suffix array.
  std::vector<int> prm(n);  // prm = pos ** (-1).
  std::vector<bool> bh(n);  // bh[i] == true iff pos[i] contains the leftmost suffix of a h-bucket.

  // Auxiliar arrays.
  std::vector<int> count(n);
  std::vector<bool> b2h(n);

  // Sorting base case.
  for (int i = 0; i < n; ++i) {
    pos[i] = i;
  }

  std::sort(pos.begin(), pos.end(), [&text] (int i, int j) -> bool { return text[i] < text[j]; });

  bh[0] = true;
  for (int i = 1; i < n; ++i) {
    bh[i] = text[pos[i-1]] != text[pos[i]];
  }

  std::fill(b2h.begin(), b2h.end(), false);
  std::vector<int> next_suffix(n);  // Gets next suffix on a h-bucket.

  // Inductive step.
  for (int h = 1; h < n; h <<= 1) {
    int i = 0;
    int j = 1;
    int num_buckets = 0;

    while (i < n) {
      // Get next suffix such that text[pos[i]] !=_h text[pos[j]].
      while (j < n && !bh[j]) ++j;
      next_suffix[i] = j;
      i = j;     
      j = i + 1;
      ++num_buckets;
    }

    // If the number of buckets is n, then the suffix array is already sorted; else, continue.
    if (num_buckets == n) {
      break;
    }

    for (int i = 0; i < n; i = next_suffix[i]) {
      count[i] = 0;
      for (int c = i; c < next_suffix[i]; ++c) {
        prm[pos[c]] = i;
      }
    }

    int d = n - h;
    int e = prm[d];
    prm[d] = e + count[e];
    ++count[e];
    b2h[prm[d]] = true;

    for (int i = 0; i < n; i = next_suffix[i]) {
      for (int c = i; c < next_suffix[i]; ++c) {
        d = pos[c] - h;

        if (d >= 0) {
          e = prm[d];
          prm[d] = e + count[e];
          ++count[e];
          b2h[prm[d]] = true;
        }
      }

      for (int c = i; c < next_suffix[i]; ++c) {
        d = pos[c] - h;

        if (d >= 0 && b2h[prm[d]]) {
          for (int f = prm[d] + 1; !bh[f] && b2h[f]; ++f) {
            b2h[f] = false;
          }
        }
      }
    }
    
    for (int i = 0; i < n; ++i) {
      pos[prm[i]] = i;
      bh[i] = bh[i] || b2h[i];
    }
  }

  return pos;
}

}  // namespace ipmt

