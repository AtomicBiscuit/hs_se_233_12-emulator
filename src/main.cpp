#include <iostream>
#include "stack.h"

int main() {
    stack::Stack<int> a;
    for (int i = 0; i < 20; i++) {
        a.push(i);
    }
    for (int i = 0; i < 21; i++) {
        std::cout << a.pop() << std::endl;
    }
    return 0;
}