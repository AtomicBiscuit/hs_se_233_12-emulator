#include <gtest/gtest.h>
#include <parser.h>

TEST(Parser, test_factorial_cycle) {
    Parser parser("./../data.factor_cycle.txt");
    std::vector<std::tuple<BaseCommand &, std::string>> ans = {
            {Label::instance(), ""}
    };
    // EXPECT_(parser.get_test_program(), ans);
}