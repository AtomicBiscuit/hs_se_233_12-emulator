#include <gtest/gtest.h>
#include <cpu.h>

TEST(Emulator, test_build) {
    CPUEmulator app("./../../test/data/factor_rec.txt");
    app.build("Chai.cold");
    std::ifstream file("Chai.cold.emu");
    EXPECT_TRUE(file.is_open());
}

TEST(Emulator, test_run) {
    auto in_orig = std::cin.rdbuf();
    auto out_orig = std::cout.rdbuf();

    std::stringstream s_out;
    std::cout.rdbuf(s_out.rdbuf());

    std::stringstream s_in("10");
    std::cin.rdbuf(s_in.rdbuf());

    CPUEmulator app("Chai.cold.emu");
    app.run();
    EXPECT_EQ(s_out.str(), "Input number: 3628800\n");
    CPUEmulator::clear();

    std::cin.rdbuf(in_orig);
    std::cout.rdbuf(out_orig);
}