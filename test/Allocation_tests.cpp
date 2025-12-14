#include <gtest/gtest.h>
#include <memory_resource>

#include "Array.hpp"
#include "MemoryResource.hpp"

TEST(AllocatorTest, AllocationAndDeallocation) {
  MemoryResource mem_res(1024);
  std::pmr::polymorphic_allocator<int> alloc(&mem_res);

  Array<int, std::pmr::polymorphic_allocator<int>> arr;
  
  for (int i = 0; i < 10; ++i) {
    arr.push_back(i);
  }

  EXPECT_EQ(arr.size(), 10);

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(arr[i], i);
  }
}

TEST(AllocatorTest, PopBackWorks) {
  MemoryResource mem_res(512);
  std::pmr::polymorphic_allocator<int> alloc(&mem_res);

  Array<int, std::pmr::polymorphic_allocator<int>> arr;
  arr.push_back(1);
  arr.push_back(2);

  arr.pop_back();
  EXPECT_EQ(arr.size(), 1);
  EXPECT_EQ(arr[0], 1);
}

TEST(AllocatorTest, MultipleArraysSameResource) {
  MemoryResource mem_res(2048);
  std::pmr::polymorphic_allocator<int> alloc(&mem_res);

  Array<int, std::pmr::polymorphic_allocator<int>> a;
  Array<int, std::pmr::polymorphic_allocator<int>> b;

  a.push_back(1);
  b.push_back(2);

  EXPECT_EQ(a[0], 1);
  EXPECT_EQ(b[0], 2);
}
