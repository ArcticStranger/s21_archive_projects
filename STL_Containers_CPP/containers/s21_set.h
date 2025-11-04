#ifndef S21_SET_H_
#define S21_SET_H_

#include <initializer_list>
#include <memory>
#include <utility>

/*
 * From <initializer_list>:
 *  std::initializer_list: Represents a list of elements, used to construct
 *    the set from a brace-enclosed list of values (e.g., {1, 2, 3}).
 *
 * From <memory>:
 *  std::allocator: The default class for memory management (allocation and
 *    deallocation) of the set's elements (the keys).
 *
 * From <utility>:
 *  std::pair: The return type for the insert and emplace methods, used to
 * bundle an iterator (to the element) and a boolean (indicating success).
 *  std::move: Used in the move constructor and move assignment operator to
 *    efficiently transfer ownership of the underlying tree structure.
 *  std::forward: Used within the 'emplace' method to perfectly forward
 * arguments to the underlying tree's emplace function for in-place
 * construction. std::less (from <functional>): Although the header is not
 * directly included, std::less is the default template argument for 'Compare',
 * acting as the default less-than comparator for ordering keys in the set.
 */

#include "tree/s21_red_black_tree.h"

namespace s21 {

template <typename Key>
struct SetTraits {
  using key_type = Key;
  using value_type = Key;
  const key_type& operator()(const value_type& value) const { return value; }
};

/* EXAMPLE
 * s21::set<int>
 *   `K` = `int`
 *   `T` = `int`
 *   `Tr` = `SetTraits<int>`
 *   `Cmp` = `std::less<int>`
 *   `A` = `std::allocator<int>`
 * */
template <typename Key, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<Key>>
class set {
 private:
  using tree_type = RedBlackTree<Key, Key, SetTraits<Key>, Compare, Allocator>;
  tree_type tree_;

 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = typename tree_type::size_type;

  set() : tree_() {}
  explicit set(const Compare& comp, const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {}
  set(std::initializer_list<value_type> const& items) : tree_(items) {}
  set(const set& s) : tree_(s.tree_) {}
  set(set&& s) noexcept : tree_(std::move(s.tree_)) {}
  ~set() = default;

  set& operator=(const set& s) {
    if (this != &s) {
      tree_ = s.tree_;
    }
    return *this;
  }

  set& operator=(set&& s) noexcept {
    if (this != &s) {
      tree_ = std::move(s.tree_);
    }
    return *this;
  }

  iterator begin() noexcept { return tree_.begin(); }
  const_iterator begin() const noexcept { return tree_.begin(); }
  iterator end() noexcept { return tree_.end(); }
  const_iterator end() const noexcept { return tree_.end(); }
  const_iterator cbegin() const noexcept { return tree_.cbegin(); }
  const_iterator cend() const noexcept { return tree_.cend(); }

  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept { return tree_.max_size(); }

  void clear() { tree_.clear(); }
  std::pair<iterator, bool> insert(const value_type& value) {
    return tree_.insert(value);
  }
  iterator erase(iterator pos) { return tree_.erase(pos); }
  void swap(set& other) noexcept { tree_.swap(other.tree_); }
  void merge(set& other) { tree_.merge(other.tree_); }

  iterator find(const Key& key) { return tree_.find(key); }
  bool contains(const Key& key) const { return tree_.contains(key); }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    return tree_.emplace(std::forward<Args>(args)...);
  }
};

}  // namespace s21
#endif  // S21_SET_H_