#ifndef S21_TREE_NODE_H_
#define S21_TREE_NODE_H_

#include <type_traits>  // Used for compile-time type checking.
#include <utility>      // Used for utility functions.

/*
 * Functions used from <type_traits>:
 * std::is_same_v: Checks if two types are the same.
 * std::decay_t: Applies type decay to a type, removing references and
 * cv-qualifiers. std::tuple_element_t: Obtains the type of a specified element
 * in a tuple. std::is_constructible_v: Checks if a type can be constructed from
 * a given set of argument types. std::is_nothrow_constructible_v: Checks if a
 * type can be constructed from a given set of argument types without throwing
 * exceptions.
 *
 * Functions used from <utility>:
 * std::forward: Forwards arguments, preserving their value category
 * (lvalue/rvalue). std::as_const: Obtains a const reference to an object.
 */

namespace s21 {

enum class TreeNodeColor { RED, BLACK };

template <typename T>
struct TreeNode {
  using value_type = T;
  using pointer = TreeNode*;
  using const_pointer = const TreeNode*;

  pointer parent = nullptr;
  pointer left = nullptr;
  pointer right = nullptr;
  TreeNodeColor color = TreeNodeColor::RED;
  value_type data;

  template <typename... Args>
    requires((sizeof...(Args) == 0) ||
             (!std::is_same_v<
                  std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>,
                  TreeNode<T>> &&
              std::is_constructible_v<value_type, Args...>))
  explicit TreeNode(Args&&... args) noexcept(
      std::is_nothrow_constructible_v<value_type, Args...>)
      : data(std::forward<Args>(args)...) {}

  TreeNode(const TreeNode& other) : color(other.color), data(other.data) {}

  ~TreeNode() = default;

  bool is_red() const noexcept { return color == TreeNodeColor::RED; }
  bool is_black() const noexcept { return color == TreeNodeColor::BLACK; }

  void set_red() noexcept { color = TreeNodeColor::RED; }
  void set_black() noexcept { color = TreeNodeColor::BLACK; }

  bool is_left_child() const noexcept { return parent && this == parent->left; }
  bool is_right_child() const noexcept {
    return parent && this == parent->right;
  }

  const_pointer minimum() const noexcept {
    const_pointer node = this;
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  pointer minimum() noexcept {
    return const_cast<pointer>(std::as_const(*this).minimum());
  }

  const_pointer maximum() const noexcept {
    const_pointer node = this;
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  pointer maximum() noexcept {
    return const_cast<pointer>(std::as_const(*this).maximum());
  }

  const_pointer successor() const noexcept {
    if (right) {
      return right->minimum();
    }
    const_pointer node = this;
    const_pointer p = parent;
    while (p && p->parent != node && node == p->right) {
      node = p;
      p = p->parent;
    }
    return p;
  }

  pointer successor() noexcept {
    return const_cast<pointer>(std::as_const(*this).successor());
  }

  const_pointer predecessor() const noexcept {
    if (parent && parent->parent == this) {
      return right;
    }
    if (left) {
      return left->maximum();
    }
    const_pointer node = this;
    const_pointer p = parent;
    while (p && p->parent != this && node == p->left) {
      node = p;
      p = p->parent;
    }
    return p;
  }

  pointer predecessor() noexcept {
    return const_cast<pointer>(std::as_const(*this).predecessor());
  }
};

}  // namespace s21

#endif  // S21_TREE_NODE_H_