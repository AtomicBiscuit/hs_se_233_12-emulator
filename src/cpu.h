#pragma once

#include "prep.h"

class CPUEmulator {
public:
    CPUEmulator() = delete;

    explicit CPUEmulator(const std::string &file_name) : proc(file_name) {}

    void build(const std::string &file_name) {
        proc.build(file_name);
    }

    void run() {
        proc.load();
        auto program = proc.get_program();
        int line = Begin::instance().get_line();
        auto stack = std::make_shared<CommandStack>();
        while (-1 < line && line < program.size()) {
            auto [command, param] = program[line];
            try {
                line = command.run(param, line, stack);
            } catch (InvalidArgumentException &e) {
                std::cout << "Error in line " << line << ": " << e.what() << std::endl;
            } catch (UniqueException &e) {
                std::cout << "Error in line " << line << ": " << e.what() << std::endl;
            } catch (std::runtime_error &e) {
                std::cout << "Error in line " << line << ": " << e.what() << std::endl;
            }
        }
    }

private:
    Preprocessor proc;
};