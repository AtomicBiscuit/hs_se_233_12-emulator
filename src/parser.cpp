#include <string>
#include <fstream>
#include "parser.h"

parser::Parser::Parser(const std::string &a) {
    this->file.open(a);
}

bool parser::Parser::check_opened() {
    return this->file.is_open();
}

void parser::Parser::parse() {
    if (not this->file.is_open()) {
        throw std::runtime_error("File is closed");
    }
    std::string line;
    while (not this->file.eof()) {
        std::getline(this->file, line);
    }
}
