#include <iostream>
#include "stack.h"
#include "commands.h"

using namespace std;

int main() {
    auto &b = Begin::instance();
    BaseCommand *c = &b;
    cout << c->test();
    return 0;
}