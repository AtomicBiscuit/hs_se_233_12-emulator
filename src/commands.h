#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <map>

#include "stack.h"
#include "data.h"


using shared_stack = std::shared_ptr<CommandStack>;

enum class eCommands {
    Begin = 0, End, Push, Pop, PushR, PopR,
    Add, Sub, Mul, Div, In, Out, Label,
    Jump, JumpE, JumpNE, JumpG, JumpGE, JumpL, JumpLE, Call, Ret, Blank
};

static std::map<eCommands, std::string> command_name{
        {eCommands::Begin,  "BEGIN"},
        {eCommands::End,    "END"},
        {eCommands::Push,   "PUSH"},
        {eCommands::Pop,    "POP"},
        {eCommands::PushR,  "PUSHR"},
        {eCommands::PopR,   "POPR"},
        {eCommands::Add,    "ADD"},
        {eCommands::Sub,    "SUB"},
        {eCommands::Mul,    "MUL"},
        {eCommands::Div,    "DIV"},
        {eCommands::In,     "IN"},
        {eCommands::Out,    "OUT"},
        {eCommands::Label,  "LABEL"},
        {eCommands::Jump,   "JMP"},
        {eCommands::JumpE,  "JEQ"},
        {eCommands::JumpNE, "JNE"},
        {eCommands::JumpG,  "JA"},
        {eCommands::JumpGE, "JAE"},
        {eCommands::JumpL,  "JB"},
        {eCommands::JumpLE, "JBE"},
        {eCommands::Call,   "CALL"},
        {eCommands::Ret,    "RET"},
        {eCommands::Blank,  "BLANK"}
};

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
    virtual eCommands name() = 0;

    virtual int run(std::string, int, shared_stack) = 0;

    virtual void configure(std::string, int) = 0;
};

class BaseParamLessCommand : public BaseCommand {
public:
    virtual int process(int, shared_stack) = 0;

    virtual void setup(int) = 0;

    int run(std::string, int, shared_stack) override;

    void configure(std::string, int) override;
};

class BaseIntegerCommand : public BaseCommand {
private:
    static int clear_param(std::string &, int);

public:
    virtual int process(int, int, shared_stack) = 0;

    virtual void setup(int, int) = 0;

    int run(std::string, int, shared_stack) override;

    void configure(std::string, int) override;
};

class BaseRegisterCommand : public BaseCommand {
public:
    virtual int process(RegisterType &, int, shared_stack) = 0;

    virtual void setup(RegisterType &, int) = 0;

    int run(std::string, int, shared_stack) override;

    void configure(std::string, int) override;
};

class BaseLabelCommand : public BaseCommand {
public:
    virtual int process(LabelType &, int, shared_stack) = 0;

    virtual void setup(LabelType &, int) = 0;

    int run(std::string, int, shared_stack) override;

    void configure(std::string, int) override;
};

class BeginCommand : public BaseParamLessCommand {
private:
    int line_ = -1;
public:
    eCommands name() override { return eCommands::Begin; };

    int process(int, shared_stack) override;

    void setup(int) override;

    [[nodiscard]] int get_line() const { return line_; }
};

class EndCommand : public BaseParamLessCommand {
private:
    int line_ = -1;
public:
    eCommands name() override { return eCommands::End; };

    int process(int, shared_stack) override;

    void setup(int) override;

    [[nodiscard]] int get_line() const { return line_; }
};

class PushCommand : public BaseIntegerCommand {
public:
    eCommands name() override { return eCommands::Push; };

    int process(int, int, shared_stack) override;

    void setup(int, int) override;
};

class PopCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Pop; };

    int process(int, shared_stack) override;

    void setup(int) override;
};

class PushRCommand : public BaseRegisterCommand {
public:
    eCommands name() override { return eCommands::PushR; };

    int process(RegisterType &, int, shared_stack) override;

    void setup(RegisterType &, int) override;
};

class PopRCommand : public BaseRegisterCommand {
public:
    eCommands name() override { return eCommands::PopR; };

    int process(RegisterType &, int, shared_stack) override;

    void setup(RegisterType &, int) override;
};

class AddCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Add; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class SubCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Sub; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class MulCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Mul; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class DivCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Div; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class InCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::In; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class OutCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Out; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class LabelCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::Label; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::Jump; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpEqualCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpE; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpNotEqualCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpNE; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpGreaterCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpG; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpGreaterOrEqualCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpGE; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpLessCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpL; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class JumpLessOrEqualCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::JumpLE; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};


class CallCommand : public BaseLabelCommand {
public:
    eCommands name() override { return eCommands::Call; }

    int process(LabelType &, int, shared_stack) override;

    void setup(LabelType &, int) override;
};

class RetCommand : public BaseParamLessCommand {
public:
    eCommands name() override { return eCommands::Ret; }

    int process(int, shared_stack) override;

    void setup(int) override;
};

class BlankCommand : public BaseCommand {
public:
    eCommands name() override { return eCommands::Blank; }

    int run(std::string, int line, shared_stack) override { return line + 1; }

    void configure(std::string, int) override {}
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