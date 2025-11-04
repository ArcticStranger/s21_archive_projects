#ifndef _S21_STACK_H_
#define _S21_STACK_H_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace s21 {
template <typename T>
class stack {  // Last In First Out только pushfront и popfront (то есть
               // удаление добавление в начале стека)
 public:
  // Stack Member Types
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

 private:
  // опять копия list
  struct Node {
    value_type data;
    Node* next;
    Node* prev;
    Node(const value_type& value) : data(value), next(nullptr), prev(nullptr) {}
  };

  Node* head;
  Node* tail;
  size_type stack_size;

 public:
  // Stack Member Functions
  stack() : head(nullptr), tail(nullptr), stack_size(0) {}

  stack(std::initializer_list<value_type> const& items)
      : head(nullptr), tail(nullptr), stack_size(0) {
    for (const auto& item : items) {
      push(item);
    }
  }

  stack(const stack& s) : head(nullptr), tail(nullptr), stack_size(0) {
    Node* current = s.head;
    stack tmp;
    while (current != nullptr) {
      tmp.push(current->data);
      current = current->next;
    }
    current = tmp.head;
    while (current != nullptr) {
      push(current->data);
      current = current->next;
    }
  }

  stack(stack&& s) : head(s.head), tail(s.tail), stack_size(s.stack_size) {
    s.head = nullptr;
    s.tail = nullptr;
    s.stack_size = 0;
  }

  ~stack() {
    while (head != nullptr) {
      Node* temp = head;
      head = head->next;
      delete temp;
    }
    tail = nullptr;
    stack_size = 0;
  }

  stack& operator=(stack&& s) noexcept {
    if (this != &s) {
      while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
      }
      head = s.head;
      tail = s.tail;
      stack_size = s.stack_size;
      s.head = nullptr;
      s.tail = nullptr;
      s.stack_size = 0;
    }
    return *this;
  }

  // Stack Element Access
  reference top() {
    if (empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return head->data;
  }

  const_reference top() const {
    if (empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return head->data;
  }

  // Stack Capacity
  bool empty() const { return stack_size == 0; }

  size_type size() const { return stack_size; }

  // Stack Modifiers
  void push(const_reference value) {
    Node* newNode = new Node(value);
    if (empty()) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head->prev = newNode;
      head = newNode;
    }
    stack_size++;
  }

  void pop() {
    if (empty()) {
      throw std::runtime_error("Stack is empty");
    }
    Node* temp = head;
    head = head->next;
    if (head != nullptr) {
      head->prev = nullptr;
    } else {
      tail = nullptr;
    }
    delete temp;
    stack_size--;
  }

  void swap(stack& other) {
    Node* temp_head = head;
    Node* temp_tail = tail;
    size_type temp_size = stack_size;
    head = other.head;
    tail = other.tail;
    stack_size = other.stack_size;
    other.head = temp_head;
    other.tail = temp_tail;
    other.stack_size = temp_size;
  }
};
}  // namespace s21

#endif