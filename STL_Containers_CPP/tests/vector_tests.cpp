#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <vector>

#include "../s21_containers.h"

TEST(VectorConstructorsTest, DefaultConstructor) {
  s21::vector<int> v;
  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), 0u);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.data(), nullptr);
  EXPECT_EQ(v.begin(), nullptr);
  EXPECT_EQ(v.end(), nullptr);
}

TEST(VectorConstructorsTest, SizeConstructor) {
  s21::vector<int> v(5);
  EXPECT_EQ(v.size(), 5u);
  EXPECT_EQ(v.capacity(), 5u);
  EXPECT_FALSE(v.empty());
  EXPECT_NE(v.data(), nullptr);
  EXPECT_EQ(v.begin(), v.data());
  EXPECT_EQ(v.end(), v.data() + 5);

  for (auto i = v.begin(); i != v.end(); ++i) {
    EXPECT_EQ(*i, 0);
  }
}

TEST(VectorConstructorsTest, SizeConstructorZero) {
  s21::vector<int> v(0);
  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), 0u);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.data(), nullptr);
  EXPECT_EQ(v.begin(), nullptr);
  EXPECT_EQ(v.end(), nullptr);
}

TEST(VectorConstructorsTest, InitializerListConstructor) {
  s21::vector<int> v{1, 2, 3, 4, 5};
  EXPECT_EQ(v.size(), 5u);
  EXPECT_EQ(v.capacity(), 5u);
  EXPECT_FALSE(v.empty());
  EXPECT_NE(v.data(), nullptr);
  EXPECT_EQ(v.begin(), v.data());
  EXPECT_EQ(v.end(), v.data() + 5);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorConstructorsTest, InitializerListConstrcutorEmpty) {
  s21::vector<int> v{};
  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), 0u);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.data(), nullptr);
  EXPECT_EQ(v.begin(), nullptr);
  EXPECT_EQ(v.end(), nullptr);
}

TEST(VectorConstructorsTest, CopyConstructor) {
  s21::vector<int> v1{1, 2, 3};
  s21::vector<int> v2(v1);

  EXPECT_EQ(v2.size(), v1.size());
  EXPECT_EQ(v2.capacity(), v1.capacity());
  EXPECT_FALSE(v2.empty());
  EXPECT_NE(v2.data(), nullptr);
  EXPECT_EQ(v2.begin(), v2.data());
  EXPECT_EQ(v2.end(), v2.data() + v2.size());

  EXPECT_EQ(v2[0], v1[0]);
  EXPECT_EQ(v2[1], v1[1]);
  EXPECT_EQ(v2[2], v1[2]);
}

TEST(VectorConstructorsTest, EmptyCopyConstructor) {
  s21::vector<int> v1;
  s21::vector<int> v2(v1);

  EXPECT_EQ(v2.size(), v1.size());
  EXPECT_EQ(v2.capacity(), v1.capacity());
  EXPECT_TRUE(v2.empty());
  EXPECT_TRUE(v1.empty());
  EXPECT_EQ(v2.data(), nullptr);
  EXPECT_EQ(v2.begin(), v2.data());
  EXPECT_EQ(v2.end(), v2.data() + v2.size());
}

TEST(VectorConstructorsTest, MoveConstructor) {
  s21::vector<int> v1{1, 2, 3};
  s21::vector<int> v2(std::move(v1));

  EXPECT_EQ(v1.size(), 0u);
  EXPECT_EQ(v1.capacity(), 0u);
  EXPECT_EQ(v1.data(), nullptr);
  EXPECT_EQ(v2.begin(), v2.data());
  EXPECT_EQ(v2.end(), v2.data() + v2.size());
  EXPECT_EQ(v2.size(), 3u);
  EXPECT_EQ(v2.capacity(), 3u);

  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
  EXPECT_EQ(v2[2], 3);
}

TEST(VectorConstructorsTest, MoveEmptyConstructor) {
  s21::vector<int> v1;
  s21::vector<int> v2(std::move(v1));

  EXPECT_EQ(v1.size(), v2.size());

  EXPECT_EQ(v2.size(), 0u);
  EXPECT_EQ(v2.capacity(), 0u);
  EXPECT_EQ(v2.data(), nullptr);
  EXPECT_EQ(v2.begin(), v2.data());
}

