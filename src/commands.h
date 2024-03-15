#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>

#include "stack.h"
#include "data.h"


using shared_stack = std::shared_ptr<CommandStack>;

template<typename T>
class Singleton {
public:
    Singleton(const Singleton &) = delete;

    Singleton(Singleton &&) = delete;

    Singleton &operator=(const Singleton &) = delete;

    Singleton &operator=(Singleton &&) = delete;

    static T &instance() {
        static T instance;
        return instance;
    }

private:
    Singleton() = default;

    ~Singleton() = default;
};

class BaseCommand {
public:
    virtual int run(std::string, int) = 0;

    virtual void configure(std::string, int, shared_stack) = 0;
};

class BaseParamLessCommand : public BaseCommand {
private:
    virtual std::string name() = 0;

public:
    virtual int process(int) = 0;

    virtual void setup(int, shared_stack) = 0;

    int run(std::string, int) override;

    void configure(std::string, int, shared_stack) override;
};

class BaseIntegerCommand : public BaseCommand {
private:
    static int clear_param(std::string &, int);

public:
    virtual int process(int, int) = 0;

    virtual void setup(int, int, shared_stack) = 0;

    int run(std::string, int) override;

    void configure(std::string, int, shared_stack) override;
};

class BaseRegisterCommand : public BaseCommand {
public:
    virtual int process(RegisterType &, int) = 0;

    virtual void setup(RegisterType &, int, shared_stack) = 0;

    int run(std::string, int) override;

    void configure(std::string, int, shared_stack) override;
};

class BaseLabelCommand : public BaseCommand {
public:
    virtual int process(LabelType &, int) = 0;

    virtual void setup(LabelType &, int, shared_stack) = 0;

    int run(std::string, int) override;

    void configure(std::string, int, shared_stack) override;
};

class BeginCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "BEGIN"; }

    int line_ = -1;
public:

    int process(int) override;

    void setup(int, shared_stack) override;

    int get_line() const { return line_; }
};

class EndCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "END"; }

    int line_ = -1;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class PushCommand : public BaseIntegerCommand {
private:
    shared_stack stack_;
public:
    int process(int, int) override;

    void setup(int, int, shared_stack) override;
};

class PopCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "POP"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class PushRCommand : public BaseRegisterCommand {
private:
    shared_stack stack_;
public:
    int process(RegisterType &, int) override;

    void setup(RegisterType &, int, shared_stack) override;
};

class PopRCommand : public BaseRegisterCommand {
private:
    shared_stack stack_;
public:
    int process(RegisterType &, int) override;

    void setup(RegisterType &, int, shared_stack) override;
};

class AddCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "ADD"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class SubCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "SUB"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class MulCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "MUL"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class DivCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "DIV"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class InCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "IN"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class OutCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "OUT"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class LabelCommand : public BaseLabelCommand {
private:
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpCommand : public BaseLabelCommand {
private:
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpEqualCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpNotEqualCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpGreaterCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpGreaterOrEqualCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpLessCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class JumpLessOrEqualCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};


class CallCommand : public BaseLabelCommand {
private:
    shared_stack stack_;
public:
    int process(LabelType &, int) override;

    void setup(LabelType &, int, shared_stack) override;
};

class RetCommand : public BaseParamLessCommand {
private:
    std::string name() override { return "RET"; }

    shared_stack stack_;
public:
    int process(int) override;

    void setup(int, shared_stack) override;
};

class BlankCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string, int line) override { return line + 1; }

    void configure(std::string, int, shared_stack) override {}
};


using Begin = Singleton<BeginCommand>;
using End = Singleton<EndCommand>;
using Push = Singleton<PushCommand>;
using Pop = Singleton<PopCommand>;
using PushR = Singleton<PushRCommand>;
using PopR = Singleton<PopRCommand>;
using Add = Singleton<AddCommand>;
using Sub = Singleton<SubCommand>;
using Mul = Singleton<MulCommand>;
using Div = Singleton<DivCommand>;
using In = Singleton<InCommand>;
using Out = Singleton<OutCommand>;

using Label = Singleton<LabelCommand>;

using Jump = Singleton<JumpCommand>;
using JumpE = Singleton<JumpEqualCommand>;
using JumpNE = Singleton<JumpNotEqualCommand>;
using JumpG = Singleton<JumpGreaterCommand>;
using JumpGE = Singleton<JumpGreaterOrEqualCommand>;
using JumpL = Singleton<JumpLessCommand>;
using JumpLE = Singleton<JumpLessOrEqualCommand>;

using Call = Singleton<CallCommand>;
using Ret = Singleton<RetCommand>;

using Blank = Singleton<BlankCommand>;