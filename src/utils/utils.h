#include <bits/stdc++.h>

namespace Utils {

template <typename T>
int indexOf(std::vector<T> v, T el) {
  auto it = find(v.begin(), v.end(), el);
  int index = it - v.begin();
  return index;
}

};  // namespace Utils