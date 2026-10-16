#ifndef IMAGE_QUEUE_H
#define IMAGE_QUEUE_H
#include <cstddef>
#include <string>

// Bounded FIFO using the supplied array. Choose shifting or circular indexing.
// No STL containers. Keep the public interface; private fields may be adjusted.
class ImageQueue {
public:
    explicit ImageQueue(std::size_t capacity);
    ~ImageQueue();
    ImageQueue(const ImageQueue&) = delete;
    ImageQueue& operator=(const ImageQueue&) = delete;
    std::size_t size() const;
    bool empty() const;
    bool full() const;
    bool enqueue(const std::string& url); // full: false, unchanged
    bool dequeue();                       // empty: false, unchanged
    const std::string& front() const;      // empty: out_of_range
    void clear();                         // keep allocated array
    const std::string& itemAt(std::size_t index) const; // logical FIFO order
private:
    std::string* data_;
    std::size_t capacity_;
    std::size_t count_;
    std::size_t front_; // optional: useful for circular indexing
};
#endif
