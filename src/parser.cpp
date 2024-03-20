#include <string>
#include <sstream>
#include <utility>
#include "parser.h"
#include "commands.h"
#include "exc.h"

void Parser::parse(const std::string &file_name) {
    std::ifstream file(file_name, std::ios::in);
    if (not file.is_open()) {
        throw std::runtime_error("File is closed");
    }
    clear();
    std::string line;
    std::string command;
    std::string param;
    int line_number = 1;
    while (not file.eof()) {
        command.clear();
        param.clear();
        std::getline(file, line);
        std::stringstream line_stream(line);
        line_stream >> command;
        if (command.empty() || command.starts_with('/')) {
            command = "BLANK";
            line_stream.str("");
        } else if (command.ends_with(':')) {
            command.pop_back();
            param = std::move(command);
            command = "LABEL";
        } else {
            std::transform(command.begin(), command.end(), command.begin(), ::toupper);
            line_stream >> param;
            if (param.starts_with('/')) {
                param = "";
                line_stream.str("");
            }
        }
        std::string tmp;
        line_stream >> tmp;
        if (not tmp.empty() and not tmp.starts_with('/')) {
            throw InvalidArgumentException("Too much arguments", line_number);
        }
        line_number++;
        program_.emplace_back(command, param);
    }
}

void Parser::parse_binary(const std::string &file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::in);
    if (not file.is_open()) {
        throw std::runtime_error("File is closed");
    }
    clear();
    char param[100]{};
    uint8_t command = file.get();
    while (not file.eof()) {
        file.getline(param, 99, '\0');
        program_.emplace_back(command_name[static_cast<eCommands>(command)], param);
        command = file.get();
    }
}

std::vector<std::tuple<BaseCommand &, std::string>> Parser::get_program() {
    std::vector<std::tuple<BaseCommand &, std::string>> program{};
    program.reserve(program_.size());
    int line = 1;
    for (auto [comma, param]: program_) {
        if (not command_by_name.contains(comma)) {
            throw InvalidArgumentException("Unknown command \"" + comma + "\"", line);
        }
        program.emplace_back(command_by_name.at(comma), param);
        line++;
    }
    return program;
}

std::vector<std::tuple<eCommands, std::string>> Parser::get_raw_program() {
    std::vector<std::tuple<eCommands, std::string>> raw_program;
    auto program = get_program();
    raw_program.reserve(program.size());
    for (auto [comma, param]: program) {
        raw_program.emplace_back(comma.name(), param);
    }
    return raw_program;
}

void Parser::clear() {
    program_.clear();
}
