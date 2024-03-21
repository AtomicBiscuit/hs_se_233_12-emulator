#pragma once

#include "parser.h"

class Preprocessor {
public:
    Preprocessor() = default;

    auto get_program() {
        return parser_.get_program();
    }

    void build(const std::string &file_name, const std::string &output_file_name) {
        std::vector<std::tuple<BaseCommand &, std::string>> program;
        parser_.parse(file_name);
        program = parser_.get_program();

        int line = 0;
        for (auto [command, param]: program) {
            try {
                command.configure(param, line);
            } catch (InvalidArgumentException &e) {
                std::cerr << "Error in line " << line + 1 << ": " << e.what() << std::endl;
                exit(1);
            } catch (UniqueException &e) {
                std::cerr << "Error in line " << line + 1 << ": " << e.what() << std::endl;
                exit(1);
            }
            line++;
        }
        clear();
        save(output_file_name);
    }

    void load(const std::string &file_name) {
        parser_.parse_binary(file_name);
        auto program = parser_.get_program();
        int line = 0;
        for (auto [command, param]: program) {
            command.configure(param, line++);
        }
    }

    static void clear() {
        for (auto [name, comma]: command_by_name) {
            comma.clear();
        }
        RegisterType::clear_all();
        LabelType::clear_all();
    }

private:

    void save(std::string file_name) {
        file_name += ".emu";
        auto program = parser_.get_raw_program();
        std::ofstream file(file_name, std::ios::binary | std::ios::out);
        if (not file.is_open()) {
            std::cerr << "Can not create file \"" + file_name + "\"" << std::endl;
            exit(1);
        }
        for (auto [comma_id, param]: program) {
            file << static_cast<uint8_t>(comma_id) << param << '\0';
        }
    }

    Parser parser_;
};