TEST(VectorAssignmentTest, AssignmentOperatorBasic) {
  s21::vector<int> v1{1, 2, 3};
  s21::vector<int> v2{10, 20, 30, 40, 50};

  auto v2_original_data = v2.data();
  auto v2_original_size = v2.size();
  auto v2_original_capacity = v2.capacity();

  v1 = std::move(v2);

  EXPECT_EQ(v1.size(), v2_original_size);
  EXPECT_EQ(v1.capacity(), v2_original_capacity);
  EXPECT_EQ(v1.data(), v2_original_data);

  EXPECT_EQ(v1[0], 10);
  EXPECT_EQ(v1[1], 20);
  EXPECT_EQ(v1[2], 30);
  EXPECT_EQ(v1[3], 40);
  EXPECT_EQ(v1[4], 50);

  EXPECT_EQ(v2.size(), 0u);
  EXPECT_EQ(v2.capacity(), 0u);
  EXPECT_EQ(v2.data(), nullptr);
  EXPECT_TRUE(v2.empty());
}

TEST(VectorAssignmentTest, AssignmentOperatorEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2{100, 200, 300};

  auto v2_original_data = v2.data();
  auto v2_original_size = v2.size();

  v1 = std::move(v2);

  EXPECT_EQ(v1.size(), v2_original_size);
  EXPECT_EQ(v1.data(), v2_original_data);
  EXPECT_EQ(v1[0], 100);
  EXPECT_EQ(v1[1], 200);
  EXPECT_EQ(v1[2], 300);

  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.data(), nullptr);
}

TEST(VectorElementAccessTest, AtMethod) {
  s21::vector<int> v{10, 20, 30, 40, 50};

  EXPECT_EQ(v.at(0), 10);
  EXPECT_EQ(v.at(1), 20);
  EXPECT_EQ(v.at(2), 30);
  EXPECT_EQ(v.at(3), 40);
  EXPECT_EQ(v.at(4), 50);
}

TEST(VectorElementAccessTest, AtMethodOutOfRange) {
  s21::vector<int> v{1, 2, 3};

  EXPECT_THROW(v.at(3), std::out_of_range);
  EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorElementAccessTest, AtMethodEmptyVector) {
  s21::vector<int> v;

  EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(VectorElementAccessTest, BracketOperator) {
  s21::vector<int> v{10, 20, 30, 40, 50};

  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
  EXPECT_EQ(v[3], 40);
  EXPECT_EQ(v[4], 50);

  v[2] = 99;
  EXPECT_EQ(v[2], 99);
}

TEST(VectorElementAccessTest, FrontMethod) {
  s21::vector<int> v{10, 20, 30};

  EXPECT_EQ(v.front(), 10);

  s21::vector<int> single{42};
  EXPECT_EQ(single.front(), 42);
}

TEST(VectorElementAccessTest, FrontMethodEmptyVector) {
  s21::vector<int> v;

  EXPECT_THROW(v.front(), std::runtime_error);
}

TEST(VectorElementAccessTest, BackMethod) {
  s21::vector<int> v{10, 20, 30};

  EXPECT_EQ(v.back(), 30);

  s21::vector<int> single{42};
  EXPECT_EQ(single.back(), 42);
}

TEST(VectorElementAccessTest, BackMethodEmptyVector) {
  s21::vector<int> v;

  EXPECT_THROW(v.back(), std::runtime_error);
}

TEST(VectorElementAccessTest, DataMethod) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  int* ptr = v.data();
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(*ptr, 1);
  EXPECT_EQ(*(ptr + 1), 2);
  EXPECT_EQ(*(ptr + 4), 5);

  s21::vector<int> empty;
  EXPECT_EQ(empty.data(), nullptr);
}

TEST(VectorIteratorTest, BeginEnd) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  EXPECT_EQ(v.begin(), v.data());
  EXPECT_EQ(v.end(), v.data() + v.size());

  int expected = 1;
  for (auto it = v.begin(); it != v.end(); ++it) {
    EXPECT_EQ(*it, expected++);
  }
}

TEST(VectorIteratorTest, BeginEndEmpty) {
  s21::vector<int> v;

  EXPECT_EQ(v.begin(), v.end());
  EXPECT_EQ(v.begin(), nullptr);
  EXPECT_EQ(v.end(), nullptr);
}

TEST(VectorIteratorTest, RangeBasedFor) {
  s21::vector<int> v{10, 20, 30};

  int sum = 0;
  for (const auto& element : v) {
    sum += element;
  }
  EXPECT_EQ(sum, 60);
}

TEST(VectorCapacityTest, Empty) {
  s21::vector<int> v;
  EXPECT_TRUE(v.empty());

  v = s21::vector<int>{1, 2, 3};
  EXPECT_FALSE(v.empty());

  s21::vector<int> zero_size(0);
  EXPECT_TRUE(zero_size.empty());
}

