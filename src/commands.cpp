#include "commands.h"

 int BaseIntegerCommand::clear_param(std::string &param) {
     try {
         return std::stoi(param);
     } catch (std::out_of_range &e) {
         throw std::invalid_argument(e.what());
     }
 }

    int BaseIntegerCommand::run(std::string param, int line) override {
        int value = clear_param(param);
        return process(value, line);
    }

    void BaseIntegerCommand::configure(std::string param, int line, shared_stack stack) override {
        int value = clear_param(param);
        setup(value, line, stack);
    }

class BaseRegisterCommand : public BaseCommand {
public:
    virtual int process(RegisterType &val, int line) = 0;

    virtual void setup(RegisterType &val, int line, shared_stack stack) = 0;

    int run(std::string param, int line) override {
        return process(RegisterType::get(param), line);
    }

    void configure(std::string param, int line, shared_stack stack) override {
        setup(RegisterType::get(param), line, stack);
    }
};

class BaseLabelCommand : public BaseCommand {
public:
    virtual int process(LabelType &val, int line) = 0;

    virtual void setup(LabelType &val, int line, shared_stack stack) = 0;

    int run(std::string param, int line) override {
        return process(LabelType::get(param), line);
    }

    void configure(std::string param, int line, shared_stack stack) override {
        setup(LabelType::get(param), line, stack);
    }
};

class BeginCommand : public BaseCommand {
private:
    int line_ = -1;
public:

    int run(std::string param, int line) override {
        return line + 1;
    }

    void configure(std::string param, int line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("BEGIN command does not take any arguments");
        }
        if (line_ != -1) {
            throw std::logic_error("BEGIN command must appear only once");
        }
        line_ = line;
    }
};

class EndCommand : public BaseCommand {
private:
    int line_ = -1;
public:
    int run(std::string param, int line) override {
        return line + 1;
    }

    void configure(std::string param, int line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("END command does not take any arguments");
        }
        if (line_ != -1) {
            throw std::logic_error("END command must appear only once");
        }
        line_ = line;
    }
};

class PushCommand : public BaseIntegerCommand {
private:
    shared_stack stack_;
public:
    int process(int val, int line) override {
        stack_->push(val);
        return line + 1;
    }

    void setup(int val, int line, shared_stack stack) override {
        stack_ = std::move(stack);
    }
};

class PopCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        stack_->pop();
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("POP command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class PushRCommand : public BaseRegisterCommand {
private:
    shared_stack stack_;
public:
    int process(RegisterType &val, int line) override {
        stack_->push(val.value());
        return line + 1;
    }

    void setup(RegisterType &val, int line, shared_stack stack) override {
        stack_ = std::move(stack);
    }
};

class PopRCommand : public BaseRegisterCommand {
private:
    shared_stack stack_;
public:
    int process(RegisterType &val, int line) override {
        val.value() = stack_->top();
        stack_->pop();
        return line + 1;
    }

    void setup(RegisterType &val, int line, shared_stack stack) override {
        stack_ = std::move(stack);
    }
};

class AddCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        int first = stack_->top();
        stack_->pop();
        int second = stack_->top();
        stack_->pop();
        stack_->push(first + second);
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("ADD command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class SubCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        int first = stack_->top();
        stack_->pop();
        int second = stack_->top();
        stack_->pop();
        stack_->push(first - second);
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("SUB command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class MulCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        int first = stack_->top();
        stack_->pop();
        int second = stack_->top();
        stack_->pop();
        stack_->push(first * second);
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("MUL command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class DivCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        int first = stack_->top();
        stack_->pop();
        int second = stack_->top();
        stack_->pop();
        stack_->push(first / second);
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("DIV command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class InCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        int value;
        std::cout << "Input number: ";
        std::cin >> value;
        std::cout << std::endl;
        stack_->push(value);
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("IN command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class OutCommand : public BaseCommand {
private:
    shared_stack stack_;
public:
    int run(std::string param, int line) override {
        std::cout << stack_->top();
        stack_->pop();
        return line + 1;
    }

    void configure(std::string param, int _line, shared_stack stack) override {
        if (not param.empty()) {
            throw std::invalid_argument("OUT command does not take any arguments");
        }
        stack_ = std::move(stack);
    }
};

class LabelCommand : public BaseLabelCommand {
private:
public:
    int process(LabelType &val, int line) override {
        return line + 1;
    }

    void setup(LabelType &val, int line, shared_stack stack) override {
        val.line() = line;
    }
};

class JumpCommand : public BaseLabelCommand {
private:
public:
    int process(LabelType &val, int line) override {
        if (val.line() == -1) {
            throw std::invalid_argument("Can not find label to jump");
        }
        return val.line();
    }

    void setup(LabelType &val, int line, shared_stack stack) override {}
};
