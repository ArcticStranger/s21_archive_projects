#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "../s21_containers.h"

TEST(SetTest, DefaultConstructor) {
  s21::set<int> s;
  ASSERT_TRUE(s.empty());
  ASSERT_EQ(s.size(), 0u);
}

TEST(SetTest, InitializerListConstructor) {
  s21::set<int> s = {1, 2, 3, 4, 3, 2, 1};
  ASSERT_FALSE(s.empty());
  ASSERT_EQ(s.size(), 4u);
  ASSERT_EQ(*s.begin(), 1);
  std::vector<int> expected = {1, 2, 3, 4};
  ASSERT_TRUE(std::equal(s.begin(), s.end(), expected.begin()));
}

TEST(SetTest, CopyConstructor) {
  s21::set<int> s1 = {10, 20, 30};
  s21::set<int> s2(s1);
  ASSERT_EQ(s1.size(), s2.size());
  ASSERT_TRUE(std::equal(s1.begin(), s1.end(), s2.begin()));
}

TEST(SetTest, MoveConstructor) {
  s21::set<int> s1 = {10, 20, 30};
  s21::set<int> s2(std::move(s1));
  ASSERT_EQ(s2.size(), 3u);
  ASSERT_TRUE(s1.empty());
  ASSERT_TRUE(s2.contains(20));
}

TEST(SetTest, CopyAssignment) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {4, 5};
  s2 = s1;
  ASSERT_EQ(s1.size(), s2.size());
  ASSERT_TRUE(s2.contains(2));
}

TEST(SetTest, MoveAssignment) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {4, 5};
  s2 = std::move(s1);
  ASSERT_EQ(s2.size(), 3u);
  ASSERT_TRUE(s1.empty());
  ASSERT_TRUE(s2.contains(3));
}

TEST(SetTest, Iterators) {
  s21::set<int> s = {40, 10, 30, 20};
  std::vector<int> expected = {10, 20, 30, 40};
  auto it = s.begin();
  for (int val : expected) {
    ASSERT_EQ(*it, val);
    ++it;
  }
  ASSERT_EQ(it, s.end());
}

TEST(SetTest, ConstIterators) {
  const s21::set<int> s = {40, 10, 30, 20};
  std::vector<int> expected = {10, 20, 30, 40};
  auto it = s.cbegin();
  for (int val : expected) {
    ASSERT_EQ(*it, val);
    ++it;
  }
  ASSERT_EQ(it, s.cend());
}

TEST(SetTest, Capacity) {
  s21::set<int> s;
  ASSERT_TRUE(s.empty());
  s.insert(1);
  ASSERT_FALSE(s.empty());
  ASSERT_EQ(s.size(), 1u);
  s.clear();
  ASSERT_TRUE(s.empty());
  ASSERT_GT(s.max_size(), 10000u);
}

TEST(SetTest, Insert) {
  s21::set<int> s;
  auto result1 = s.insert(20);
  ASSERT_TRUE(result1.second);
  ASSERT_EQ(*result1.first, 20);
  ASSERT_EQ(s.size(), 1u);

  auto result2 = s.insert(20);
  ASSERT_FALSE(result2.second);
  ASSERT_EQ(*result2.first, 20);
  ASSERT_EQ(s.size(), 1u);
}

TEST(SetTest, Erase) {
  s21::set<int> s = {10, 20, 30, 40, 50};
  auto it = s.find(30);
  it = s.erase(it);
  ASSERT_EQ(*it, 40);
  ASSERT_EQ(s.size(), 4u);
  ASSERT_FALSE(s.contains(30));

  it = s.find(10);
  it = s.erase(it);
  ASSERT_EQ(*it, 20);
  ASSERT_EQ(s.size(), 3u);
  ASSERT_FALSE(s.contains(10));
}

TEST(SetTest, Swap) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {4, 5};
  s1.swap(s2);
  ASSERT_EQ(s1.size(), 2u);
  ASSERT_EQ(s2.size(), 3u);
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s2.contains(1));
}

TEST(SetTest, Merge) {
  s21::set<int> s1 = {1, 2, 5};
  s21::set<int> s2 = {2, 3, 4};
  s1.merge(s2);
  ASSERT_EQ(s1.size(), 5u);
  ASSERT_TRUE(s1.contains(3));
  ASSERT_TRUE(s1.contains(4));
  ASSERT_EQ(s2.size(), 1u);
  ASSERT_TRUE(s2.contains(2));
}

TEST(SetTest, Emplace) {
  s21::set<std::string> s;
  auto result = s.emplace("hello");
  ASSERT_TRUE(result.second);
  ASSERT_EQ(*result.first, "hello");
  auto result2 = s.emplace("world!");
  ASSERT_TRUE(result2.second);
  ASSERT_EQ(s.size(), 2u);
}

TEST(SetTest, FindAndContains) {
  s21::set<int> s = {10, 20, 30};
  ASSERT_NE(s.find(20), s.end());
  ASSERT_EQ(s.find(40), s.end());
  ASSERT_TRUE(s.contains(30));
  ASSERT_FALSE(s.contains(50));
}