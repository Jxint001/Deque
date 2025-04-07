#include <cstddef>
#include <deque>
#include <iostream>
#include <ostream>
#include "deque.hpp"
using std::cout, std::endl;
int main() {
    sjtu::deque<int> dq;
    std::deque<int> stl;
    for (int i = 0; i < 90; ++i) {
        dq.push_back(i);
        stl.push_back(i);
    }
    auto it = dq.begin() + 45;
    auto is = stl.begin() + 45;

    for (int i = 0; i < 15; ++i) {
        int t = rand() % (dq.size() - 3);
        it = dq.begin() + t;
        is = stl.begin() + t;
        it = dq.erase(it);
        is = stl.erase(is);
        it = dq.erase(it);
        is = stl.erase(is);
    }

    cout << it - dq.begin() << " " << is-stl.begin() << endl;
    cout << dq.size() << " " << stl.size() << endl;
    
    return 0;
}