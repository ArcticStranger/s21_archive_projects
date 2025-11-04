#ifndef S21_RED_BLACK_TREE_TPP_
#define S21_RED_BLACK_TREE_TPP_

#include <utility>

/*
 * Functions/types used from included standard library headers:
 *
 * From <utility>:
 *  std::exchange: Replaces the value of an object with a new one and returns
 * the object's old value. It is used in the move constructor/assignment to
 * transfer a resource pointer (like 'header_') and simultaneously nullify the
 * source pointer. std::move: Casts its argument to an rvalue reference,
 * enabling the use of move semantics. It is used in the move
 * constructor/assignment to efficiently transfer ownership of members like the
 * allocator and comparator. std::forward: Forwards arguments while preserving
 * their value category (lvalue/rvalue). It is essential for the 'emplace'
 * function to pass arguments "perfectly" to the node's constructor. std::swap:
 * Exchanges the contents of two objects. It is used to implement a no-throw
 * swap member function and for the copy-and-swap idiom in the copy assignment
 * operator. std::pair: A template that holds two values. It is used as the
 * return type for the insert and emplace functions to return an iterator and a
 * boolean value.
 */

namespace s21 {

#define RBT_TEMPLATE_PARAMS \
  template <typename K, typename T, typename Tr, typename Cmp, typename A>
#define RBT_CLASS RedBlackTree<K, T, Tr, Cmp, A>

RBT_TEMPLATE_PARAMS
RBT_CLASS::RedBlackTree(const A& alloc)
    : size_(0), node_allocator_(alloc), key_compare_(Cmp()) {
  init_header();
}

RBT_TEMPLATE_PARAMS
RBT_CLASS::RedBlackTree(const Cmp& comp, const A& alloc)
    : size_(0), node_allocator_(alloc), key_compare_(comp) {
  init_header();
}

RBT_TEMPLATE_PARAMS
RBT_CLASS::RedBlackTree(std::initializer_list<value_type> const& items,
                        const A& alloc)
    : RedBlackTree(alloc) {
  for (const auto& item : items) {
    insert(item);
  }
}

RBT_TEMPLATE_PARAMS
RBT_CLASS::RedBlackTree(const RBT_CLASS& other)
    : size_(0),
      node_allocator_(alloc_traits::select_on_container_copy_construction(
          other.node_allocator_)),
      key_extractor_(other.key_extractor_),
      key_compare_(other.key_compare_) {
  init_header();
  if (other.get_root()) {
    set_root(copy_tree(other.get_root(), header_));
    size_ = other.size_;
    header_->left = get_root()->minimum();
    header_->right = get_root()->maximum();
  }
}

RBT_TEMPLATE_PARAMS
RBT_CLASS::RedBlackTree(RBT_CLASS&& other) noexcept
    : header_(std::exchange(other.header_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      node_allocator_(std::move(other.node_allocator_)),
      key_extractor_(std::move(other.key_extractor_)),
      key_compare_(std::move(other.key_compare_)) {
  if (header_ == nullptr) {
    init_header();
  } else {
    if (get_root()) get_root()->parent = header_;
  }
  other.init_header();
}

RBT_TEMPLATE_PARAMS
RBT_CLASS::~RedBlackTree() {
  clear();
  destroy_node(header_);
}

RBT_TEMPLATE_PARAMS
RBT_CLASS& RBT_CLASS::operator=(const RBT_CLASS& other) {
  if (this != &other) {
    RBT_CLASS temp(other);
    swap(temp);
  }
  return *this;
}

RBT_TEMPLATE_PARAMS
RBT_CLASS& RBT_CLASS::operator=(RBT_CLASS&& other) noexcept {
  if (this != &other) {
    clear();
    destroy_node(header_);
    header_ = std::exchange(other.header_, nullptr);
    size_ = std::exchange(other.size_, 0);
    node_allocator_ = std::move(other.node_allocator_);
    key_extractor_ = std::move(other.key_extractor_);
    key_compare_ = std::move(other.key_compare_);
    if (header_ == nullptr) {
      init_header();
    } else {
      if (get_root()) get_root()->parent = header_;
    }
    other.init_header();
  }
  return *this;
}

// --- ITERATORS ---
RBT_TEMPLATE_PARAMS
typename RBT_CLASS::iterator RBT_CLASS::begin() noexcept {
  return iterator(header_->left);
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::const_iterator RBT_CLASS::begin() const noexcept {
  return const_iterator(header_->left);
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::iterator RBT_CLASS::end() noexcept {
  return iterator(header_);
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::const_iterator RBT_CLASS::end() const noexcept {
  return const_iterator(header_);
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::const_iterator RBT_CLASS::cbegin() const noexcept {
  return begin();
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::const_iterator RBT_CLASS::cend() const noexcept {
  return end();
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::clear() {
  if (get_root()) {
    destroy_tree(get_root());
  }
  size_ = 0;
  set_root(nullptr);
  header_->left = header_;
  header_->right = header_;
}

RBT_TEMPLATE_PARAMS
std::pair<typename RBT_CLASS::iterator, bool> RBT_CLASS::insert(
    const value_type& value) {
  return emplace(value);
}

RBT_TEMPLATE_PARAMS
template <typename... Args>
std::pair<typename RBT_CLASS::iterator, bool> RBT_CLASS::emplace(
    Args&&... args) {
  Node* new_node = create_node(std::forward<Args>(args)...);
  const key_type& key = get_key(new_node->data);
  iterator result_it(new_node);

  if (empty()) {
    set_root(new_node);
    size_++;
    get_root()->set_black();
  } else {
    Node* current = get_root();
    Node* parent = header_;
    while (current) {
      parent = current;
      if (key_compare_(key, get_key(current->data))) {
        current = current->left;
      } else if (key_compare_(get_key(current->data), key)) {
        current = current->right;
      } else {
        destroy_node(new_node);
        return {iterator(current), false};
      }
    }
    new_node->parent = parent;
    if (key_compare_(key, get_key(parent->data))) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
    size_++;
    fix_insertion(new_node);
  }
  header_->left = get_root()->minimum();
  header_->right = get_root()->maximum();
  return {result_it, true};
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::iterator RBT_CLASS::erase(iterator pos) {
  if (pos == end() || empty()) {
    return end();
  }

  iterator next_it(pos);
  ++next_it;

  Node* z = pos.get_node();
  Node* y = z;
  Node* x = nullptr;
  Node* x_parent = nullptr;
  TreeNodeColor y_original_color = y->color;

  if (z->left == nullptr) {
    x = z->right;
    x_parent = z->parent;
    transplant(z, z->right);
  } else if (z->right == nullptr) {
    x = z->left;
    x_parent = z->parent;
    transplant(z, z->left);
  } else {
    y = z->right->minimum();
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x_parent = y;
    } else {
      x_parent = y->parent;
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  destroy_node(z);
  size_--;

  if (y_original_color == TreeNodeColor::BLACK) {
    fix_deletion(x, x_parent);
  }

  if (size_ > 0) {
    header_->left = get_root()->minimum();
    header_->right = get_root()->maximum();
  } else {
    header_->left = header_;
    header_->right = header_;
  }
  return next_it;
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::swap(RBT_CLASS& other) noexcept {
  std::swap(header_, other.header_);
  std::swap(size_, other.size_);
  if (alloc_traits::propagate_on_container_swap::value) {
    std::swap(node_allocator_, other.node_allocator_);
  }
  std::swap(key_extractor_, other.key_extractor_);
  std::swap(key_compare_, other.key_compare_);
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::merge(RBT_CLASS& other) {
  if (this == &other) return;
  for (auto it = other.begin(); it != other.end();) {
    if (!contains(get_key(*it))) {
      insert(*it);
      it = other.erase(it);
    } else {
      ++it;
    }
  }
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::iterator RBT_CLASS::find(const key_type& key) {
  Node* current = get_root();
  while (current) {
    if (key_compare_(key, get_key(current->data))) {
      current = current->left;
    } else if (key_compare_(get_key(current->data), key)) {
      current = current->right;
    } else {
      return iterator(current);
    }
  }
  return end();
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::const_iterator RBT_CLASS::find(const key_type& key) const {
  Node* current = get_root();
  while (current) {
    if (key_compare_(key, get_key(current->data))) {
      current = current->left;
    } else if (key_compare_(get_key(current->data), key)) {
      current = current->right;
    } else {
      return const_iterator(current);
    }
  }
  return end();
}

RBT_TEMPLATE_PARAMS
bool RBT_CLASS::contains(const key_type& key) const {
  return find(key) != end();
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::init_header() {
  header_ = create_node();
  header_->parent = nullptr;
  header_->left = header_;
  header_->right = header_;
}

RBT_TEMPLATE_PARAMS
template <typename... Args>
typename RBT_CLASS::Node* RBT_CLASS::create_node(Args&&... args) {
  Node* node = alloc_traits::allocate(node_allocator_, 1);
  try {
    alloc_traits::construct(node_allocator_, node, std::forward<Args>(args)...);
  } catch (...) {
    alloc_traits::deallocate(node_allocator_, node, 1);
    throw;
  }
  return node;
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::destroy_node(Node* node) {
  if (node) {
    alloc_traits::destroy(node_allocator_, node);
    alloc_traits::deallocate(node_allocator_, node, 1);
  }
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::destroy_tree(Node* node) {
  if (node) {
    destroy_tree(node->left);
    destroy_tree(node->right);
    destroy_node(node);
  }
}

RBT_TEMPLATE_PARAMS
typename RBT_CLASS::Node* RBT_CLASS::copy_tree(Node* other_node, Node* parent) {
  if (!other_node) return nullptr;
  Node* new_node = create_node(other_node->data);
  new_node->color = other_node->color;
  new_node->parent = parent;
  new_node->left = copy_tree(other_node->left, new_node);
  new_node->right = copy_tree(other_node->right, new_node);
  return new_node;
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::rotate_left(Node* x) {
  Node* y = x->right;
  x->right = y->left;
  if (y->left) y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == header_) {
    set_root(y);
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

/*
 *       G
 *       |
 *       y
 *      / \
 *     x   C
 *    / \
 *   A   B
 *
 *      G
 *      |
 *      y
 *     / \
 *    B   C
 *
 *   x
 *  /
 * A
 *
 *      G
 *      |
 *      x
 *     / \
 *    A   y
 *   / \
 *  B   C
 *
 */

RBT_TEMPLATE_PARAMS
void RBT_CLASS::rotate_right(Node* y) {
  Node* x = y->left;
  y->left = x->right;
  if (x->right) x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == header_) {
    set_root(x);
  } else if (y == y->parent->right) {
    y->parent->right = x;
  } else {
    y->parent->left = x;
  }
  x->right = y;
  y->parent = x;
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::fix_insertion(Node* z) {
  // Цикл выполняется, пока нарушено свойство:
  // z - не корень, и его родитель - красный.
  while (z != get_root() && z->parent->is_red()) {
    if (z->parent ==
        z->parent->parent->left) {  // случай родитель левый потомок дедушки
      // y - это "дядя" узла z (брат родителя)
      Node* y = z->parent->parent->right;
      if (y && y->is_red()) {  // случай дядя красный
        /*Родитель становится чёрным
        Дядя становится чёрным
        Дедушка становится красным
        Теперь проблема может возникнуть у дедушки,
        поэтому перемещаем z на него для следующей итерации.*/
        z->parent->set_black();
        y->set_black();
        z->parent->parent->set_red();
        z = z->parent->parent;
      } else {  // случай дядя черный или его нет
        if (z == z->parent->right) {  // Переводим в линию
          z = z->parent;
          rotate_left(z);
        }
        // Решаем проблему
        z->parent->set_black();
        z->parent->parent->set_red();
        rotate_right(z->parent->parent);
      }
    } else {  // случай родитель левый потомок дедушки
      Node* y = z->parent->parent->left;
      if (y && y->is_red()) {
        z->parent->set_black();
        y->set_black();
        z->parent->parent->set_red();
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rotate_right(z);
        }
        z->parent->set_black();
        z->parent->parent->set_red();
        rotate_left(z->parent->parent);
      }
    }
  }
  if (get_root()) get_root()->set_black();
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::fix_deletion(Node* x, Node* x_parent) {
  while (x != get_root() && (x == nullptr || x->is_black())) {
    if (x == x_parent->left) {
      Node* w = x_parent->right;
      if (w->is_red()) {
        w->set_black();
        x_parent->set_red();
        rotate_left(x_parent);
        w = x_parent->right;
      }
      if ((w->left == nullptr || w->left->is_black()) &&
          (w->right == nullptr || w->right->is_black())) {
        w->set_red();
        x = x_parent;
        x_parent = x->parent;
      } else {
        if (w->right == nullptr || w->right->is_black()) {
          if (w->left) w->left->set_black();
          w->set_red();
          rotate_right(w);
          w = x_parent->right;
        }
        w->color = x_parent->color;
        x_parent->set_black();
        if (w->right) w->right->set_black();
        rotate_left(x_parent);
        x = get_root();
      }
    } else {
      Node* w = x_parent->left;
      if (w->is_red()) {
        w->set_black();
        x_parent->set_red();
        rotate_right(x_parent);
        w = x_parent->left;
      }
      if ((w->left == nullptr || w->left->is_black()) &&
          (w->right == nullptr || w->right->is_black())) {
        w->set_red();
        x = x_parent;
        x_parent = x->parent;
      } else {
        if (w->left == nullptr || w->left->is_black()) {
          if (w->right) w->right->set_black();
          w->set_red();
          rotate_left(w);
          w = x_parent->left;
        }
        w->color = x_parent->color;
        x_parent->set_black();
        if (w->left) w->left->set_black();
        rotate_right(x_parent);
        x = get_root();
      }
    }
  }
  if (x) x->set_black();
}

RBT_TEMPLATE_PARAMS
void RBT_CLASS::transplant(Node* u, Node* v) {
  if (u->parent == header_) {
    set_root(v);
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  if (v) {
    v->parent = u->parent;
  }
}

#undef RBT_TEMPLATE_PARAMS
#undef RBT_CLASS

}  // namespace s21

#endif  // S21_RED_BLACK_TREE_TPP_