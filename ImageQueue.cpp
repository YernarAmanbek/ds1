#include "ImageQueue.h"
#include <stdexcept>

ImageQueue::ImageQueue(std::size_t capacity)
    : data_(new std::string[capacity]), capacity_(capacity), count_(0), front_(0) {}
ImageQueue::~ImageQueue() { delete[] data_; }

std::size_t ImageQueue::size() const {
    // TODO 8: Return the number of waiting images.
    return count_;
}

bool ImageQueue::empty() const {
    // TODO 9: Return true if no images are waiting; otherwise, return false.
    return count_ == 0;
}

bool ImageQueue::full() const {
    // TODO 10: Return true if the queue has reached its capacity; otherwise, return false.
    return count_ == capacity_;
}

bool ImageQueue::enqueue(const std::string& url) {
    // TODO 11: Append at the rear; preserve state when full.
    if (full()) {
        return false;
    }

    data_[count_] = url;
    count_++;
    return true;
}

bool ImageQueue::dequeue() {
    // TODO 12: Remove and clear the oldest entry; preserve state when empty.
    if (empty()) {
        return false;
    }


    for (std::size_t i = 1; i < count_; i++) {
        data_[i - 1] = data_[i];
    }


    count_--;
    data_[count_].clear();
    return true;
    
}

const std::string& ImageQueue::front() const {
    // TODO 13: Inspect the oldest entry; throw std::out_of_range when empty.
    if (empty()) {
        throw std::out_of_range("out of range"); 
    }
    return data_[0];
}

void ImageQueue::clear() {
    // TODO 14: Clear live strings and reset the queue for reuse.
    for (std::size_t i = 0; i < count_; i++) {
        data_[i].clear();
    }

    count_ = 0;
}

const std::string& ImageQueue::itemAt(std::size_t index) const {
    // TODO 15: Logical index zero is the front; throw std::out_of_range if invalid.
    if (index >= count_) {
        throw std::out_of_range("queueu out of range");
    }

    return data_[index];
}
