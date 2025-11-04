#ifndef S21_MAP_H_
#define S21_MAP_H_

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

/*
 * From <initializer_list>:
 *  std::initializer_list: Represents a list of key-value pairs, used to
 * construct the map from a brace-enclosed list (e.g., {{"key1", 1}, {"key2",
 * 2}}).
 *
 * From <memory>:
 *  std::allocator: The default class for memory management (allocation and
 *    deallocation) of the map's elements (key-value pairs).
 *
 * From <stdexcept>:
 *  std::out_of_range: An exception type thrown by the 'at' method when the
 *    requested key does not exist in the map.
 *
 * From <utility>:
 *  std::pair: The key-value pair structure that serves as the 'value_type' for
 *    the map. It is also used as the return type for insertion methods to
 * bundle an iterator and a boolean value. std::move: Used in the move
 * constructor and move assignment operator to efficiently transfer ownership of
 * the underlying tree structure. std::forward: Used within the 'emplace' method
 * to perfectly forward arguments to the underlying tree's emplace function.
 *  std::piecewise_construct: A tag used in conjunction with emplace/pair
 * constructors to construct the key and value of a pair in-place from separate
 * argument lists. std::forward_as_tuple (from <tuple>): Creates a tuple of
 * references, used with piecewise_construct in the operator[] implementation to
 * construct pair elements. std::tuple (from <tuple>): A fixed-size collection
 * of heterogeneous values, used with piecewise_construct to group constructor
 * arguments for the pair's elements. std::less (from <functional>): Although
 * not directly included, it is the default template argument for 'Compare',
 * acting as the default less-than comparator.
 */

#include "tree/s21_red_black_tree.h"

namespace s21 {

template <typename Key, typename T>
struct MapTraits {
  using key_type = Key;
  using value_type = std::pair<const Key, T>;
  const key_type& operator()(const value_type& value) const {
    return value.first;
  }
};

/* EXAMPLE
 *`s21::map<std::string, double>`
 *   `K` = `std::string`
 *   `T` = `std::pair<const std::string, double>`
 *   `Tr` = `MapTraits<std::string, double>`
 *   `Cmp` = `std::less<std::string>`
 *   `A` = `std::allocator<std::pair<const std::string, double>>`
 */
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class map {
 private:
  using tree_type = RedBlackTree<Key, std::pair<const Key, T>,
                                 MapTraits<Key, T>, Compare, Allocator>;
  tree_type tree_;

 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = typename tree_type::size_type;

  map() : tree_() {}
  explicit map(const Compare& comp, const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {}
  map(std::initializer_list<value_type> const& items) : tree_(items) {}
  map(const map& m) : tree_(m.tree_) {}
  map(map&& m) noexcept : tree_(std::move(m.tree_)) {}
  ~map() = default;

  map& operator=(const map& m) {
    if (this != &m) {
      tree_ = m.tree_;
    }
    return *this;
  }

  map& operator=(map&& m) noexcept {
    if (this != &m) {
      tree_ = std::move(m.tree_);
    }
    return *this;
  }

  T& at(const Key& key) {
    iterator it = tree_.find(key);
    if (it == tree_.end()) {
      throw std::out_of_range("map::at: key not found");
    }
    return it->second;
  }

  const T& at(const Key& key) const {
    const_iterator it = tree_.find(key);
    if (it == tree_.end()) {
      throw std::out_of_range("map::at: key not found");
    }
    return it->second;
  }

  T& operator[](const Key& key) {
    auto result_pair = tree_.emplace(
        std::piecewise_construct, std::forward_as_tuple(key), std::tuple<>());
    return result_pair.first->second;
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
  std::pair<iterator, bool> insert(const Key& key, const T& obj) {
    return tree_.emplace(key, obj);
  }
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj) {
    iterator it = tree_.find(key);
    if (it != tree_.end()) {
      it->second = obj;
      return {it, false};
    } else {
      return insert(key, obj);
    }
  }

  iterator erase(iterator pos) { return tree_.erase(pos); }
  void swap(map& other) noexcept { tree_.swap(other.tree_); }
  void merge(map& other) { tree_.merge(other.tree_); }

  iterator find(const Key& key) { return tree_.find(key); }
  const_iterator find(const Key& key) const { return tree_.find(key); }
  bool contains(const Key& key) const { return tree_.contains(key); }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    return tree_.emplace(std::forward<Args>(args)...);
  }
};

}  // namespace s21
#endif  // S21_MAP_H_