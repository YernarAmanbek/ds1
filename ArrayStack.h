#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H
#include <cstddef>
#include <string>

// Implement with the supplied array. Do not use STL containers.
class ArrayStack {
public:
    explicit ArrayStack(std::size_t capacity);
    ~ArrayStack();
    ArrayStack(const ArrayStack&) = delete;
    ArrayStack& operator=(const ArrayStack&) = delete;

    std::size_t size() const;
    bool empty() const;
    bool full() const;
    bool push(const std::string& value); // false if full; unchanged
    bool pop();                        // false if empty; unchanged
    const std::string& top() const;     // throw std::out_of_range if empty
    void clear();                      // empty stored entries; retain array

    // Supplied: read-only access for the visualizer, bottom at index zero.
    const std::string& itemAt(std::size_t index) const;
private:
    std::string* data_;
    std::size_t capacity_;
    std::size_t count_;
};
#endif
