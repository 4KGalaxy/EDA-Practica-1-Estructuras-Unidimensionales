#include "stack_array.hpp"

template <typename T>
StackArray<T>::StackArray(std::size_t initialCapacity)
    : capacity(initialCapacity), currentSize(0) {
    data = new T[capacity];
}

template <typename T>
StackArray<T>::~StackArray() {
    delete[] data;
}

template <typename T>
void StackArray<T>::resize(std::size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (std::size_t i = 0; i < currentSize; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void StackArray<T>::push(const T& value) {
    if (currentSize == capacity) {
        resize(capacity * 2);
    }
    data[currentSize++] = value;
}

template <typename T>
void StackArray<T>::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    --currentSize;
}

template <typename T>
const T& StackArray<T>::top() const {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return data[currentSize - 1];
}

template <typename T>
bool StackArray<T>::isEmpty() const {
    return currentSize == 0;
}

template <typename T>
std::size_t StackArray<T>::size() const {
    return currentSize;
}