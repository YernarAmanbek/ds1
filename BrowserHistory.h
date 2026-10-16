#ifndef BROWSER_HISTORY_H
#define BROWSER_HISTORY_H
#include "ArrayStack.h"
#include <string>

class BrowserHistory {
public:
    explicit BrowserHistory(std::size_t capacity)
        : previous_(capacity), next_(capacity), current_("lab://home") {}
    bool visit(const std::string& url);
    bool back();
    bool forward();
    void reset() { previous_.clear(); next_.clear(); current_ = "lab://home"; }
    const std::string& current() const { return current_; }
    const ArrayStack& previous() const { return previous_; }
    const ArrayStack& next() const { return next_; }
private:
    ArrayStack previous_;
    ArrayStack next_;
    std::string current_;
};
#endif
