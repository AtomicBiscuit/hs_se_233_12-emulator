#pragma once

#include <exception>
#include <string>
#include <utility>

class InvalidArgumentException : public std::exception {
private:
    std::string message_;
    int line_;
public:
    int line() const { return line_; }

    InvalidArgumentException(std::string message, int line) : message_(std::move(message)), line_(line) {}

    explicit InvalidArgumentException(std::string message) : message_(std::move(message)), line_(-1) {}

    const char *what() { return message_.c_str(); }
};

class UniqueException : public std::exception {
private:
    std::string message_;
    int line_;
public:
    int line() const { return line_; }

    UniqueException(std::string message, int line) : message_(std::move(message)), line_(line) {}

    const char *what() { return message_.c_str(); }
};
