#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#define private public
#include "../containers/tree/s21_red_black_tree.h"
#undef private

template <typename Key>
struct TestSetTraits {
  using key_type = Key;
  using value_type = Key;
  const key_type& operator()(const value_type& value) const { return value; }
};

class RedBlackTreeTest : public ::testing::Test {
 protected:
  using Tree = s21::RedBlackTree<int, int, TestSetTraits<int>>;
  using Node = typename Tree::Node;

  int validate_rb_properties(Node* node) {
    if (!node) return 1;  // Leaf nodes (NIL) are black
    if (node->is_red()) {
      if ((node->left && node->left->is_red()) ||
          (node->right && node->right->is_red())) {
        return -1;  // Red violation
      }
    }
    int left_bh = validate_rb_properties(node->left);
    int right_bh = validate_rb_properties(node->right);
    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh) {
      return -1;  // Black height violation
    }
    return (node->is_black() ? 1 : 0) + left_bh;
  }

  void assert_is_valid_rb_tree(const Tree& tree) {
    Node* root = tree.get_root();
    if (root) {
      ASSERT_TRUE(root->is_black());
    }
    ASSERT_NE(validate_rb_properties(root), -1);
    ASSERT_EQ(tree.empty(), tree.size() == 0);
    if (!tree.empty()) {
      ASSERT_EQ(*(tree.begin()), tree.header_->left->data);
      ASSERT_EQ(*(--tree.end()), tree.header_->right->data);
    } else {
      ASSERT_EQ(tree.begin(), tree.end());
      ASSERT_EQ(tree.header_->left, tree.header_);
      ASSERT_EQ(tree.header_->right, tree.header_);
    }
  }

  void SetUp() override {
    filled_tree_.insert(8);
    filled_tree_.insert(18);
    filled_tree_.insert(5);
    filled_tree_.insert(15);
    filled_tree_.insert(17);
    filled_tree_.insert(25);
    filled_tree_.insert(40);
    filled_tree_.insert(80);
  }

  Tree empty_tree_;
  Tree filled_tree_;
};

TEST_F(RedBlackTreeTest, DefaultConstructor) {
  ASSERT_TRUE(empty_tree_.empty());
  ASSERT_EQ(empty_tree_.size(), 0u);
  assert_is_valid_rb_tree(empty_tree_);
}

TEST_F(RedBlackTreeTest, InitializerListConstructor) {
  Tree list_tree = {10, 5, 15, 3, 7, 12, 17};
  ASSERT_FALSE(list_tree.empty());
  ASSERT_EQ(list_tree.size(), 7u);
  assert_is_valid_rb_tree(list_tree);
}

TEST_F(RedBlackTreeTest, CopyConstructor) {
  Tree copy_tree(filled_tree_);
  ASSERT_EQ(filled_tree_.size(), copy_tree.size());
  ASSERT_TRUE(
      std::equal(filled_tree_.begin(), filled_tree_.end(), copy_tree.begin()));
  assert_is_valid_rb_tree(copy_tree);
}

TEST_F(RedBlackTreeTest, MoveConstructor) {
  Tree original_tree = {1, 2, 3};
  Tree moved_tree(std::move(original_tree));
  ASSERT_EQ(moved_tree.size(), 3u);
  ASSERT_TRUE(original_tree.empty());
  assert_is_valid_rb_tree(moved_tree);
}

TEST_F(RedBlackTreeTest, CopyAssignment) {
  Tree copy_assign;
  copy_assign.insert(999);
  copy_assign = filled_tree_;
  ASSERT_EQ(filled_tree_.size(), copy_assign.size());
  assert_is_valid_rb_tree(copy_assign);
}