TEST(VectorCapacityTest, Size) {
  s21::vector<int> v;
  EXPECT_EQ(v.size(), 0u);

  s21::vector<int> v1(5);
  EXPECT_EQ(v1.size(), 5u);

  s21::vector<int> v2{1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(v2.size(), 7u);
}

TEST(VectorCapacityTest, MaxSize) {
  s21::vector<int> v;
  EXPECT_GT(v.max_size(), 0u);
  EXPECT_EQ(v.max_size(),
            std::numeric_limits<std::size_t>::max() / sizeof(int));

  s21::vector<double> v_double;
  EXPECT_EQ(v_double.max_size(),
            std::numeric_limits<std::size_t>::max() / sizeof(double));
}

TEST(VectorCapacityTest, Reserve) {
  s21::vector<int> v{1, 2, 3};
  auto original_data = v.data();

  v.reserve(2);
  EXPECT_EQ(v.capacity(), 3u);
  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v.data(), original_data);

  v.reserve(10);
  EXPECT_EQ(v.capacity(), 10u);
  EXPECT_EQ(v.size(), 3u);
  EXPECT_NE(v.data(), original_data);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorCapacityTest, ReserveEmpty) {
  s21::vector<int> v;

  v.reserve(5);
  EXPECT_EQ(v.capacity(), 5u);
  EXPECT_EQ(v.size(), 0u);
  EXPECT_NE(v.data(), nullptr);
}

TEST(VectorCapacityTest, ReserveThrowsOnTooLarge) {
  s21::vector<int> v;

  EXPECT_THROW(v.reserve(v.max_size() + 1), std::out_of_range);
}

TEST(VectorCapacityTest, Capacity) {
  s21::vector<int> v;
  EXPECT_EQ(v.capacity(), 0u);

  s21::vector<int> v1(5);
  EXPECT_EQ(v1.capacity(), 5u);

  s21::vector<int> v2{1, 2, 3};
  EXPECT_EQ(v2.capacity(), 3u);
}

TEST(VectorCapacityTest, ShrinkToFit) {
  s21::vector<int> v{1, 2, 3, 4, 5};
  v.reserve(20);

  EXPECT_EQ(v.capacity(), 20u);
  EXPECT_EQ(v.size(), 5u);

  v.shrink_to_fit();

  EXPECT_EQ(v.capacity(), 5u);
  EXPECT_EQ(v.size(), 5u);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorCapacityTest, ShrinkToFitNoChange) {
  s21::vector<int> v{1, 2, 3};
  auto original_data = v.data();

  v.shrink_to_fit();

  EXPECT_EQ(v.capacity(), 3u);
  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v.data(), original_data);
}

TEST(VectorModifiersTest, Clear) {
  s21::vector<int> v{1, 2, 3, 4, 5};
  auto original_capacity = v.capacity();
  auto original_data = v.data();

  v.clear();

  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), original_capacity);
  EXPECT_EQ(v.data(), original_data);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, ClearEmpty) {
  s21::vector<int> v;

  v.clear();

  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), 0u);
  EXPECT_EQ(v.data(), nullptr);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, ClearAfterReserve) {
  s21::vector<int> v{1, 2, 3};
  v.reserve(10);
  auto original_capacity = v.capacity();

  v.clear();

  EXPECT_EQ(v.size(), 0u);
  EXPECT_EQ(v.capacity(), original_capacity);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, InsertAtBeginning) {
  s21::vector<int> v{2, 3, 4};

  auto it = v.insert(v.begin(), 1);

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, v.begin());

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiersTest, InsertAtEnd) {
  s21::vector<int> v{1, 2, 3};

  auto it = v.insert(v.end(), 4);

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(*it, 4);
  EXPECT_EQ(it, v.end() - 1);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiersTest, InsertInMiddle) {
  s21::vector<int> v{1, 2, 4, 5};

  auto it = v.insert(v.begin() + 2, 3);

  EXPECT_EQ(v.size(), 5u);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(it, v.begin() + 2);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorModifiersTest, InsertIntoEmpty) {
  s21::vector<int> v;

  auto it = v.insert(v.begin(), 42);

  EXPECT_EQ(v.size(), 1u);
  EXPECT_EQ(*it, 42);
  EXPECT_EQ(it, v.begin());
  EXPECT_EQ(v[0], 42);
}

