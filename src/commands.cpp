#include "commands.h"
#include "exc.h"

int BaseParamLessCommand::run(std::string param, int line, shared_stack stack) {
    return process(line, stack);
}

void BaseParamLessCommand::configure(std::string param, int line) {
    if (not param.empty()) {
        throw InvalidArgumentException(command_name.at(name()) + " command does not take any arguments");
    }
    setup(line);
}


int BaseIntegerCommand::clear_param(std::string &param, int line) {
    try {
        return std::stoi(param);
    } catch (std::out_of_range &e) {
        throw InvalidArgumentException(e.what());
    }
}

int BaseIntegerCommand::run(std::string param, int line, shared_stack stack) {
    int value = clear_param(param, line);
    return process(value, line, stack);
}

void BaseIntegerCommand::configure(std::string param, int line) {
    int value = clear_param(param, line);
    setup(value, line);
}


int BaseRegisterCommand::run(std::string param, int line, shared_stack stack) {
    return process(RegisterType::get(param), line, stack);
}

void BaseRegisterCommand::configure(std::string param, int line) {
    setup(RegisterType::get(param), line);
}


int BaseLabelCommand::run(std::string param, int line, shared_stack stack) {
    return process(LabelType::get(param), line, stack);
}

void BaseLabelCommand::configure(std::string param, int line) {
    setup(LabelType::get(param), line);
}

int BeginCommand::process(int line, shared_stack stack) {
    return line + 1;
}

void BeginCommand::setup(int line) {
    if (line_ != -1) {
        throw UniqueException("BEGIN command must appear only once", line);
    }
    line_ = line;
}

void BeginCommand::clear() {
    line_ = -1;
}

int EndCommand::process(int line, shared_stack stack) {
    return -1;
}

void EndCommand::setup(int line) {
    if (line_ != -1) {
        throw UniqueException("END command must appear only once", line);
    }
    line_ = line;
}

void EndCommand::clear() {
    line_ = -1;
}


int PushCommand::process(int val, int line, shared_stack stack) {
    stack->data.push(val);
    return line + 1;
}

void PushCommand::setup(int val, int line) {}


int PopCommand::process(int line, shared_stack stack) {
    stack->data.pop();
    return line + 1;
}

void PopCommand::setup(int line) {}


int PushRCommand::process(RegisterType &val, int line, shared_stack stack) {
    stack->data.push(val.value());
    return line + 1;
}

void PushRCommand::setup(RegisterType &val, int line) {}


int PopRCommand::process(RegisterType &val, int line, shared_stack stack) {
    val.value() = stack->data.top();
    stack->data.pop();
    return line + 1;
}

void PopRCommand::setup(RegisterType &val, int line) {}

int AddCommand::process(int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.pop();
    stack->data.push(first + second);
    return line + 1;
}

void AddCommand::setup(int line) {}

int SubCommand::process(int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.pop();
    stack->data.push(second - first);
    return line + 1;
}

void SubCommand::setup(int line) {}

int MulCommand::process(int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.pop();
    stack->data.push(first * second);
    return line + 1;
}

void MulCommand::setup(int line) {}

int DivCommand::process(int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.pop();
    stack->data.push(second / first);
    return line + 1;
}

void DivCommand::setup(int line) {}

int InCommand::process(int line, shared_stack stack) {
    int value = 0;
    std::cout << "Input number: ";
    std::cin >> value;
    stack->data.push(value);
    return line + 1;
}

void InCommand::setup(int line) {}

int OutCommand::process(int line, shared_stack stack) {
    std::cout << stack->data.top() << std::endl;
    stack->data.pop();
    return line + 1;
}

void OutCommand::setup(int line) {}

int LabelCommand::process(LabelType &val, int line, shared_stack stack) {
    return line + 1;
}

void LabelCommand::setup(LabelType &val, int line) {
    val.line() = line;
}

int JumpCommand::process(LabelType &val, int line, shared_stack stack) {
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpCommand::setup(LabelType &val, int line) {}

int JumpEqualCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first != second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpEqualCommand::setup(LabelType &val, int line) {}

int JumpNotEqualCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first == second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpNotEqualCommand::setup(LabelType &val, int line) {}

int JumpGreaterCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first <= second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpGreaterCommand::setup(LabelType &val, int line) {}

int JumpGreaterOrEqualCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first < second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpGreaterOrEqualCommand::setup(LabelType &val, int line) {}

int JumpLessCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first >= second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpLessCommand::setup(LabelType &val, int line) {}

int JumpLessOrEqualCommand::process(LabelType &val, int line, shared_stack stack) {
    int first = stack->data.top();
    stack->data.pop();
    int second = stack->data.top();
    stack->data.push(first);
    if (first > second) {
        return line + 1;
    }
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    return val.line();
}

void JumpLessOrEqualCommand::setup(LabelType &val, int line) {}


int CallCommand::process(LabelType &val, int line, shared_stack stack) {
    if (val.line() == -1) {
        throw InvalidArgumentException("Can not find label \"" + val.name() + "\" to jump");
    }
    stack->call.push(line);
    return val.line();
}

void CallCommand::setup(LabelType &val, int line) {}

int RetCommand::process(int line, shared_stack stack) {
    int to = stack->call.top();
    stack->call.pop();
    return to + 1;
}

void RetCommand::setup(int line) {}