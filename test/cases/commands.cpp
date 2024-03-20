#include "gtest/gtest.h"
#include "commands.h"
#include "exc.h"

shared_stack stack_ = std::make_shared<CommandStack>();

TEST(Commands, test_begin) {
    BeginCommand beg = BeginCommand();
    beg.configure("", 15);
    EXPECT_EQ(beg.run("", 15, stack_), 16);
    EXPECT_EQ(beg.get_line(), 15);

    BeginCommand beg_incorrect = BeginCommand();
    EXPECT_THROW(beg_incorrect.configure("argument", 0), InvalidArgumentException);

    EXPECT_THROW(beg.configure("", 5), UniqueException);
}

TEST(Commands, test_end) {
    EndCommand end = EndCommand();
    end.configure("", 14);
    EXPECT_EQ(end.run("", 14, stack_), -1);

    EndCommand end_incorrect = EndCommand();
    EXPECT_THROW(end_incorrect.configure("argument", 4), InvalidArgumentException);

    EXPECT_THROW(end.configure("", 5), UniqueException);
}

TEST(Commands, test_push) {
    PushCommand push = PushCommand();
    push.configure("42", 2);
    EXPECT_EQ(push.run("42", 2, stack_), 3);
    EXPECT_EQ(stack_->data.top(), 42);

    PushCommand push_incorrect = PushCommand();
    EXPECT_THROW(push_incorrect.configure("ax", 4), std::invalid_argument);
    EXPECT_THROW(push_incorrect.configure("9999999999999999", 4), InvalidArgumentException);
}

TEST(Commands, test_pop) {
    uint32_t size = stack_->data.size();
    stack_->data.push(11);
    PopCommand pop = PopCommand();
    pop.configure("", 2);
    EXPECT_EQ(pop.run("", 2, stack_), 3);
    EXPECT_EQ(size, stack_->data.size());

    PopCommand pop_incorrect = PopCommand();
    EXPECT_THROW(pop_incorrect.configure("o", 4), InvalidArgumentException);
}

TEST(Commands, test_pushr) {
    RegisterType::get("ax").value() = -777;
    PushRCommand pushr = PushRCommand();
    pushr.configure("ax", 2);
    EXPECT_EQ(pushr.run("ax", 2, stack_), 3);
    EXPECT_EQ(stack_->data.top(), -777);

    PushRCommand pushr_incorrect = PushRCommand();
    EXPECT_THROW(pushr_incorrect.configure("", 4), InvalidArgumentException);
}

TEST(Commands, test_popr) {
    uint32_t size = stack_->data.size();
    stack_->data.push(993);
    PopRCommand popr = PopRCommand();
    popr.configure("cx", 2);
    EXPECT_EQ(popr.run("cx", 2, stack_), 3);
    EXPECT_EQ(RegisterType::get("cx").value(), 993);
    EXPECT_EQ(size, stack_->data.size());

    PopRCommand popr_incorrect = PopRCommand();
    EXPECT_THROW(popr_incorrect.configure("o", 4), InvalidArgumentException);
}

TEST(Commands, test_add) {
    stack_->data.push(993);
    stack_->data.push(7);
    AddCommand add = AddCommand();
    add.configure("", 777);
    EXPECT_EQ(add.run("", 777, stack_), 778);
    EXPECT_EQ(1000, stack_->data.top());

    AddCommand add_incorrect = AddCommand();
    EXPECT_THROW(add_incorrect.configure("38", 4), InvalidArgumentException);
}

TEST(Commands, test_sub) {
    stack_->data.push(1000);
    stack_->data.push(7);
    SubCommand sub = SubCommand();
    sub.configure("", 0);
    EXPECT_EQ(sub.run("", 0, stack_), 1);
    EXPECT_EQ(993, stack_->data.top());

    SubCommand sub_incorrect = SubCommand();
    EXPECT_THROW(sub_incorrect.configure("string", 4), InvalidArgumentException);
}

TEST(Commands, test_mul) {
    stack_->data.push(33);
    stack_->data.push(-7982);
    MulCommand mul = MulCommand();
    mul.configure("", 5);
    EXPECT_EQ(mul.run("", 5, stack_), 6);
    EXPECT_EQ(-7982 * 33, stack_->data.top());

    MulCommand mul_incorrect = MulCommand();
    EXPECT_THROW(mul_incorrect.configure("AHAHAAHAH", 4), InvalidArgumentException);
}

TEST(Commands, test_div) {
    stack_->data.push(-11);
    stack_->data.push(2);
    DivCommand div = DivCommand();
    div.configure("", 5);
    EXPECT_EQ(div.run("", 5, stack_), 6);
    EXPECT_EQ(-5, stack_->data.top());

    DivCommand div_incorrect = DivCommand();
    EXPECT_THROW(div_incorrect.configure("100^Pi", 4), InvalidArgumentException);
}

