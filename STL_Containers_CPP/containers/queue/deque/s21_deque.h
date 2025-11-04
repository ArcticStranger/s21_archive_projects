#ifndef _SUPPORTED_DEQUE_H_
#define _SUPPORTED_DEQUE_H_

#include <cstddef>
#include <stdexcept>

namespace s21 {
template <typename T>
class deque {
 private:
  struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
  };

  Node* head;
  Node* tail;
  size_t deque_size;

 public:
  // member types
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  // member funcs
  deque() : head(nullptr), tail(nullptr), deque_size(0) {}

  deque(size_type n) : head(nullptr), tail(nullptr), deque_size(0) {
    for (size_type i = 0; i < n; ++i) {
      push_back(value_type());
    }
  }

  deque(const deque& other) : head(nullptr), tail(nullptr), deque_size(0) {
    Node* current = other.head;
    while (current != nullptr) {
      push_back(current->data);
      current = current->next;
    }
  }

  deque(deque&& other)
      : head(other.head), tail(other.tail), deque_size(other.deque_size) {
    other.head = nullptr;
    other.tail = nullptr;
    other.deque_size = 0;
  }

  ~deque() {
    while (head != nullptr) {
      Node* temp = head;
      head = head->next;
      delete temp;
    }
    tail = nullptr;
    deque_size = 0;
  }

  deque& operator=(const deque& other) {
    if (this != &other) {
      while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
      }
      tail = nullptr;
      deque_size = 0;
      Node* current = other.head;
      while (current != nullptr) {
        push_back(current->data);
        current = current->next;
      }
    }
    return *this;
  }

  deque& operator=(deque&& other) noexcept {
    if (this != &other) {
      while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
      }
      head = other.head;
      tail = other.tail;
      deque_size = other.deque_size;
      other.head = nullptr;
      other.tail = nullptr;
      other.deque_size = 0;
    }
    return *this;
  }

  // elem access
  reference front() {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    return head->data;
  }

  const_reference front() const {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    return head->data;
  }

  reference back() {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    return tail->data;
  }

  const_reference back() const {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    return tail->data;
  }

  // capacity
  bool empty() const { return deque_size == 0; }

  size_type size() const { return deque_size; }

  // modifiers
  void push_back(const_reference value) {
    Node* newNode = new Node(value);
    if (empty()) {
      head = tail = newNode;
    } else {
      newNode->prev = tail;
      tail->next = newNode;
      tail = newNode;
    }
    deque_size++;
  }

  void push_front(const_reference value) {
    Node* newNode = new Node(value);
    if (empty()) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head->prev = newNode;
      head = newNode;
    }
    deque_size++;
  }

  void pop_front() {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    Node* temp = head;
    head = head->next;
    if (head != nullptr) {
      head->prev = nullptr;
    } else {
      tail = nullptr;
    }
    delete temp;
    deque_size--;
  }

  void pop_back() {
    if (empty()) {
      throw std::runtime_error("Deque is empty");
    }
    Node* temp = tail;
    tail = tail->prev;
    if (tail != nullptr) {
      tail->next = nullptr;
    } else {
      head = nullptr;
    }
    delete temp;
    deque_size--;
  }

  void swap(deque& other) {
    Node* temp_head = head;
    Node* temp_tail = tail;
    size_type temp_size = deque_size;
    head = other.head;
    tail = other.tail;
    deque_size = other.deque_size;
    other.head = temp_head;
    other.tail = temp_tail;
    other.deque_size = temp_size;
  }
};
}  // namespace s21

#endif