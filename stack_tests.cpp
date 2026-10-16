#include "BrowserHistory.h"
#include <iostream>
#include <stdexcept>
#include <string>
static void check(bool condition, const char* description) {
    if (!condition) throw std::runtime_error(description);
}
int main() {
    try {
        ArrayStack s(2);
        check(s.empty() && s.size() == 0 && !s.full(), "new stack");
        check(!s.pop(), "empty pop");
        bool threw = false;
        try { (void)s.top(); } catch (const std::out_of_range&) { threw = true; }
        check(threw, "empty top must throw out_of_range");
        check(s.push("A") && s.push("B"), "push");
        check(s.full() && !s.push("C") && s.top() == "B", "full stack unchanged");
        check(s.pop() && s.top() == "A", "LIFO");
        s.clear(); check(s.empty() && s.push("D"), "clear and reuse");
        ArrayStack one(1);
        for (int i = 0; i < 10000; ++i) {
            check(one.push(std::to_string(i)), "reuse push");
            check(one.top() == std::to_string(i) && one.pop(), "reuse pop");
        }
        BrowserHistory h(2);
        check(h.current() == "lab://home" && !h.back(), "initial browser");
        check(h.visit("lab://arrays") && h.visit("lab://stacks"), "visits");
        check(!h.visit("lab://queues") && h.current() == "lab://stacks", "full visit unchanged");
        check(h.back() && h.current() == "lab://arrays", "back");
        check(h.visit("lab://queues") && h.back() && h.current() == "lab://arrays", "branch after back");
        check(h.back() && h.current() == "lab://home" && !h.back(), "home and empty");
        check(h.visit("lab://home") && h.previous().size() == 1, "duplicate visit");
        h.reset(); check(h.current() == "lab://home" && h.previous().empty() && h.next().empty(), "reset both stacks");
        check(!h.forward(), "initial forward empty");
        check(h.visit("A") && h.visit("B") && !h.visit("C"), "history capacity");
        check(h.back() && h.current()=="A" && h.next().top()=="B", "Back pushes current to forward stack");
        check(h.back() && h.current()=="lab://home" && h.next().size()==2, "Back twice");
        check(!h.back() && h.next().size()==2, "empty Back preserves forward");
        check(h.forward() && h.current()=="A" && h.next().top()=="B", "Forward restores nearest page");
        check(h.forward() && h.current()=="B" && h.next().empty(), "Forward twice");
        check(!h.forward() && h.previous().size()==2, "empty Forward unchanged");
        check(h.back() && h.visit("C") && h.next().empty() && !h.forward(), "new Visit clears forward branch");
        check(h.back() && h.current()=="A", "Back after branching");
        h.reset(); check(h.previous().empty() && h.next().empty(), "Reset with populated stacks");
        BrowserHistory small(1);
        check(small.visit("A"), "capacity one Visit");
        for(int i=0;i<1000;++i) { check(small.back() && small.forward(), "two stacks reuse capacity one"); }
        check(!small.visit("B") && small.current()=="A", "full Visit unchanged");
        std::cout << "All public tests passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "TEST FAILED: " << e.what() << '\n'; return 1;
    }
}
