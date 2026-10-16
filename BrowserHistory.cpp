#include "BrowserHistory.h"
bool BrowserHistory::visit(const std::string& url) {
    if (!previous_.push(current_)) return false;
    current_ = url;
    next_.clear(); // A new branch discards the old forward path.
    return true;
}
bool BrowserHistory::back() {
    if (previous_.empty()) return false;
    if (!next_.push(current_)) return false;
    current_ = previous_.top();
    previous_.pop();
    return true;
}
bool BrowserHistory::forward() {
    if (next_.empty()) return false;
    if (!previous_.push(current_)) return false;
    current_ = next_.top();
    next_.pop();
    return true;
}
