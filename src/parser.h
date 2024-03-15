#pragma once

#include <string>
#include <fstream>
#include <map>
#include "commands.h"

class Parser {
public:
    Parser() = delete;

    explicit Parser(const std::string &);

    void parse();

    std::vector<std::tuple<BaseCommand &, std::string>> get_program();

    std::vector<std::tuple<std::string, std::string>> get_test_program() { return program_; }

private:
    std::vector<std::tuple<std::string, std::string>> program_;
    static inline std::map<std::string, BaseCommand &> commands_ = {
            {"BEGIN",  Begin::instance()},
            {"BEG",    Begin::instance()},
            {"END",    End::instance()},
            {"PUSH",   Push::instance()},
            {"POP",    Pop::instance()},
            {"PUSHR",  PushR::instance()},
            {"POPR",   PopR::instance()},
            {"ADD",    Add::instance()},
            {"SUB",    Sub::instance()},
            {"MUL",    Mul::instance()},
            {"DIV",    Div::instance()},
            {"IN",     In::instance()},
            {"OUT",    Out::instance()},
            {"LABEL",  Label::instance()},
            {"JMP",    Jump::instance()},
            {"JEQ",    JumpE::instance()},
            {"JNE",    JumpNE::instance()},
            {"JA",     JumpG::instance()},
            {"JAE",    JumpGE::instance()},
            {"JB",     JumpL::instance()},
            {"JBE",    JumpLE::instance()},
            {"CALL",   Call::instance()},
            {"RET",    Ret::instance()},
            {"/BLANK", Blank::instance()}
    };
    std::ifstream file;
};