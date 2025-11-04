#include <gtest/gtest.h>

#include <queue>
#include <stdexcept>

#include "../s21_containers.h"

class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_queue = new s21::queue<int>();
    test_queue_filled = new s21::queue<int>{1, 2, 3, 4, 5};
  }

  void TearDown() override {
    delete test_queue;
    delete test_queue_filled;
  }

  s21::queue<int>* test_queue;
  s21::queue<int>* test_queue_filled;
};

TEST_F(QueueTest, DefaultConstructor) {
  EXPECT_EQ(test_queue->size(), size_t(0));
  EXPECT_TRUE(test_queue->empty());
}

TEST_F(QueueTest, SizeConstructor) {
  s21::queue<int> q(size_t(3));
  EXPECT_EQ(q.size(), size_t(3));
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 0);
  q.pop();
  EXPECT_EQ(q.front(), 0);
  q.pop();
  EXPECT_EQ(q.front(), 0);
}

TEST_F(QueueTest, InitializerListConstructor) {
  s21::queue<int> q = {1, 2, 3};
  EXPECT_EQ(q.size(), size_t(3));
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}

TEST_F(QueueTest, CopyConstructor) {
  s21::queue<int> q = *test_queue_filled;
  EXPECT_EQ(q.size(), test_queue_filled->size());
  EXPECT_EQ(q.front(), test_queue_filled->front());
  EXPECT_EQ(q.back(), test_queue_filled->back());
  for (int i = 1; i <= 5; ++i) {
    EXPECT_EQ(q.front(), i);
    q.pop();
  }
}

TEST_F(QueueTest, MoveConstructor) {
  s21::queue<int> temp = *test_queue_filled;
  s21::queue<int> q(std::move(temp));
  EXPECT_EQ(q.size(), size_t(5));
  EXPECT_EQ(temp.size(), size_t(0));
  EXPECT_TRUE(temp.empty());
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 5);
}

TEST_F(QueueTest, CopyAssignment) {
  s21::queue<int> q;
  q = *test_queue_filled;
  EXPECT_EQ(q.size(), test_queue_filled->size());
  EXPECT_EQ(q.front(), test_queue_filled->front());
  EXPECT_EQ(q.back(), test_queue_filled->back());
  for (int i = 1; i <= 5; ++i) {
    EXPECT_EQ(q.front(), i);
    q.pop();
  }
}

TEST_F(QueueTest, MoveAssignment) {
  s21::queue<int> temp = *test_queue_filled;
  s21::queue<int> q;
  q = std::move(temp);
  EXPECT_EQ(q.size(), size_t(5));
  EXPECT_EQ(temp.size(), size_t(0));
  EXPECT_TRUE(temp.empty());
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 5);
}

TEST_F(QueueTest, Front) {
  EXPECT_EQ(test_queue_filled->front(), 1);
  EXPECT_THROW(test_queue->front(), std::runtime_error);
}

TEST_F(QueueTest, Back) {
  EXPECT_EQ(test_queue_filled->back(), 5);
  EXPECT_THROW(test_queue->back(), std::runtime_error);
}

TEST_F(QueueTest, Size) {
  EXPECT_EQ(test_queue->size(), size_t(0));
  EXPECT_EQ(test_queue_filled->size(), size_t(5));
}

TEST_F(QueueTest, Empty) {
  EXPECT_TRUE(test_queue->empty());
  EXPECT_FALSE(test_queue_filled->empty());
}

TEST_F(QueueTest, Push) {
  test_queue->push(42);
  EXPECT_EQ(test_queue->size(), size_t(1));
  EXPECT_EQ(test_queue->front(), 42);
  EXPECT_EQ(test_queue->back(), 42);
  test_queue->push(43);
  EXPECT_EQ(test_queue->size(), size_t(2));
  EXPECT_EQ(test_queue->front(), 42);
  EXPECT_EQ(test_queue->back(), 43);
}

TEST_F(QueueTest, Pop) {
  test_queue_filled->pop();
  EXPECT_EQ(test_queue_filled->size(), size_t(4));
  EXPECT_EQ(test_queue_filled->front(), 2);
  EXPECT_EQ(test_queue_filled->back(), 5);
  EXPECT_THROW(test_queue->pop(), std::runtime_error);
  EXPECT_TRUE(test_queue->empty());
}

TEST_F(QueueTest, Swap) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = {4, 5};
  q1.swap(q2);
  EXPECT_EQ(q1.size(), size_t(2));
  EXPECT_EQ(q2.size(), size_t(3));
  EXPECT_EQ(q1.front(), 4);
  EXPECT_EQ(q1.back(), 5);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

TEST_F(QueueTest, SwapEmpty) {
  s21::queue<int> q1;
  s21::queue<int> q2 = {1, 2};
  q1.swap(q2);
  EXPECT_EQ(q1.size(), size_t(2));
  EXPECT_EQ(q2.size(), size_t(0));
  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 2);
  EXPECT_TRUE(q2.empty());
}

TEST_F(QueueTest, SwapSelf) {
  s21::queue<int> q = {1, 2, 3};
  q.swap(q);
  EXPECT_EQ(q.size(), size_t(3));
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}