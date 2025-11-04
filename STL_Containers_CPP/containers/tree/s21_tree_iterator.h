#ifndef S21_TREE_ITERATOR_H_
#define S21_TREE_ITERATOR_H_

#include <iterator>
#include <type_traits>

/*
 * Functions/types used from included headers:
 *
 * From <iterator>:
 * std::bidirectional_iterator_tag: A type tag used to identify the iterator's
 * capabilities. It specifies that the iterator can be moved both forwards
 * (incremented) and backwards (decremented). This is part of the iterator
 * traits system used by standard library algorithms.
 *
 * From <type_traits>:
 * std::ptrdiff_t: A signed integer type that is the result of subtracting two
 * pointers. It is used here as the 'difference_type' for the iterator, which is
 * standard practice. std::conditional_t: A template that selects one of two
 * types based on a compile-time boolean condition. It is used here to switch
 * between const and non-const versions of 'pointer', 'reference', and
 * 'node_pointer' depending on the 'IsConst' template parameter.
 */

#include "s21_tree_node.h"

namespace s21 {

template <typename Node, bool IsConst>
class TreeIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Node::value_type;
  using difference_type = std::ptrdiff_t;
  using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;
  using reference = std::conditional_t<IsConst, const value_type&, value_type&>;

  using node_pointer = std::conditional_t<IsConst, typename Node::const_pointer,
                                          typename Node::pointer>;

  TreeIterator() : node_(nullptr) {}
  explicit TreeIterator(node_pointer node) : node_(node) {}

  operator TreeIterator<Node, true>() const {
    return TreeIterator<Node, true>(node_);
  }

  reference operator*() const { return node_->data; }
  pointer operator->() const { return &(node_->data); }

  TreeIterator& operator++() {
    node_ = node_->successor();
    return *this;
  }

  TreeIterator operator++(int) {
    TreeIterator temp = *this;
    ++(*this);
    return temp;
  }

  TreeIterator& operator--() {
    node_ = node_->predecessor();
    return *this;
  }

  TreeIterator operator--(int) {
    TreeIterator temp = *this;
    --(*this);
    return temp;
  }

  template <bool OtherIsConst>
  bool operator==(const TreeIterator<Node, OtherIsConst>& other) const {
    return node_ == other.get_node();
  }

  template <bool OtherIsConst>
  bool operator!=(const TreeIterator<Node, OtherIsConst>& other) const {
    return !(*this == other);
  }

  node_pointer get_node() const { return node_; }

 private:
  node_pointer node_;
};

}  // namespace s21
#endif  // S21_TREE_ITERATOR_H_