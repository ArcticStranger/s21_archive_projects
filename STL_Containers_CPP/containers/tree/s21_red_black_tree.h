#ifndef S21_RED_BLACK_TREE_H_
#define S21_RED_BLACK_TREE_H_

#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

/*
 * From <functional>:
 *  std::less: A function object for performing less-than comparisons. It is the
 *    default comparator for ordering keys in the tree.
 *
 * From <initializer_list>:
 *  std::initializer_list: Represents a list of elements, used to construct
 *    the tree from a brace-enclosed list of values (e.g., {1, 2, 3}).
 *
 * From <memory>:
 *  std::allocator: The default class for memory management (allocation and
 *    deallocation) of the tree's nodes.
 *  std::allocator_traits: Provides a uniform interface to access the properties
 *    and operations of any allocator type, making the tree's memory management
 *    generic. It is used to allocate, deallocate, construct, and destroy nodes.
 *
 * From <stdexcept>:
 *  This header provides standard exception classes. While not directly visible
 *  in the function signatures, it is included to allow for the throwing of
 *  exceptions like std::bad_alloc on allocation failure or other potential
 *  runtime errors.
 *
 * From <utility>:
 *  std::pair: A template that holds two values, used here as the return type
 *    for insert and emplace to bundle an iterator and a boolean.
 *  std::move: Used to transfer ownership of resources in the move constructor
 *    and move assignment operator, enabling efficient move semantics.
 *  std::forward: Used within the emplace function to pass arguments perfectly
 *    to the node's constructor, preserving their lvalue/rvalue properties.
 *  std::swap: A generic function to exchange the contents of two objects,
 *    used to implement the tree's swap member function efficiently.
 */

#include "s21_tree_iterator.h"
#include "s21_tree_node.h"

namespace s21 {
/*Key type,
 *Value type,
 *Traits functor get key from value: SetTraits, MapTraits.
 *Compare functor std::less and others
 *Allocator class for memory handling
 */
template <typename Key, typename T, typename Traits,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<T>>
class RedBlackTree {
 public:
  using key_type = Key;
  using value_type = T;
  using size_type = std::size_t;

  using Node = TreeNode<value_type>;
  using iterator = TreeIterator<Node, false>;
  using const_iterator = TreeIterator<Node, true>;
  using node_allocator_type =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using alloc_traits = std::allocator_traits<node_allocator_type>;

  explicit RedBlackTree(const Allocator& alloc = Allocator());
  explicit RedBlackTree(const Compare& comp,
                        const Allocator& alloc = Allocator());
  RedBlackTree(std::initializer_list<value_type> const& items,
               const Allocator& alloc = Allocator());
  RedBlackTree(const RedBlackTree& other);
  RedBlackTree(RedBlackTree&& other) noexcept;
  ~RedBlackTree();

  RedBlackTree& operator=(const RedBlackTree& other);
  RedBlackTree& operator=(RedBlackTree&& other) noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type max_size() const noexcept {
    return alloc_traits::max_size(node_allocator_);
  }

  void clear();
  std::pair<iterator, bool> insert(const value_type& value);
  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args);
  iterator erase(iterator pos);
  void swap(RedBlackTree& other) noexcept;
  void merge(RedBlackTree& other);

  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  bool contains(const key_type& key) const;

 private:
  Node* header_;
  size_type size_;
  node_allocator_type node_allocator_;
  Traits key_extractor_;
  Compare key_compare_;

  void init_header();
  template <typename... Args>
  Node* create_node(Args&&... args);
  void destroy_node(Node* node);
  void destroy_tree(Node* node);
  Node* copy_tree(Node* other_node, Node* parent);

  Node* get_root() const noexcept { return header_->parent; }
  void set_root(Node* node) noexcept {
    header_->parent = node;
    if (node) node->parent = header_;
  }

  void rotate_left(Node* node);
  void rotate_right(Node* node);
  void fix_insertion(Node* node);
  void fix_deletion(Node* node, Node* parent);
  void transplant(Node* u, Node* v);

  const key_type& get_key(const value_type& value) const {
    return key_extractor_(value);
  }
};

}  // namespace s21

#include "s21_red_black_tree.tpp"

#endif  // S21_RED_BLACK_TREE_H_