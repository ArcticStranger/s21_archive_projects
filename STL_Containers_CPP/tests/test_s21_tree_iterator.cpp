#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../s21_containers.h"

class TreeIteratorTest : public ::testing::Test {
 protected:
  using Node = s21::TreeNode<int>;
  using TreeIterator = s21::TreeIterator<Node, false>;
  using ConstTreeIterator = s21::TreeIterator<Node, true>;

  void SetUp() override {
    root_ = new Node(10);
    nodes_.push_back(root_);
    auto n5 = new Node(5);
    nodes_.push_back(n5);
    auto n15 = new Node(15);
    nodes_.push_back(n15);
    auto n3 = new Node(3);
    nodes_.push_back(n3);
    auto n7 = new Node(7);
    nodes_.push_back(n7);

    root_->left = n5;
    n5->parent = root_;
    root_->right = n15;
    n15->parent = root_;
    n5->left = n3;
    n3->parent = n5;
    n5->right = n7;
    n7->parent = n5;
  }

  void TearDown() override {
    for (auto node : nodes_) {
      delete node;
    }
  }

  Node* root_;
  std::vector<Node*> nodes_;
};

TEST_F(TreeIteratorTest, ConstructorsAndConversion) {
  TreeIterator it;
  ASSERT_EQ(it.get_node(), nullptr);
  TreeIterator it_node(root_);
  ASSERT_EQ(it_node.get_node(), root_);
  ConstTreeIterator cit = it_node;
  ASSERT_EQ(cit.get_node(), root_);
}

TEST_F(TreeIteratorTest, Dereferencing) {
  TreeIterator it(root_->left);  // Node with value 5
  ASSERT_EQ(*it, 5);
  *it = 55;
  ASSERT_EQ(root_->left->data, 55);
}

TEST_F(TreeIteratorTest, ArrowOperator) {
  s21::TreeNode<std::pair<const int, std::string>> pair_node(
      std::make_pair(1, "one"));
  s21::TreeIterator<s21::TreeNode<std::pair<const int, std::string>>, false> it(
      &pair_node);
  ASSERT_EQ(it->first, 1);
  it->second = "ONE";
  ASSERT_EQ(pair_node.data.second, "ONE");
}

TEST_F(TreeIteratorTest, Increment) {
  TreeIterator it(root_->left->left);  // Starts at 3
  ASSERT_EQ(*it, 3);
  ++it;
  ASSERT_EQ(*it, 5);
  it++;
  ASSERT_EQ(*it, 7);
  ++it;
  ASSERT_EQ(*it, 10);
  ++it;
  ASSERT_EQ(*it, 15);
  ++it;
  ASSERT_EQ(it.get_node(), nullptr);
}

TEST_F(TreeIteratorTest, Decrement) {
  TreeIterator it(root_->right);  // Starts at 15
  ASSERT_EQ(*it, 15);
  --it;
  ASSERT_EQ(*it, 10);
  it--;
  ASSERT_EQ(*it, 7);
  --it;
  ASSERT_EQ(*it, 5);
  --it;
  ASSERT_EQ(*it, 3);
  --it;
  ASSERT_EQ(it.get_node(), nullptr);
}

TEST_F(TreeIteratorTest, Comparison) {
  TreeIterator it1(root_);
  TreeIterator it2(root_);
  TreeIterator it3(root_->left);
  ConstTreeIterator cit1(root_);

  ASSERT_TRUE(it1 == it2);
  ASSERT_FALSE(it1 == it3);
  ASSERT_TRUE(it1 != it3);
  ASSERT_FALSE(it1 != it2);

  ASSERT_TRUE(it1 == cit1);
  ASSERT_TRUE(cit1 == it1);
  ASSERT_FALSE(it3 == cit1);
}