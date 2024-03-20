#pragma once

#include <string>
#include <vector>
#include "exc.h"


class RegisterType {
private:
    std::string name_;

    int value_ = 0;

    explicit RegisterType(std::string name) : name_(std::move(name)) {}

    static inline std::vector<RegisterType> regs_ = {};

public:
    static RegisterType &get(const std::string &name) {
        if (std::find(available.begin(), available.end(), name) == available.end()) {
            throw InvalidArgumentException("Incorrect register name \"" + name + "\"");
        }
        for (auto &i: regs_) {
            if (i.name_ == name) {
                return i;
            }
        }
        regs_.push_back(RegisterType(name));
        return regs_.back();
    }

    int &value() { return value_; }

    const std::string &name() { return name_; }

    static void clear_all() {
        regs_.clear();
    }

    const static inline std::vector<std::string> available = {"ax", "bx", "cx", "dx", "ex"};
};

class LabelType {
private:
    std::string name_;
    int line_ = -1;

    static inline std::vector<LabelType> labels_;

    explicit LabelType(std::string name) : name_(std::move(name)) {}

public:

    LabelType(const LabelType &other) = default;

    static LabelType &get(const std::string &name) {

        if (name.empty() || not isalpha(name.front())) {
            throw InvalidArgumentException("Incorrect label name \"" + name + "\"");
        }
        for (auto &i: name) {
            if (not isalnum(i)) {
                throw InvalidArgumentException("Incorrect label name \"" + name + "\"");
            }
        }
        for (auto &i: labels_) {
            if (i.name_ == name) {
                return i;
            }
        }
        labels_.push_back(LabelType(name));
        return labels_.back();
    }

    static void clear_all() {
        labels_.clear();
    }

    int &line() { return line_; }

    const std::string &name() { return name_; }
};

class CommandStack {
public:
    stack::Stack<int> data;
    stack::Stack<int> call;
};