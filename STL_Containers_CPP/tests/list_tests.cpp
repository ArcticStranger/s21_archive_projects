#include <gtest/gtest.h>

#include <list>
#include <stdexcept>

#include "../s21_containers.h"

class ListTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_list = new s21::list<int>();
    test_list_filled = new s21::list<int>{1, 2, 3, 4, 5};
  }

  void TearDown() override {
    delete test_list;
    delete test_list_filled;
  }

  s21::list<int>* test_list;
  s21::list<int>* test_list_filled;
};

TEST_F(ListTest, DefaultConstructor) {
  EXPECT_EQ(test_list->size(), size_t(0));
  EXPECT_TRUE(test_list->empty());
}

TEST_F(ListTest, SizeConstructor) {
  s21::list<int> l(size_t(3));
  EXPECT_EQ(l.size(), size_t(3));
  for (auto it = l.begin(); it != l.end(); ++it) {
    EXPECT_EQ(*it, 0);
  }
}

TEST_F(ListTest, InitializerListConstructor) {
  s21::list<int> l = {1, 2, 3};
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, CopyConstructor) {
  s21::list<int> l = *test_list_filled;
  EXPECT_EQ(l.size(), test_list_filled->size());
  auto it1 = l.begin();
  auto it2 = test_list_filled->begin();
  for (; it1 != l.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
}

TEST_F(ListTest, MoveConstructor) {
  s21::list<int> temp = *test_list_filled;
  s21::list<int> l(std::move(temp));
  EXPECT_EQ(l.size(), size_t(5));
  EXPECT_EQ(temp.size(), size_t(0));
  auto it = l.begin();
  for (int i = 1; i <= 5; ++i, ++it) {
    EXPECT_EQ(*it, i);
  }
}

TEST_F(ListTest, MoveAssignment) {
  s21::list<int> temp = *test_list_filled;
  s21::list<int> l;
  l = std::move(temp);
  EXPECT_EQ(l.size(), size_t(5));
  EXPECT_EQ(temp.size(), size_t(0));
  auto it = l.begin();
  for (int i = 1; i <= 5; ++i, ++it) {
    EXPECT_EQ(*it, i);
  }
}

TEST_F(ListTest, Front) {
  EXPECT_EQ(test_list_filled->front(), 1);
  EXPECT_THROW(test_list->front(), std::out_of_range);
}

TEST_F(ListTest, Back) {
  EXPECT_EQ(test_list_filled->back(), 5);
  EXPECT_THROW(test_list->back(), std::out_of_range);
}

TEST_F(ListTest, BeginEnd) {
  auto it = test_list_filled->begin();
  EXPECT_EQ(*it, 1);
  it = test_list_filled->end();
  --it;
  EXPECT_EQ(*it, 5);
}

TEST_F(ListTest, IteratorOperations) {
  auto it = test_list_filled->begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
  auto it2 = it++;
  EXPECT_EQ(*it2, 1);
  EXPECT_EQ(*it, 2);
}

TEST_F(ListTest, Size) {
  EXPECT_EQ(test_list->size(), size_t(0));
  EXPECT_EQ(test_list_filled->size(), size_t(5));
}

TEST_F(ListTest, Empty) {
  EXPECT_TRUE(test_list->empty());
  EXPECT_FALSE(test_list_filled->empty());
}

TEST_F(ListTest, MaxSize) { EXPECT_GT(test_list->max_size(), size_t(0)); }

TEST_F(ListTest, Clear) {
  test_list_filled->clear();
  EXPECT_EQ(test_list_filled->size(), size_t(0));
  EXPECT_TRUE(test_list_filled->empty());
}

TEST_F(ListTest, PushBack) {
  test_list->push_back(42);
  EXPECT_EQ(test_list->size(), size_t(1));
  EXPECT_EQ(test_list->front(), 42);
  EXPECT_EQ(test_list->back(), 42);
}

TEST_F(ListTest, PushFront) {
  test_list->push_front(42);
  EXPECT_EQ(test_list->size(), size_t(1));
  EXPECT_EQ(test_list->front(), 42);
  EXPECT_EQ(test_list->back(), 42);
}

TEST_F(ListTest, PopBack) {
  test_list_filled->pop_back();
  EXPECT_EQ(test_list_filled->size(), size_t(4));
  EXPECT_EQ(test_list_filled->back(), 4);
  test_list->pop_back();
  EXPECT_TRUE(test_list->empty());
}

TEST_F(ListTest, PopFront) {
  test_list_filled->pop_front();
  EXPECT_EQ(test_list_filled->size(), size_t(4));
  EXPECT_EQ(test_list_filled->front(), 2);
  test_list->pop_front();
  EXPECT_TRUE(test_list->empty());
}

TEST_F(ListTest, Insert) {
  auto it = test_list_filled->begin();
  ++it;
  test_list_filled->insert(it, 42);
  EXPECT_EQ(test_list_filled->size(), size_t(6));
  auto new_it = test_list_filled->begin();
  ++new_it;
  EXPECT_EQ(*new_it, 42);
}

TEST_F(ListTest, Erase) {
  auto it = test_list_filled->begin();
  ++it;
  test_list_filled->erase(it);
  EXPECT_EQ(test_list_filled->size(), size_t(4));
  auto new_it = test_list_filled->begin();
  ++new_it;
  EXPECT_EQ(*new_it, 3);
}

TEST_F(ListTest, Swap) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2 = {4, 5};
  l1.swap(l2);
  EXPECT_EQ(l1.size(), size_t(2));
  EXPECT_EQ(l2.size(), size_t(3));
  auto it1 = l1.begin();
  EXPECT_EQ(*it1, 4);
  ++it1;
  EXPECT_EQ(*it1, 5);
  auto it2 = l2.begin();
  EXPECT_EQ(*it2, 1);
  ++it2;
  EXPECT_EQ(*it2, 2);
  ++it2;
  EXPECT_EQ(*it2, 3);
}

