#pragma once

#include <cstdint>
#include <utility>
#include <cassert>
#include <iostream>

namespace stack {

    template<class T>
    class Stack {
    private:
        T *_data = nullptr;
        uint64_t _size = 0;
        uint64_t _capacity = 0;

        void _increase_capacity();

    public:
        Stack() = default;

        explicit Stack(uint64_t);

        Stack(Stack &);

        Stack(const Stack &&) noexcept;

        ~Stack();

        Stack &operator=(const Stack &);

        Stack &operator=(Stack &&) noexcept;

        Stack &push(const T &);

        Stack &push(const T &&);

        T &front();

        T &pop();

        uint64_t size();
    };

    template<class T>
    uint64_t Stack<T>::size() {
        return _size;
    }

    template<class T>
    Stack<T>::Stack(uint64_t size) {
        if (size == 0) {
            return;
        }
        _data = new T[size];
        _capacity = size;
    }

    template<class T>
    Stack<T>::Stack(Stack &other) {
        if (other._size == 0) {
            return;
        }
        _data = new T[other._capacity];
        _capacity = other._capacity;
        _size = other._size;
        for (uint64_t i = 0; i < other._size; i++) {
            _data[i] = other._data[i];
        }
    }

    template<class T>
    Stack<T>::Stack(const Stack &&other) noexcept {
        _data = other._data;
        _capacity = other._capacity;
        _size = other._size;
        other._data = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    template<class T>
    Stack<T> &Stack<T>::operator=(const Stack &other) {
        if (&other == this) {
            return *this;
        }
        delete[] _data;
        _data = new T[other._capacity];
        _capacity = other._capacity;
        _size = other._size;
        for (uint64_t i = 0; i < other._size; i++) {
            _data[i] = other._data[i];
        }
    }

    template<class T>
    Stack<T> &Stack<T>::operator=(Stack &&other) noexcept {
        std::swap(*this, other);
        return *this;
    }

    template<class T>
    Stack<T>::~Stack() {
        delete[] _data;
    }

    template<class T>
    Stack<T> &Stack<T>::push(const T &obj) {
        if (_size == _capacity) {
            _increase_capacity();
        }
        _size++;
        _data[_size - 1] = obj;
        return *this;
    }

    template<class T>
    Stack<T> &Stack<T>::push(const T &&obj) {
        if (_size == _capacity) {
            _increase_capacity();
        }
        _size++;
        _data[_size - 1] = std::move(obj);
        return *this;
    }

    template<class T>
    void Stack<T>::_increase_capacity() {
        _capacity = _capacity * 2 + 1;
        auto _data = new T[_capacity];
        for (uint64_t i = 0; i < _size; i++) {
            _data[i] = std::move(_data[i]);
        }
        delete[] _data;
        _data = _data;
    }

    template<class T>
    T &Stack<T>::front() {
        assert(_size > 0);
        return _data[_size - 1];
    }

    template<class T>
    T &Stack<T>::pop() {
        assert(_size > 0);
        return _data[_size--];
    }
}