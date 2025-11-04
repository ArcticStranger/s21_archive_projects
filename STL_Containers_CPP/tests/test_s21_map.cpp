#include <gtest/gtest.h>

#include <map>
#include <string>

#include "../s21_containers.h"

// --- CONSTRUCTORS AND ASSIGNMENT ---
TEST(MapTest, DefaultConstructor) {
  s21::map<int, std::string> m;
  ASSERT_TRUE(m.empty());
  ASSERT_EQ(m.size(), 0u);
}

TEST(MapTest, InitializerListConstructor) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  ASSERT_FALSE(m.empty());
  ASSERT_EQ(m.size(), 3u);
  ASSERT_EQ(m.at(1), "one");
  ASSERT_EQ(m.at(3), "three");
}

TEST(MapTest, CopyConstructor) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2(m1);
  ASSERT_EQ(m1.size(), m2.size());
  ASSERT_EQ(m1.at(1), m2.at(1));
  m2.insert({3, "three"});
  ASSERT_NE(m1.size(), m2.size());
}

TEST(MapTest, MoveConstructor) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2(std::move(m1));
  ASSERT_EQ(m2.size(), 2u);
  ASSERT_TRUE(m1.empty());
  ASSERT_EQ(m2.at(1), "one");
}

TEST(MapTest, CopyAssignment) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2;
  m2.insert({99, "ninety-nine"});
  m2 = m1;
  ASSERT_EQ(m1.size(), m2.size());
  ASSERT_EQ(m1.at(2), m2.at(2));
}

TEST(MapTest, MoveAssignment) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2;
  m2 = std::move(m1);
  ASSERT_EQ(m2.size(), 2u);
  ASSERT_TRUE(m1.empty());
  ASSERT_EQ(m2.at(2), "two");
}

// --- ELEMENT ACCESS ---
TEST(MapTest, At) {
  s21::map<int, std::string> m = {{10, "ten"}, {20, "twenty"}};
  ASSERT_EQ(m.at(10), "ten");
  m.at(10) = "TEN";
  ASSERT_EQ(m.at(10), "TEN");
  ASSERT_THROW(m.at(99), std::out_of_range);
}

TEST(MapTest, AtConst) {
  const s21::map<int, std::string> m = {{10, "ten"}, {20, "twenty"}};
  ASSERT_EQ(m.at(20), "twenty");
  ASSERT_THROW(m.at(1), std::out_of_range);
}

TEST(MapTest, OperatorBrackets) {
  s21::map<int, std::string> m;
  m[1] = "one";
  ASSERT_EQ(m.size(), 1u);
  ASSERT_EQ(m[1], "one");
  m[1] = "ONE";
  ASSERT_EQ(m[1], "ONE");
  ASSERT_EQ(m.size(), 1u);
  ASSERT_EQ(m[2], "");
  ASSERT_EQ(m.size(), 2u);
}

// --- ITERATORS ---
TEST(MapTest, Iterators) {
  s21::map<int, std::string> m = {{3, "three"}, {1, "one"}, {2, "two"}};
  auto it = m.begin();
  ASSERT_EQ(it->first, 1);
  ASSERT_EQ(it->second, "one");
  ++it;
  ASSERT_EQ(it->first, 2);
  ++it;
  ASSERT_EQ(it->first, 3);
  ++it;
  ASSERT_EQ(it, m.end());
}

TEST(MapTest, ConstIterators) {
  const s21::map<int, std::string> m = {{2, "two"}, {1, "one"}};
  auto it = m.cbegin();
  ASSERT_EQ(it->first, 1);
  ++it;
  ASSERT_EQ(it->first, 2);
  ++it;
  ASSERT_EQ(it, m.cend());
}

// --- CAPACITY ---
TEST(MapTest, EmptyAndSize) {
  s21::map<int, int> m;
  ASSERT_TRUE(m.empty());
  ASSERT_EQ(m.size(), 0u);
  m.insert({1, 1});
  ASSERT_FALSE(m.empty());
  ASSERT_EQ(m.size(), 1u);
  m.clear();
  ASSERT_TRUE(m.empty());
  ASSERT_EQ(m.size(), 0u);
}

TEST(MapTest, MaxSize) {
  s21::map<int, int> m;
  ASSERT_GT(m.max_size(), 100000u);
}

// --- MODIFIERS ---
TEST(MapTest, Insert) {
  s21::map<int, std::string> m;
  auto res1 = m.insert({10, "ten"});
  ASSERT_TRUE(res1.second);
  ASSERT_EQ(res1.first->first, 10);
  ASSERT_EQ(m.size(), 1u);

  auto res2 = m.insert({10, "TEN"});
  ASSERT_FALSE(res2.second);
  ASSERT_EQ(res2.first->second, "ten");
  ASSERT_EQ(m.size(), 1u);

  auto res3 = m.insert(5, "five");
  ASSERT_TRUE(res3.second);
  ASSERT_EQ(m.size(), 2u);
  ASSERT_EQ(m.at(5), "five");
}

TEST(MapTest, InsertOrAssign) {
  s21::map<int, std::string> m;
  auto res1 = m.insert_or_assign(10, "ten");
  ASSERT_TRUE(res1.second);
  ASSERT_EQ(res1.first->second, "ten");
  ASSERT_EQ(m.size(), 1u);

  auto res2 = m.insert_or_assign(10, "TEN");
  ASSERT_FALSE(res2.second);
  ASSERT_EQ(res2.first->second, "TEN");
  ASSERT_EQ(m.size(), 1u);
  ASSERT_EQ(m.at(10), "TEN");
}

TEST(MapTest, Erase) {
  s21::map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
  auto it = m.find(3);
  m.erase(it);
  ASSERT_EQ(m.size(), 3u);
  ASSERT_FALSE(m.contains(3));
  ASSERT_EQ(m.find(3), m.end());
}

TEST(MapTest, Swap) {
  s21::map<int, int> m1 = {{1, 1}, {2, 2}};
  s21::map<int, int> m2 = {{3, 3}, {4, 4}, {5, 5}};
  m1.swap(m2);
  ASSERT_EQ(m1.size(), 3u);
  ASSERT_EQ(m2.size(), 2u);
  ASSERT_TRUE(m1.contains(4));
  ASSERT_TRUE(m2.contains(1));
}

TEST(MapTest, Merge) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = {{2, "TWO"}, {3, "three"}};
  m1.merge(m2);
  ASSERT_EQ(m1.size(), 3u);
  ASSERT_EQ(m1.at(1), "one");
  ASSERT_EQ(m1.at(2), "two");
  ASSERT_EQ(m1.at(3), "three");
  ASSERT_EQ(m2.size(), 1u);
  ASSERT_TRUE(m2.contains(2));
}

TEST(MapTest, Emplace) {
  s21::map<int, std::string> m;
  auto result = m.emplace(std::make_pair(10, "ten"));
  ASSERT_TRUE(result.second);
  ASSERT_EQ(result.first->first, 10);
  ASSERT_EQ(result.first->second, "ten");
  ASSERT_EQ(m.size(), 1u);
}

// --- LOOKUP ---
TEST(MapTest, FindAndContains) {
  s21::map<int, int> m = {{1, 1}, {10, 10}, {100, 100}};
  auto it = m.find(10);
  ASSERT_NE(it, m.end());
  ASSERT_EQ(it->second, 10);

  ASSERT_EQ(m.find(99), m.end());

  ASSERT_TRUE(m.contains(100));
  ASSERT_FALSE(m.contains(5));
}