TEST(VectorModifiersTest, InsertWithReallocation) {
  s21::vector<int> v{1, 2, 3};
  auto original_capacity = v.capacity();

  while (v.size() < v.capacity()) {
    v.insert(v.end(), 0);
  }

  auto it = v.insert(v.begin(), 99);

  EXPECT_GT(v.capacity(), original_capacity);
  EXPECT_EQ(*it, 99);
  EXPECT_EQ(v[0], 99);
}

TEST(VectorModifiersTest, InsertOutOfRange) {
  s21::vector<int> v{1, 2, 3};

  EXPECT_THROW(v.insert(v.end() + 1, 4), std::out_of_range);
  EXPECT_THROW(v.insert(v.begin() + 10, 4), std::out_of_range);
}

TEST(VectorModifiersTest, InsertMultiple) {
  s21::vector<int> v{1, 4};

  auto it1 = v.insert(v.begin() + 1, 2);
  EXPECT_EQ(*it1, 2);

  auto it2 = v.insert(v.begin() + 2, 3);
  EXPECT_EQ(*it2, 3);

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiersTest, EraseFromBeginning) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  v.erase(v.begin());

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v[2], 4);
  EXPECT_EQ(v[3], 5);
}

TEST(VectorModifiersTest, EraseFromEnd) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  v.erase(v.end() - 1);

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiersTest, EraseFromMiddle) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  v.erase(v.begin() + 2);

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 4);
  EXPECT_EQ(v[3], 5);
}

TEST(VectorModifiersTest, EraseSingleElement) {
  s21::vector<int> v{42};

  v.erase(v.begin());

  EXPECT_EQ(v.size(), 0u);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, EraseOutOfRange) {
  s21::vector<int> v{1, 2, 3};

  EXPECT_THROW(v.erase(v.end()), std::out_of_range);
  EXPECT_THROW(v.erase(v.begin() + 10), std::out_of_range);
}

TEST(VectorModifiersTest, EraseEmptyVector) {
  s21::vector<int> v;

  EXPECT_THROW(v.erase(v.begin()), std::out_of_range);
}

TEST(VectorModifiersTest, EraseMultiple) {
  s21::vector<int> v{1, 2, 3, 4, 5, 6};

  v.erase(v.begin() + 1);
  EXPECT_EQ(v.size(), 5u);
  EXPECT_EQ(v[1], 3);

  v.erase(v.begin() + 2);
  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v[2], 5);
  EXPECT_EQ(v[3], 6);
}

TEST(VectorModifiersTest, PushBackToEmpty) {
  s21::vector<int> v;

  v.push_back(42);

  EXPECT_EQ(v.size(), 1u);
  EXPECT_GE(v.capacity(), 1u);
  EXPECT_EQ(v[0], 42);
  EXPECT_FALSE(v.empty());
}

TEST(VectorModifiersTest, PushBackMultiple) {
  s21::vector<int> v;

  for (int i = 1; i <= 5; ++i) {
    v.push_back(i * 10);
  }

  EXPECT_EQ(v.size(), 5u);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(v[i], (i + 1) * 10);
  }
}

TEST(VectorModifiersTest, PushBackWithReallocation) {
  s21::vector<int> v{1};
  auto initial_capacity = v.capacity();

  while (v.size() < v.capacity()) {
    v.push_back(v.size() + 1);
  }

  auto size_before = v.size();
  v.push_back(999);

  EXPECT_EQ(v.size(), size_before + 1);
  EXPECT_GT(v.capacity(), initial_capacity);
  EXPECT_EQ(v.back(), 999);
}

TEST(VectorModifiersTest, PushBackCapacityGrowth) {
  s21::vector<int> v;
  std::vector<size_t> capacities;

  for (int i = 0; i < 10; ++i) {
    if (i == 0 || v.size() == v.capacity()) {
      capacities.push_back(v.capacity());
    }
    v.push_back(i);
  }

  EXPECT_GT(capacities.size(), 1u);
  for (size_t i = 1; i < capacities.size(); ++i) {
    EXPECT_GT(capacities[i], capacities[i - 1]);
  }
}

TEST(VectorModifiersTest, PopBack) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  v.pop_back();

  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiersTest, PopBackSingleElement) {
  s21::vector<int> v{42};

  v.pop_back();

  EXPECT_EQ(v.size(), 0u);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, PopBackEmpty) {
  s21::vector<int> v;

  EXPECT_NO_THROW(v.pop_back());
  EXPECT_EQ(v.size(), 0u);
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiersTest, PopBackMultiple) {
  s21::vector<int> v{10, 20, 30, 40, 50};

  v.pop_back();
  v.pop_back();

  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
}

