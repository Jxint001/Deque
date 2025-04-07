#include <cstddef>
#include <deque>
#include <iostream>
#include <ostream>
#include "deque.hpp"
using std::cout, std::endl;
int main() {
    sjtu::deque<int> d;
    for(int i=0; i<10; i++)
        d.push_back(i);

    const sjtu::deque<int> c = d;
    //c = c;

    for(sjtu::deque<int>::const_iterator it = c.cbegin(); it != c.cend(); it++) {
        (*it) = 1;
        std::cout << *it << std::endl;
    }

    const sjtu::deque<int>::const_iterator it = c.cbegin();
    it = c.cend();

    return 0;
}