TEST_F(RedBlackTreeTest, MoveAssignment) {
  Tree other_tree = {1, 2, 3};
  filled_tree_ = std::move(other_tree);
  ASSERT_EQ(filled_tree_.size(), 3u);
  ASSERT_TRUE(other_tree.empty());
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, Clear) {
  filled_tree_.clear();
  ASSERT_TRUE(filled_tree_.empty());
  ASSERT_EQ(filled_tree_.size(), 0u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, InsertAndEmplace) {
  auto result1 = filled_tree_.insert(20);
  ASSERT_TRUE(result1.second);
  ASSERT_EQ(*(result1.first), 20);
  ASSERT_EQ(filled_tree_.size(), 9u);
  assert_is_valid_rb_tree(filled_tree_);

  auto result2 = filled_tree_.insert(18);
  ASSERT_FALSE(result2.second);
  ASSERT_EQ(filled_tree_.size(), 9u);

  auto result3 = filled_tree_.emplace(30);
  ASSERT_TRUE(result3.second);
  ASSERT_EQ(*(result3.first), 30);
  ASSERT_EQ(filled_tree_.size(), 10u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, EraseLeaf) {
  filled_tree_.erase(filled_tree_.find(80));
  ASSERT_EQ(filled_tree_.size(), 7u);
  assert_is_valid_rb_tree(filled_tree_);

  filled_tree_.erase(filled_tree_.find(17));
  ASSERT_EQ(filled_tree_.size(), 6u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, EraseNodeWithTwoChildren) {
  filled_tree_.erase(filled_tree_.find(18));
  ASSERT_EQ(filled_tree_.size(), 7u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, EraseComplexCase) {
  filled_tree_.erase(filled_tree_.find(5));
  ASSERT_EQ(filled_tree_.size(), 7u);
  assert_is_valid_rb_tree(filled_tree_);

  filled_tree_.erase(filled_tree_.find(25));
  ASSERT_EQ(filled_tree_.size(), 6u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, EraseMinMax) {
  filled_tree_.erase(filled_tree_.begin());  // Erase 5
  ASSERT_EQ(*(filled_tree_.begin()), 8);
  ASSERT_EQ(filled_tree_.size(), 7u);
  assert_is_valid_rb_tree(filled_tree_);

  filled_tree_.erase(--filled_tree_.end());  // Erase 80
  ASSERT_EQ(*(--filled_tree_.end()), 40);
  ASSERT_EQ(filled_tree_.size(), 6u);
  assert_is_valid_rb_tree(filled_tree_);
}

TEST_F(RedBlackTreeTest, Swap) {
  Tree other_tree = {100, 200};
  filled_tree_.swap(other_tree);
  ASSERT_EQ(filled_tree_.size(), 2u);
  ASSERT_EQ(other_tree.size(), 8u);
  ASSERT_TRUE(filled_tree_.contains(100));
  assert_is_valid_rb_tree(filled_tree_);
  assert_is_valid_rb_tree(other_tree);
}

TEST_F(RedBlackTreeTest, Merge) {
  Tree other_tree = {18, 99, 1};
  filled_tree_.merge(other_tree);
  ASSERT_EQ(filled_tree_.size(), 10u);
  ASSERT_TRUE(filled_tree_.contains(1));
  ASSERT_TRUE(filled_tree_.contains(99));
  ASSERT_EQ(other_tree.size(), 1u);
  ASSERT_TRUE(other_tree.contains(18));
}

TEST_F(RedBlackTreeTest, FindAndContains) {
  auto it = filled_tree_.find(17);
  ASSERT_NE(it, filled_tree_.end());
  ASSERT_EQ(*it, 17);
  ASSERT_EQ(filled_tree_.find(999), filled_tree_.end());
  ASSERT_TRUE(filled_tree_.contains(40));
  ASSERT_FALSE(filled_tree_.contains(999));
}

TEST_F(RedBlackTreeTest, Iteration) {
  std::vector<int> expected = {5, 8, 15, 17, 18, 25, 40, 80};
  std::vector<int> actual;
  for (int val : filled_tree_) {
    actual.push_back(val);
  }
  ASSERT_EQ(expected, actual);
}