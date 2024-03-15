#include <string>
#include <sstream>
#include "parser.h"
#include "commands.h"
#include "exc.h"

Parser::Parser(const std::string &a) {
    this->file.open(a);
}

void Parser::parse() {
    if (not this->file.is_open()) {
        throw std::runtime_error("File is closed");
    }
    std::string line;
    std::string command;
    std::string param;
    int line_number = 1;
    while (not this->file.eof()) {
        command.clear();
        param.clear();
        std::getline(this->file, line);
        std::stringstream line_stream(line);
        line_stream >> command;
        if (command.empty() || command.starts_with('/')) {
            command = "/BLANK";
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

std::vector<std::tuple<BaseCommand &, std::string>> Parser::get_program() {
    std::vector<std::tuple<BaseCommand &, std::string>> program{};
    program.reserve(program_.size());
    int line = 1;
    for (auto [comma, param]: program_) {
        if (not commands_.contains(comma)) {
            throw InvalidArgumentException("Unknown command \"" + comma + "\"", line);
        }
        program.emplace_back(commands_.at(comma), param);
        line++;
    }
    return program;
}