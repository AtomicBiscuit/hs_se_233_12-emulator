#include <gtest/gtest.h>
#include <prep.h>
#include "test_data.h"

TEST(Preprocessor, test_build) {
    Preprocessor pre;
    pre.build("./../../test/data/factor_cycle.txt", "prep_test_build");
    std::ifstream file("prep_test_build.emu");
    EXPECT_TRUE(file.is_open());
}

TEST(Preprocessor, test_load) {
    Preprocessor pre;
    pre.load("prep_test_build.emu");
    auto prog = pre.get_program();
    for (int i = 0; i < prog.size(); i++) {
        EXPECT_EQ(get<0>(prog[i]).name(), get<0>(factor_cyc[i]));
    }
    Preprocessor::clear();
}