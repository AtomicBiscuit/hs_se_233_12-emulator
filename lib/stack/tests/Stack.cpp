#include "gtest/gtest.h"
#include "stack.h"

using namespace stack;

TEST(StackConstructor, default_constructor) {
    Stack<std::string> a;
    EXPECT_EQ(a.size(), 0);
    EXPECT_TRUE(a.empty());
    EXPECT_THROW(a.top(), std::runtime_error);
}

TEST(StackConstructor, reserve) {
    Stack<std::string> a(0);
    Stack<std::vector<int>> b(50);
    Stack<long double> c(100000);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
    EXPECT_TRUE(c.empty());
    EXPECT_EQ(a.capacity(), 0);
    EXPECT_EQ(b.capacity(), 50);
    EXPECT_EQ(c.capacity(), 100000);
}

TEST(StackConstructor, copy) {
    Stack<std::string> a(10);
    a.push("string1");
    a.push("string2");
    a.push("string3");
    Stack<std::string> b(a);
    EXPECT_EQ(a.capacity(), 10);
    EXPECT_EQ(b.capacity(), 10);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(b.size(), 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(a.top(), "string" + std::to_string(3 - i));
        EXPECT_EQ(b.top(), "string" + std::to_string(3 - i));
        a.pop();
        b.pop();
    }
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TEST(StackConstructor, copy_empty) {
    Stack<std::string> a;
    Stack<std::string> b(a);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TEST(StackConstructor, move) {
    Stack<std::string> a(10);
    a.push("string1");
    a.push("string2");
    a.push("string3");
    Stack<std::string> b(std::move(a));
    EXPECT_EQ(a.capacity(), 0);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(b.capacity(), 10);
    EXPECT_EQ(b.size(), 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(b.top(), "string" + std::to_string(3 - i));
        b.pop();
    }
    EXPECT_TRUE(b.empty());
}

TEST(StackAssignment, copy) {
    Stack<std::vector<int>> a(10);
    a.push({1});
    a.push({4});
    a.push({9});
    Stack<std::vector<int>> b(15);
    b = a;
    EXPECT_EQ(a.capacity(), 10);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(b.capacity(), 10);
    EXPECT_EQ(b.size(), 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(a.top(), std::vector<int>{(3 - i) * (3 - i)});
        EXPECT_EQ(b.top(), std::vector<int>{(3 - i) * (3 - i)});
        b.pop();
        a.pop();
    }
    EXPECT_TRUE(b.empty());
    EXPECT_TRUE(a.empty());
}

TEST(StackAssignment, self_copy) {
    Stack<std::vector<int>> a(10);
    a.push({1});
    a.push({4});
    a.push({9});
    a = a;
    EXPECT_EQ(a.capacity(), 10);
    EXPECT_EQ(a.size(), 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(a.top(), std::vector<int>{(3 - i) * (3 - i)});
        a.pop();
    }
    EXPECT_TRUE(a.empty());
}

TEST(StackAssignment, move) {
    Stack<int *> a(10);
    int p[100000];
    a.push(nullptr);
    for (int &i: p) {
        a.push(&i);
    }
    Stack<int *> b(std::move(a));
    EXPECT_EQ(a.capacity(), 0);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(b.size(), 100001);
    for (int i = 0; i < 100001 - 1; i++) {
        EXPECT_EQ(b.top(), &p[100000 - i - 1]);
        b.pop();
        EXPECT_EQ(b.size(), 100001 - i - 1);
    }
    EXPECT_EQ(b.top(), nullptr);
    b.pop();
    EXPECT_EQ(b.size(), 0);
    EXPECT_TRUE(b.empty());
}

TEST(Stack, push_copy) {
    Stack<std::set<std::string>> a;
    for (int i = 0; i < 1530; i++) {
        std::set<std::string> b{"Abobus", std::to_string(i), std::to_string(168)};
        a.push(b);
        EXPECT_EQ(a.top(), b);
    }
    for (int i = 1529; i >= 0; i--) {
        std::set<std::string> b{std::to_string(i), "Abobus", std::to_string(168)};
        EXPECT_EQ(a.top(), b);
        a.pop();
    }
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0);
}

TEST(Stack, push_move) {
    Stack<std::set<std::string>> a;
    for (int i = 0; i < 1530; i++) {
        std::set<std::string> b{"Abobus", std::to_string(i), std::to_string(168)};
        a.push(std::move(b));
        EXPECT_EQ(b, std::set<std::string>());
    }
    for (int i = 1529; i >= 0; i--) {
        std::set<std::string> b{std::to_string(i), "Abobus", std::to_string(168)};
        EXPECT_EQ(a.top(), b);
        a.pop();
    }
    EXPECT_TRUE(a.empty());
}

TEST(Stack, pop_empty) {
    Stack<uint32_t> a;
    EXPECT_TRUE(a.empty());
    a.pop();
    EXPECT_TRUE(a.empty());
}

TEST(Stack, pop) {
    Stack<uint32_t> a;
    a.push(777);
    EXPECT_EQ(a.top(), 777);
    a.pop();
    EXPECT_TRUE(a.empty());
    EXPECT_THROW(a.top(), std::runtime_error);
}

TEST(Stack, top) {
    Stack<uint32_t> a;
    for (uint32_t i = 0; i < 10000000; i++) {
        a.push(i);
        EXPECT_EQ(a.top(), i);
    }
}

TEST(Stack, top_empty) {
    Stack<uint32_t> a;
    a.push(888888888);
    a.pop();
    EXPECT_THROW(a.top(), std::runtime_error);
}
