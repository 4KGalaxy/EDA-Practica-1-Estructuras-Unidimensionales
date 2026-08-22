#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP

#include <cstddef>
#include <stdexcept>

template <typename T>
class StackArray {
private:
    T* data;
    std::size_t capacity;
    std::size_t currentSize;

    void resize(std::size_t newCapacity);

public:
    explicit StackArray(std::size_t initialCapacity = 10);
    ~StackArray();

    void push(const T& value);
    void pop();
    const T& top() const;
    bool isEmpty() const;
    std::size_t size() const;
};

#include "stack_array.cpp"

#endif // STACK_ARRAY_HPP