#ifndef STACK_LIST_HPP
#define STACK_LIST_HPP

#include <cstddef>
#include <stdexcept>

template <typename T>
class StackList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* n = nullptr) : data(val), next(n) {}
    };

    Node* head;
    size_t count;

public:
    StackList();
    ~StackList();

    void push(const T& value);
    void pop();
    const T& top() const;
    bool isEmpty() const;
    size_t size() const;
};

#include "stack_list.cpp"

#endif // STACK_LIST_HPP