#pragma once

#include <string>
#include <fstream>

namespace parser {
    class Parser {
    public:
        constexpr static uint32_t LINE_MAX_SIZE = 1000;

        Parser() = delete;

        explicit Parser(const std::string &);

        bool check_opened();

        void parse();

        ~Parser();

    private:
        std::ifstream file;
    };
}
