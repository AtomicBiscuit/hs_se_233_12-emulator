#pragma once

#include <string>
#include <fstream>
#include <map>
#include "commands.h"

class Parser {
public:
    Parser() = default;

    void parse(const std::string &);

    void parse_binary(const std::string &);

    std::vector<std::tuple<BaseCommand &, std::string>> get_program();

    std::vector<std::tuple<eCommands, std::string>> get_raw_program();

    void clear();

private:
    std::vector<std::tuple<std::string, std::string>> program_;
};