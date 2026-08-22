#include "stack_list.hpp"

template <typename T>
StackList<T>::StackList() : head(nullptr), currentSize(0) {}

template <typename T>
StackList<T>::~StackList() {
    while (!isEmpty()) {
        pop();
    }
}

template <typename T>
void StackList<T>::push(const T& val) {
    head = new Node(val, head);
    ++currentSize;
}

template <typename T>
void StackList<T>::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Pila vacia: no se puede realizar pop.");
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    --currentSize;
}

template <typename T>
const T& StackList<T>::top() const {
    if (isEmpty()) {
        throw std::underflow_error("Pila vacia: no hay elementos en top.");
    }
    return head->data;
}

template <typename T>
bool StackList<T>::isEmpty() const {
    return head == nullptr;
}

template <typename T>
std::size_t StackList<T>::size() const {
    return currentSize;
}