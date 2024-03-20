#include <gtest/gtest.h>
#include <parser.h>
#include <cpu.h>
#include "test_data.h"

TEST(Parser, test_parse) {
    Parser parser;
    parser.parse("./../../test/data/factor_cycle.txt");
    auto prog = parser.get_program();
    for (int i = 0; i < prog.size(); i++) {
        EXPECT_EQ(get<0>(prog[i]).name(), get<0>(factor_cyc[i]));
    }
    CPUEmulator::clear();
}

TEST(Parser, test_get_raw_program) {
    Parser parser;
    parser.parse("./../../test/data/factor_rec.txt");
    auto prog = parser.get_raw_program();
    for (int i = 0; i < prog.size(); i++) {
        EXPECT_EQ(get<0>(prog[i]), get<0>(factor_rec[i]));
    }
    CPUEmulator::clear();
}

TEST(Parser, test_parse_binary) {
    Parser parser;
    parser.parse_binary("./../../test/data/factor_rec.txt.emu");
    auto prog = parser.get_raw_program();
    for (int i = 0; i < prog.size(); i++) {
        EXPECT_EQ(get<0>(prog[i]), get<0>(factor_rec[i]));
    }
    CPUEmulator::clear();
}

TEST(Parser, test_parse_incorrect) {
    Parser parser;
    EXPECT_THROW(parser.parse("AAAA.dot"), std::runtime_error);
    EXPECT_THROW(parser.parse_binary("./../../test/data/factor_rec.txt.emulator"), std::runtime_error);
}

