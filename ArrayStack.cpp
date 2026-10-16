#include "ArrayStack.h"
#include <stdexcept>

ArrayStack::ArrayStack(std::size_t capacity)
    : data_(new std::string[capacity]), capacity_(capacity), count_(0) {}
ArrayStack::~ArrayStack() { delete[] data_; }

std::size_t ArrayStack::size() const {
    // TODO 1: Return the number of stored pages.
    return count_;
}
bool ArrayStack::empty() const {
    // TODO 2: Return true if no pages are stored; otherwise, return false.
    return count_ == 0;
}
bool ArrayStack::full() const {
    // TODO 3: Return true if the stack has reached its capacity; otherwise, return false.
    return count_ == capacity_;
}
bool ArrayStack::push(const std::string& value) {
    // TODO 4: Check capacity before modifying the stack.
    if (full()) {
        return false;
    }
    data_[count_] = value;
    count_++;
    return true;
}
bool ArrayStack::pop() {
    // TODO 5: Remove the top page and clear its stored string.
    if (empty()) {
        return false;
    }
    count_--;
    data_[count_].clear();
    return true;
}
const std::string& ArrayStack::top() const {
    // TODO 6: Inspect without removing.
    if (empty()) {
        // string return
        throw std::out_of_range("Stack is empty");
    }
    return data_[count_ - 1];
}
void ArrayStack::clear() {
    // TODO 7: Empty the stack; retain the allocated array.
    while (!empty()) {
        pop();
    }
}
const std::string& ArrayStack::itemAt(std::size_t index) const {
    if (index >= count_) throw std::out_of_range("stack index");
    return data_[index];
}
