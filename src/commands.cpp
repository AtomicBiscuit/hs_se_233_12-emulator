#include "commands.h"
#include "exc.h"

int BaseIntegerCommand::clear_param(std::string &param, int line) {
    try {
        return std::stoi(param);
    } catch (std::out_of_range &e) {
        throw InvalidArgumentException(e.what());
    }
}

int BaseIntegerCommand::run(std::string param, int line) {
    int value = clear_param(param, line);
    return process(value, line);
}

void BaseIntegerCommand::configure(std::string param, int line, shared_stack stack) {
    int value = clear_param(param, line);
    setup(value, line, stack);
}


int BaseRegisterCommand::run(std::string param, int line) {
    return process(RegisterType::get(param), line);
}

void BaseRegisterCommand::configure(std::string param, int line, shared_stack stack) {
    setup(RegisterType::get(param), line, stack);
}


int BaseLabelCommand::run(std::string param, int line) {
    return process(LabelType::get(param), line);
}

void BaseLabelCommand::configure(std::string param, int line, shared_stack stack) {
    setup(LabelType::get(param), line, stack);
}


int BeginCommand::run(std::string param, int line) {
    return line + 1;
}

void BeginCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("BEGIN command does not take any arguments");
    }
    if (line_ != -1) {
        throw UniqueException("BEGIN command must appear only once", line);
    }
    line_ = line;
}

int EndCommand::run(std::string param, int line) {
    return -1;
}

void EndCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("END command does not take any arguments");
    }
    if (line_ != -1) {
        throw UniqueException("END command must appear only once", line);
    }
    line_ = line;
}


int PushCommand::process(int val, int line) {
    stack_->data.push(val);
    return line + 1;
}

void PushCommand::setup(int val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}


int PopCommand::run(std::string param, int line) {
    stack_->data.pop();
    return line + 1;
}

void PopCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("POP command does not take any arguments");
    }
    stack_ = std::move(stack);
}


int PushRCommand::process(RegisterType &val, int line) {
    stack_->data.push(val.value());
    return line + 1;
}

void PushRCommand::setup(RegisterType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}


int PopRCommand::process(RegisterType &val, int line) {
    val.value() = stack_->data.top();
    stack_->data.pop();
    return line + 1;
}

void PopRCommand::setup(RegisterType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int AddCommand::run(std::string param, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.pop();
    stack_->data.push(first + second);
    return line + 1;
}

void AddCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("ADD command does not take any arguments");
    }
    stack_ = std::move(stack);
}

int SubCommand::run(std::string param, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.pop();
    stack_->data.push(second - first);
    return line + 1;
}

void SubCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("SUB command does not take any arguments");
    }
    stack_ = std::move(stack);
}

int MulCommand::run(std::string param, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.pop();
    stack_->data.push(first * second);
    return line + 1;
}

void MulCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("MUL command does not take any arguments", line);
    }
    stack_ = std::move(stack);
}

int DivCommand::run(std::string param, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.pop();
    stack_->data.push(second / first);
    return line + 1;
}

void DivCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("DIV command does not take any arguments");
    }
    stack_ = std::move(stack);
}

int InCommand::run(std::string param, int line) {
    int value;
    std::cout << "Input number: ";
    std::cin >> value;
    stack_->data.push(value);
    return line + 1;
}

void InCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("IN command does not take any arguments");
    }
    stack_ = std::move(stack);
}

int OutCommand::run(std::string param, int line) {
    std::cout << stack_->data.top() << std::endl;
    stack_->data.pop();
    return line + 1;
}

void OutCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("OUT command does not take any arguments");
    }
    stack_ = std::move(stack);
}

int LabelCommand::process(LabelType &val, int line) {
    return line + 1;
}

void LabelCommand::setup(LabelType &val, int line, shared_stack stack) {
    val.line() = line;
}

int JumpCommand::process(LabelType &val, int line) {
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpCommand::setup(LabelType &val, int line, shared_stack stack) {}

int JumpEqualCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first != second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpEqualCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int JumpNotEqualCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first == second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpNotEqualCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int JumpGreaterCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first <= second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpGreaterCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int JumpGreaterOrEqualCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first < second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpGreaterOrEqualCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int JumpLessCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first >= second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpLessCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int JumpLessOrEqualCommand::process(LabelType &val, int line) {
    int first = stack_->data.top();
    stack_->data.pop();
    int second = stack_->data.top();
    stack_->data.push(first);
    if (first > second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpLessOrEqualCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}


int CallCommand::process(LabelType &val, int line) {
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    stack_->call.push(line);
    return val.line();
}

void CallCommand::setup(LabelType &val, int line, shared_stack stack) {
    stack_ = std::move(stack);
}

int RetCommand::run(std::string param, int line) {
    int to = stack_->call.top();
    stack_->call.pop();
    return to + 1;
}

void RetCommand::configure(std::string param, int line, shared_stack stack) {
    if (not param.empty()) {
        throw InvalidArgumentException("IN command does not take any arguments");
    }
    stack_ = std::move(stack);
}