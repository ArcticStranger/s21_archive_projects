#include <gtest/gtest.h>

#include <stack>

#include "../s21_containers.h"

class StackTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_stack = new s21::stack<int>();
    test_stack_filled = new s21::stack<int>{1, 2, 3};
  }

  void TearDown() override {
    delete test_stack;
    delete test_stack_filled;
  }

  s21::stack<int>* test_stack;
  s21::stack<int>* test_stack_filled;
};

TEST_F(StackTest, DefaultConstructor) {
  EXPECT_EQ(test_stack->size(), size_t(0));
  EXPECT_TRUE(test_stack->empty());
}

TEST_F(StackTest, InitializerListConstructor) {
  EXPECT_EQ(test_stack_filled->size(), size_t(3));
  EXPECT_FALSE(test_stack_filled->empty());
  EXPECT_EQ(test_stack_filled->top(), 3);
}

TEST_F(StackTest, CopyConstructor) {
  s21::stack<int> s = *test_stack_filled;
  EXPECT_EQ(s.size(), test_stack_filled->size());
  EXPECT_EQ(s.top(), test_stack_filled->top());
  s.pop();
  EXPECT_EQ(s.top(), 2);
}

TEST_F(StackTest, MoveConstructor) {
  s21::stack<int> temp = *test_stack_filled;
  s21::stack<int> s(std::move(temp));
  EXPECT_EQ(s.size(), size_t(3));
  EXPECT_EQ(temp.size(), size_t(0));
  EXPECT_TRUE(temp.empty());
  EXPECT_EQ(s.top(), 3);
}

TEST_F(StackTest, MoveAssignment) {
  s21::stack<int> temp = *test_stack_filled;
  s21::stack<int> s;
  s = std::move(temp);
  EXPECT_EQ(s.size(), size_t(3));
  EXPECT_EQ(temp.size(), size_t(0));
  EXPECT_TRUE(temp.empty());
  EXPECT_EQ(s.top(), 3);
}

TEST_F(StackTest, Top) {
  EXPECT_EQ(test_stack_filled->top(), 3);
  EXPECT_THROW(test_stack->top(), std::runtime_error);
}

TEST_F(StackTest, Size) {
  EXPECT_EQ(test_stack->size(), size_t(0));
  EXPECT_EQ(test_stack_filled->size(), size_t(3));
}

TEST_F(StackTest, Empty) {
  EXPECT_TRUE(test_stack->empty());
  EXPECT_FALSE(test_stack_filled->empty());
}

TEST_F(StackTest, Push) {
  test_stack->push(42);
  EXPECT_EQ(test_stack->size(), size_t(1));
  EXPECT_EQ(test_stack->top(), 42);
  test_stack->push(43);
  EXPECT_EQ(test_stack->size(), size_t(2));
  EXPECT_EQ(test_stack->top(), 43);
}

TEST_F(StackTest, Pop) {
  test_stack_filled->pop();
  EXPECT_EQ(test_stack_filled->size(), size_t(2));
  EXPECT_EQ(test_stack_filled->top(), 2);
  EXPECT_THROW(test_stack->pop(), std::runtime_error);
}

TEST_F(StackTest, Swap) {
  s21::stack<int> s1 = {1, 2, 3};
  s21::stack<int> s2 = {4, 5};
  s1.swap(s2);
  EXPECT_EQ(s1.size(), size_t(2));
  EXPECT_EQ(s2.size(), size_t(3));
  EXPECT_EQ(s1.top(), 5);
  EXPECT_EQ(s2.top(), 3);
}

TEST_F(StackTest, SwapEmpty) {
  s21::stack<int> s1;
  s21::stack<int> s2 = {1, 2};
  s1.swap(s2);
  EXPECT_EQ(s1.size(), size_t(2));
  EXPECT_EQ(s2.size(), size_t(0));
  EXPECT_EQ(s1.top(), 2);
  EXPECT_TRUE(s2.empty());
}

TEST_F(StackTest, SwapSelf) {
  s21::stack<int> s = {1, 2, 3};
  s.swap(s);
  EXPECT_EQ(s.size(), size_t(3));
  EXPECT_EQ(s.top(), 3);
}