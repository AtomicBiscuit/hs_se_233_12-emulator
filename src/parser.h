#pragma once

#include <string>
#include <fstream>
#include <map>
#include "commands.h"

class Parser {
public:
    constexpr static uint32_t LINE_MAX_SIZE = 120;

    Parser() = delete;

    explicit Parser(const std::string &);

    bool check_opened();

    void parse();

private:
    static inline std::map<std::string, BaseCommand *> commands = {
            {"BEGIN", &Begin::instance()},
            {"END",   &End::instance()},
            {"PUSH", &Push::instance()},
            {"POP",  &Pop::instance()}
    };
    std::ifstream file;
};