#include <iostream>
#include "stack.h"
#include "parser.h"

int main() {
    stack::Stack<int> a;
    for (int i = 0; i < 20; i++) {
        a.push(i);
    }
    while (not a.empty()) {
        std::cout << a.top() << " ";
        a.pop();
    }
    std::cout << a.size() << std::endl;
    return 0;
}