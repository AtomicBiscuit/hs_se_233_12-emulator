#pragma once

#include "parser.h"

class Preprocessor {
public:
    Preprocessor() = delete;

    explicit Preprocessor(const std::string &file_path) : parser_(file_path) {}

    auto get_program() {
        return parser_.get_program();
    }

    void build(const std::string &file_name) {
        std::vector<std::tuple<BaseCommand &, std::string>> program;
        try {
            parser_.parse();
            program = parser_.get_program();
        } catch (InvalidArgumentException &e) {
            std::cout << e.what() << std::endl;
            exit(0);
        }
        int line = 0;
        for (auto [command, param]: program) {
            try {
                command.configure(param, line);
            } catch (InvalidArgumentException &e) {
                std::cout << InvalidArgumentException(e.what(), line + 1).what() << std::endl;
                exit(0);
            }
            line++;
        }
        save(file_name);
    }

    void load() {
        parser_.parse_binary();
        auto program = parser_.get_program();
        int line = 0;
        for (auto [command, param]: program) {
            command.configure(param, line++);
        }
    }

    void save(std::string file_name) {
        file_name += ".emu";
        auto program = parser_.get_raw_program();
        std::ofstream file(file_name, std::ios::binary | std::ios::out);
        if (not file.is_open()) {
            std::cout << "Can not create file \"" + file_name + "\"" << std::endl;
            exit(0);
        }
        for (auto [comma_id, param]: program) {
            file << static_cast<uint8_t>(comma_id) << param << '\0';
        }
    }

private:
    Parser parser_;
};