TEST_F(ListTest, SwapEmpty) {
  s21::list<int> l1;
  s21::list<int> l2 = {1, 2};
  l1.swap(l2);
  EXPECT_EQ(l1.size(), size_t(2));
  EXPECT_EQ(l2.size(), size_t(0));
  auto it = l1.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  EXPECT_TRUE(l2.empty());
}

TEST_F(ListTest, SwapSelf) {
  s21::list<int> l = {1, 2, 3};
  l.swap(l);
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, Merge) {
  s21::list<int> l1 = {1, 3, 5};
  s21::list<int> l2 = {2, 4, 6};
  l1.merge(l2);
  EXPECT_EQ(l1.size(), size_t(6));
  EXPECT_EQ(l2.size(), size_t(0));
  auto it = l1.begin();
  for (int i = 1; i <= 6; ++i) {
    EXPECT_EQ(*it, i);
    ++it;
  }
}

TEST_F(ListTest, MergeEmpty) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2;
  l1.merge(l2);
  EXPECT_EQ(l1.size(), size_t(3));
  EXPECT_EQ(l2.size(), size_t(0));
  auto it = l1.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, MergeSelf) {
  s21::list<int> l = {1, 2, 3};
  l.merge(l);
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, Splice) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2 = {4, 5};
  auto it = l1.begin();
  ++it;
  l1.splice(it, l2);
  EXPECT_EQ(l1.size(), size_t(5));
  EXPECT_EQ(l2.size(), size_t(0));
  auto it1 = l1.begin();
  EXPECT_EQ(*it1, 1);
  ++it1;
  EXPECT_EQ(*it1, 4);
  ++it1;
  EXPECT_EQ(*it1, 5);
  ++it1;
  EXPECT_EQ(*it1, 2);
  ++it1;
  EXPECT_EQ(*it1, 3);
}

TEST_F(ListTest, SpliceEmpty) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2;
  auto it = l1.begin();
  l1.splice(it, l2);
  EXPECT_EQ(l1.size(), size_t(3));
  EXPECT_EQ(l2.size(), size_t(0));
  auto it1 = l1.begin();
  EXPECT_EQ(*it1, 1);
  ++it1;
  EXPECT_EQ(*it1, 2);
  ++it1;
  EXPECT_EQ(*it1, 3);
}

TEST_F(ListTest, SpliceSelf) {
  s21::list<int> l = {1, 2, 3};
  auto it = l.begin();
  ++it;
  l.splice(it, l);
  EXPECT_EQ(l.size(), size_t(3));
  auto it1 = l.begin();
  EXPECT_EQ(*it1, 1);
  ++it1;
  EXPECT_EQ(*it1, 2);
  ++it1;
  EXPECT_EQ(*it1, 3);
}

TEST_F(ListTest, Reverse) {
  s21::list<int> l = {1, 2, 3, 4};
  l.reverse();
  EXPECT_EQ(l.size(), size_t(4));
  auto it = l.begin();
  EXPECT_EQ(*it, 4);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 1);
}

TEST_F(ListTest, ReverseEmpty) {
  s21::list<int> l;
  l.reverse();
  EXPECT_EQ(l.size(), size_t(0));
  EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, ReverseSingle) {
  s21::list<int> l = {1};
  l.reverse();
  EXPECT_EQ(l.size(), size_t(1));
  EXPECT_EQ(l.front(), 1);
}

TEST_F(ListTest, Unique) {
  s21::list<int> l = {1, 1, 2, 2, 3, 3, 3};
  l.unique();
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, UniqueEmpty) {
  s21::list<int> l;
  l.unique();
  EXPECT_EQ(l.size(), size_t(0));
  EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, UniqueSingle) {
  s21::list<int> l = {1};
  l.unique();
  EXPECT_EQ(l.size(), size_t(1));
  EXPECT_EQ(l.front(), 1);
}

TEST_F(ListTest, UniqueNoDuplicates) {
  s21::list<int> l = {1, 2, 3};
  l.unique();
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST_F(ListTest, Sort) {
  s21::list<int> l = {3, 1, 4, 1, 5, 9, 2, 6, 5};
  l.sort();
  EXPECT_EQ(l.size(), size_t(9));
  auto it = l.begin();
  int expected[] = {1, 1, 2, 3, 4, 5, 5, 6, 9};
  for (size_t i = 0; i < 9; ++i) {
    EXPECT_EQ(*it, expected[i]);
    ++it;
  }
}

TEST_F(ListTest, SortEmpty) {
  s21::list<int> l;
  l.sort();
  EXPECT_EQ(l.size(), size_t(0));
  EXPECT_TRUE(l.empty());
}

TEST_F(ListTest, SortSingle) {
  s21::list<int> l = {1};
  l.sort();
  EXPECT_EQ(l.size(), size_t(1));
  EXPECT_EQ(l.front(), 1);
}

TEST_F(ListTest, SortSorted) {
  s21::list<int> l = {1, 2, 3};
  l.sort();
  EXPECT_EQ(l.size(), size_t(3));
  auto it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}