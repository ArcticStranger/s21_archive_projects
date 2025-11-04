#ifndef _s21_queue_h_
#define _s21_queue_h_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "./deque/s21_deque.h"

namespace s21 {
template <typename T>
class queue {  // FIFO - First In First Out
 public:
  // queue member type
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

 private:
  deque<T> deque_;

 public:
  // queue member functions
  queue() = default;

  queue(size_type n) : deque_(n) {}

  queue(std::initializer_list<value_type> const& items) {
    for (const auto& item : items) {
      deque_.push_back(item);
    }
  }

  queue(const queue& q) : deque_(q.deque_) {}

  queue(queue&& q) : deque_(std::move(q.deque_)) {}

  ~queue() = default;

  queue& operator=(const queue& q) {
    if (this != &q) {
      deque_ = q.deque_;
    }
    return *this;
  }

  queue& operator=(queue&& q) noexcept {
    if (this != &q) {
      deque_ = std::move(q.deque_);
    }
    return *this;
  }

  // queue element access
  reference front() { return deque_.front(); }

  const_reference front() const { return deque_.front(); }

  reference back() { return deque_.back(); }

  const_reference back() const { return deque_.back(); }

  // queue capacity
  bool empty() const { return deque_.empty(); }

  size_type size() const { return deque_.size(); }

  // queue modifiers
  void push(const_reference value) { deque_.push_back(value); }

  void pop() { deque_.pop_front(); }

  void swap(queue& other) { deque_.swap(other.deque_); }
};
}  // namespace s21

#endif