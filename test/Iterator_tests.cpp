#include <gtest/gtest.h>
#include "Array.hpp"

TEST(IteratorTest, ForwardIteration) {
  Array<int> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);

  int expected = 1;
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    EXPECT_EQ(*it, expected);
    ++expected;
  }
}

TEST(IteratorTest, ConstIterator) {
  Array<int> arr;
  arr.push_back(10);
  arr.push_back(20);

  const Array<int>& const_arr = arr;

  auto it = const_arr.cbegin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
}

TEST(IteratorTest, RangeBasedFor) {
  Array<int> arr;
  arr.push_back(5);
  arr.push_back(6);
  arr.push_back(7);

  int sum = 0;
  for (int v : arr) {
    sum += v;
  }

  EXPECT_EQ(sum, 18);
}

TEST(IteratorTest, ReverseIterator) {
  Array<int> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);

  auto rit = arr.rbegin();
  EXPECT_EQ(*rit, 3);
  ++rit;
  EXPECT_EQ(*rit, 2);
  ++rit;
  EXPECT_EQ(*rit, 1);
}
