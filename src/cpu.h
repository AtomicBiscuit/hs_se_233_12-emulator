#pragma once

#include "prep.h"

class CPUEmulator {
public:
    CPUEmulator() = delete;

    explicit CPUEmulator(std::string file_name) : file_name_(std::move(file_name)) {}

    void build(const std::string &output_file_name) {
        proc_.build(file_name_, output_file_name);
        clear();
    }

    void run() {
        proc_.load(file_name_);
        auto program = proc_.get_program();
        int line = Begin::instance().get_line();
        auto stack = std::make_shared<CommandStack>();
        while (-1 < line && line < program.size()) {
            auto [command, param] = program[line];
            try {
                line = command.run(param, line, stack);
            } catch (InvalidArgumentException &e) {
                std::cerr << "Error in line " << line << ": " << e.what() << std::endl;
                break;
            } catch (UniqueException &e) {
                std::cerr << "Error in line " << line << ": " << e.what() << std::endl;
                break;
            } catch (std::runtime_error &e) {
                std::cerr << "Error in line " << line << ": " << e.what() << std::endl;
                break;
            }
        }
        clear();
    }

    static void clear() {
        Preprocessor::clear();
    }


private:
    std::string file_name_;
    Preprocessor proc_;
};