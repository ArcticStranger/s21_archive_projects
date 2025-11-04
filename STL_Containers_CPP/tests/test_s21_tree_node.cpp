#include <gtest/gtest.h>

#include <string>
#include <utility>
#include <vector>

#include "../s21_containers.h"

class TreeNodeTest : public ::testing::Test {
 protected:
  using Node = s21::TreeNode<int>;

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

TEST_F(TreeNodeTest, Constructor) {
  s21::TreeNode<std::string> node("hello");
  ASSERT_EQ(node.data, "hello");
  ASSERT_EQ(node.parent, nullptr);
  ASSERT_EQ(node.color, s21::TreeNodeColor::RED);
}

TEST_F(TreeNodeTest, CopyConstructor) {
  s21::TreeNode<int> original(100);
  original.set_black();
  s21::TreeNode<int> copy(original);
  ASSERT_EQ(copy.data, 100);
  ASSERT_EQ(copy.color, s21::TreeNodeColor::BLACK);
  ASSERT_EQ(copy.parent, nullptr);
}

TEST_F(TreeNodeTest, ColorMethods) {
  s21::TreeNode<int> node(1);
  ASSERT_TRUE(node.is_red());
  ASSERT_FALSE(node.is_black());
  node.set_black();
  ASSERT_TRUE(node.is_black());
  ASSERT_FALSE(node.is_red());
}

TEST_F(TreeNodeTest, Relationship) {
  ASSERT_TRUE(root_->left->is_left_child());
  ASSERT_FALSE(root_->right->is_left_child());
  ASSERT_TRUE(root_->right->is_right_child());
  ASSERT_FALSE(root_->left->is_right_child());
  ASSERT_FALSE(root_->is_left_child());
  ASSERT_FALSE(root_->is_right_child());
}

TEST_F(TreeNodeTest, MinimumMaximum) {
  ASSERT_EQ(root_->minimum()->data, 3);
  ASSERT_EQ(root_->maximum()->data, 15);
  ASSERT_EQ(root_->left->minimum()->data, 3);
  ASSERT_EQ(root_->left->maximum()->data, 7);
}

TEST_F(TreeNodeTest, Successor) {
  ASSERT_EQ(root_->left->left->successor()->data, 5);    // 3 -> 5
  ASSERT_EQ(root_->left->right->successor()->data, 10);  // 7 -> 10
  ASSERT_EQ(root_->left->successor()->data, 7);          // 5 -> 7
  ASSERT_EQ(root_->successor()->data, 15);               // 10 -> 15
  ASSERT_EQ(root_->right->successor(), nullptr);         // 15 -> end
}

TEST_F(TreeNodeTest, Predecessor) {
  ASSERT_EQ(root_->right->predecessor()->data, 10);       // 15 -> 10
  ASSERT_EQ(root_->predecessor()->data, 7);               // 10 -> 7
  ASSERT_EQ(root_->left->right->predecessor()->data, 5);  // 7 -> 5
  ASSERT_EQ(root_->left->predecessor()->data, 3);         // 5 -> 3
  ASSERT_EQ(root_->left->left->predecessor(), nullptr);   // 3 -> rend
}