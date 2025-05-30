#include <iostream>
#include <vector>

class DynamicBuffer {
 private:
  std::vector<char> buffer;

 public:
  // Constructor with initial capacity
  DynamicBuffer(size_t initial_size = 0) : buffer(initial_size) {}

  // Resize the buffer
  void resize(size_t new_size) { buffer.resize(new_size); }

  // Reserve capacity without changing size
  void reserve(size_t capacity) { buffer.reserve(capacity); }

  // Add data to buffer
  void append(const char* data, size_t length) {
    size_t old_size = buffer.size();
    buffer.resize(old_size + length);
    std::copy(data, data + length, buffer.data() + old_size);
  }

  // Get raw pointer to data
  char* data() { return buffer.data(); }
  const char* data() const { return buffer.data(); }

  // Get size and capacity
  size_t size() const { return buffer.size(); }
  size_t capacity() const { return buffer.capacity(); }

  // Access operators
  char& operator[](size_t index) { return buffer[index]; }
  const char& operator[](size_t index) const { return buffer[index]; }

  // Clear buffer
  void clear() { buffer.clear(); }

  // Check if empty
  bool empty() const { return buffer.empty(); }
};