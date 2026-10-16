#include "ImageQueue.h"
#include <iostream>
#include <stdexcept>
#include <string>
static void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
int main() {
    try {
        ImageQueue q(3);
        check(q.empty() && q.size()==0 && !q.full() && !q.dequeue(), "initial empty");
        bool thrown=false; try { (void)q.front(); } catch(const std::out_of_range&) { thrown=true; }
        check(thrown, "empty front must throw");
        check(q.enqueue("A") && q.enqueue("B") && q.enqueue("C"), "fill");
        check(q.full() && !q.enqueue("D") && q.size()==3 && q.front()=="A", "full unchanged");
        check(q.dequeue() && q.enqueue("D"), "reuse freed capacity");
        check(q.itemAt(0)=="B" && q.itemAt(1)=="C" && q.itemAt(2)=="D", "logical FIFO order");
        thrown=false; try { (void)q.itemAt(3); } catch(const std::out_of_range&) { thrown=true; }
        check(thrown, "invalid logical index");
        for(int i=0;i<10000;++i) { check(q.dequeue() && q.enqueue(std::to_string(i)), "sustained reuse"); }
        check(q.itemAt(0)=="9997" && q.itemAt(2)=="9999", "order after reuse");
        q.clear(); check(q.empty() && q.enqueue("X") && q.front()=="X", "clear reuse");
        ImageQueue one(1);
        for(int i=0;i<100;++i) { check(one.enqueue("same") && !one.enqueue("other"), "capacity one full"); check(one.dequeue() && one.empty(), "capacity one reuse"); }
        q.clear(); check(q.enqueue("same") && q.enqueue("same") && q.size()==2, "queue permits duplicates");
        std::cout << "All public queue tests passed.\n";
    } catch(const std::exception& e) { std::cerr << "TEST FAILED: " << e.what() << '\n'; return 1; }
}
