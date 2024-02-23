#pragma once

#include <cstdint>
#include <utility>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace stack {

    template<class T>
    class Stack {
    private:
        T *_data = nullptr;
        uint32_t _size = 0;
        uint32_t _capacity = 0;

        void _resize(uint32_t new_size);

        class iterator : public std::iterator<
                std::bidirectional_iterator_tag,
                T,
                long long,
                T *,
                T &
        > {
        private:
            typename iterator::pointer _ptr;
        public:
            explicit iterator(typename iterator::pointer ptr) : _ptr(ptr) {}

            iterator(const iterator &other) : _ptr(other._ptr) {}

            iterator &operator++() {
                ++_ptr;
                return *this;
            }

            iterator operator++(int) { return iterator(_ptr++); }

            iterator &operator--() {
                --_ptr;
                return *this;
            }

            iterator operator--(int) { return iterator(_ptr--); }

            bool operator==(iterator other) const { return _ptr == other._ptr; }

            bool operator!=(iterator other) const { return _ptr != other._ptr; }

            typename iterator::reference operator*() const {
                return *_ptr;
            }
        };

        iterator begin();

        iterator end();

    public:

        Stack() = default;

        explicit Stack(uint32_t);

        Stack(const Stack &);

        Stack(Stack &&) noexcept;

        ~Stack();

        Stack &operator=(const Stack &);

        Stack &operator=(Stack &&) noexcept;

        Stack &push(const T &);

        Stack &push(T &&);

        T &top();

        void pop();

        uint32_t size();

        uint32_t capacity();

        bool empty();
    };

    template<class T>
    uint32_t Stack<T>::capacity() {
        return _capacity;
    }

    template<class T>
    Stack<T>::iterator Stack<T>::end() {
        return Stack::iterator(_data + _size);
    }

    template<class T>
    Stack<T>::iterator Stack<T>::begin() {
        return Stack::iterator(_data);
    }

    template<class T>
    bool Stack<T>::empty() {
        return this->begin() == this->end();
    }

    template<class T>
    uint32_t Stack<T>::size() {
        return _size;
    }

    template<class T>
    Stack<T>::Stack(uint32_t size) {
        if (size == 0) {
            return;
        }
        _data = new T[size];
        _capacity = size;
    }

    template<class T>
    Stack<T>::Stack(const Stack &other) {
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
    Stack<T>::Stack(Stack &&other) noexcept {
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
        for (uint32_t i = 0; i < other._size; i++) {
            _data[i] = other._data[i];
        }
        return *this;
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
            _resize(_capacity * 2 + 1);
        }
        _size++;
        _data[_size - 1] = obj;
        return *this;
    }

    template<class T>
    Stack<T> &Stack<T>::push(T &&obj) {
        if (_size == _capacity) {
            _resize(_capacity * 2 + 1);
        }
        _size++;
        _data[_size - 1] = std::move(obj);
        return *this;
    }

    template<class T>
    void Stack<T>::_resize(uint32_t new_size) {
        _capacity = new_size;
        auto data = new T[_capacity];
        for (uint32_t i = 0; i < std::min(_size, _capacity); i++) {
            data[i] = std::move(_data[i]);
        }
        delete[] _data;
        _data = data;
    }

    template<class T>
    T &Stack<T>::top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return *--end();
    }

    template<class T>
    void Stack<T>::pop() {
        if (not empty()) --_size;
        if (_size * 2 + 1000 < _capacity) {
            _resize(_size);
        }
    }
}