TEST(Commands, test_in) {
    auto in_orig = std::cin.rdbuf();
    auto out_orig = std::cout.rdbuf();

    std::stringstream s_out("");
    std::cout.rdbuf(s_out.rdbuf());

    std::stringstream s_in("1524");
    std::cin.rdbuf(s_in.rdbuf());

    InCommand in = InCommand();
    in.configure("", 5);
    EXPECT_EQ(in.run("", 5, stack_), 6);
    EXPECT_EQ(1524, stack_->data.top());
    EXPECT_EQ("Input number: ", s_out.str());

    InCommand in_incorrect = InCommand();
    EXPECT_THROW(in_incorrect.configure("rax", 1), InvalidArgumentException);

    std::cin.rdbuf(in_orig);
    std::cout.rdbuf(out_orig);
}

TEST(Commands, test_out) {
    auto a = std::cout.rdbuf();
    std::stringstream s("");
    std::cout.rdbuf(s.rdbuf());


    stack_->data.push(7777777);
    stack_->data.push(8888888);

    OutCommand out = OutCommand();
    out.configure("", 5);
    EXPECT_EQ(out.run("", 5, stack_), 6);
    EXPECT_EQ(s.str(), "8888888\n");
    EXPECT_EQ(7777777, stack_->data.top());

    OutCommand out_incorrect = OutCommand();
    EXPECT_THROW(out_incorrect.configure("eax", 1), InvalidArgumentException);

    std::cout.rdbuf(a);
}

TEST(Commands, test_label) {
    LabelCommand label = LabelCommand();
    label.configure("sett0ings", 5);
    EXPECT_EQ(label.run("sett0ings", 5, stack_), 6);
    EXPECT_EQ(LabelType::get("sett0ings").line(), 5);
    EXPECT_EQ(LabelType::get("sett0ings").name(), "sett0ings");

    LabelCommand label_incorrect = LabelCommand();
    EXPECT_THROW(label_incorrect.configure("1_name", 1), InvalidArgumentException);
    EXPECT_THROW(label_incorrect.configure("name_", 1), InvalidArgumentException);
}

TEST(Commands, test_jump) {
    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpCommand jump = JumpCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    JumpCommand jump_incorrect = JumpCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_equal) {
    stack_->data.push(11);
    stack_->data.push(11);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpEqualCommand jump = JumpEqualCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpEqualCommand jump_incorrect = JumpEqualCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_not_equal) {
    stack_->data.push(11);
    stack_->data.push(12);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpNotEqualCommand jump = JumpNotEqualCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpNotEqualCommand jump_incorrect = JumpNotEqualCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_greater) {
    stack_->data.push(11);
    stack_->data.push(12);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpGreaterCommand jump = JumpGreaterCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpGreaterCommand jump_incorrect = JumpGreaterCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_greater_equal) {
    stack_->data.push(11);
    stack_->data.push(12);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpGreaterOrEqualCommand jump = JumpGreaterOrEqualCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(11);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpGreaterOrEqualCommand jump_incorrect = JumpGreaterOrEqualCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_less) {
    stack_->data.push(12);
    stack_->data.push(11);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpLessCommand jump = JumpLessCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpLessCommand jump_incorrect = JumpLessCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_jump_less_equal) {
    stack_->data.push(12);
    stack_->data.push(11);

    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    JumpLessOrEqualCommand jump = JumpLessOrEqualCommand();
    jump.configure("target", 15);
    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(11);

    EXPECT_EQ(jump.run("target", 15, stack_), 5);

    stack_->data.push(12);

    EXPECT_EQ(jump.run("target", 15, stack_), 16);

    stack_->data.pop();

    JumpLessOrEqualCommand jump_incorrect = JumpLessOrEqualCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_call) {
    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    CallCommand call = CallCommand();
    call.configure("target", 15);
    EXPECT_EQ(call.run("target", 15, stack_), 5);
    EXPECT_EQ(stack_->call.top(), 15);
    stack_->call.pop();

    CallCommand jump_incorrect = CallCommand();
    jump_incorrect.configure("target2", 1);
    EXPECT_THROW(jump_incorrect.run("target2", 1, stack_), InvalidArgumentException);
}

TEST(Commands, test_ret) {
    LabelCommand label = LabelCommand();
    label.configure("target", 5);
    label.run("target", 5, stack_);

    CallCommand call = CallCommand();
    call.configure("target", 15);
    call.run("target", 15, stack_);

    RetCommand ret = RetCommand();
    ret.configure("", 5);
    EXPECT_EQ(ret.run("", 5, stack_), 16);
    EXPECT_TRUE(stack_->call.empty());
    EXPECT_THROW(ret.run("target", 5, stack_), std::runtime_error);

    RetCommand ret_incorrect = RetCommand();
    EXPECT_THROW(ret_incorrect.configure("target", 1), InvalidArgumentException);
}