TEST(VectorModifiersTest, PushPopCombination) {
  s21::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.pop_back();
  v.push_back(3);

  EXPECT_EQ(v.size(), 2u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
}

TEST(VectorModifiersTest, SwapBasic) {
  s21::vector<int> v1{1, 2, 3};
  s21::vector<int> v2{4, 5, 6, 7, 8};

  auto v1_original_data = v1.data();
  auto v1_original_size = v1.size();
  auto v1_original_capacity = v1.capacity();

  auto v2_original_data = v2.data();
  auto v2_original_size = v2.size();
  auto v2_original_capacity = v2.capacity();

  v1.swap(v2);

  EXPECT_EQ(v1.data(), v2_original_data);
  EXPECT_EQ(v1.size(), v2_original_size);
  EXPECT_EQ(v1.capacity(), v2_original_capacity);

  EXPECT_EQ(v2.data(), v1_original_data);
  EXPECT_EQ(v2.size(), v1_original_size);
  EXPECT_EQ(v2.capacity(), v1_original_capacity);

  EXPECT_EQ(v1[0], 4);
  EXPECT_EQ(v1[1], 5);
  EXPECT_EQ(v1[2], 6);
  EXPECT_EQ(v1[3], 7);
  EXPECT_EQ(v1[4], 8);

  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
  EXPECT_EQ(v2[2], 3);
};

TEST(VectorModifiersTest, SwapWithEmpty) {
  s21::vector<int> v1{1, 2, 3, 4, 5};
  s21::vector<int> v2;

  auto v1_original_data = v1.data();
  auto v1_original_size = v1.size();
  auto v1_original_capacity = v1.capacity();

  v1.swap(v2);

  EXPECT_EQ(v1.data(), nullptr);
  EXPECT_EQ(v1.size(), 0u);
  EXPECT_EQ(v1.capacity(), 0u);
  EXPECT_TRUE(v1.empty());

  EXPECT_EQ(v2.data(), v1_original_data);
  EXPECT_EQ(v2.size(), v1_original_size);
  EXPECT_EQ(v2.capacity(), v1_original_capacity);
  EXPECT_FALSE(v2.empty());

  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
  EXPECT_EQ(v2[2], 3);
  EXPECT_EQ(v2[3], 4);
  EXPECT_EQ(v2[4], 5);
}

TEST(VectorModifiersTest, SwapBothEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2;

  v1.swap(v2);

  EXPECT_TRUE(v1.empty());
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v1.data(), nullptr);
  EXPECT_EQ(v2.data(), nullptr);
  EXPECT_EQ(v1.size(), 0u);
  EXPECT_EQ(v2.size(), 0u);
  EXPECT_EQ(v1.capacity(), 0u);
  EXPECT_EQ(v2.capacity(), 0u);
}

TEST(VectorModifiersTest, SwapSelf) {
  s21::vector<int> v{1, 2, 3, 4, 5};

  auto original_data = v.data();
  auto original_size = v.size();
  auto original_capacity = v.capacity();

  v.swap(v);

  EXPECT_EQ(v.data(), original_data);
  EXPECT_EQ(v.size(), original_size);
  EXPECT_EQ(v.capacity(), original_capacity);

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorModifiersTest, SwapDifferentCapacities) {
  s21::vector<int> v1{1, 2};
  s21::vector<int> v2{3, 4, 5, 6};

  v1.reserve(10);

  auto v1_capacity = v1.capacity();
  auto v2_capacity = v2.capacity();

  v1.swap(v2);

  EXPECT_EQ(v1.size(), 4u);
  EXPECT_EQ(v1.capacity(), v2_capacity);
  EXPECT_EQ(v2.size(), 2u);
  EXPECT_EQ(v2.capacity(), v1_capacity);

  EXPECT_EQ(v1[0], 3);
  EXPECT_EQ(v1[1], 4);
  EXPECT_EQ(v1[2], 5);
  EXPECT_EQ(v1[3], 6);

  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
}

TEST(VectorModifiersTest, ComplexOperationsSequence) {
  s21::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  EXPECT_EQ(v.size(), 3u);

  v.insert(v.begin() + 1, 99);
  EXPECT_EQ(v.size(), 4u);
  EXPECT_EQ(v[1], 99);

  v.erase(v.begin());
  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], 99);

  v.pop_back();
  EXPECT_EQ(v.size(), 2u);

  v.clear();
  EXPECT_EQ(v.size(), 0u);
  EXPECT_TRUE(v.empty());

  v.push_back(100);
  EXPECT_EQ(v.size(), 1u);
  EXPECT_EQ(v[0], 100);
}