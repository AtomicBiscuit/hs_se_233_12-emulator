#include <iostream>
#include "cpu.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        return 0;
    }
    CPUEmulator app(argv[2]);
    if (std::string(argv[1]) == "build") {
        app.build(argv[2]);
    } else if (std::string(argv[1]) == "run") {
        app.run();
    }
    return 0;
}