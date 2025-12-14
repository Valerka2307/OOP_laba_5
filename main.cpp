#include <iostream>
#include <memory_resource>

#include "Array.hpp"
#include "MemoryResource.hpp"

int main() {
  // Создаём собственный memory_resource на 1 КБ
  MemoryResource mem_res(1024);

  // polymorphic_allocator, привязанный к нашему ресурсу
  std::pmr::polymorphic_allocator<int> alloc(&mem_res);

  // Используем Array с этим аллокатором
  Array<int, std::pmr::polymorphic_allocator<int>> arr(0);
  arr = Array<int, std::pmr::polymorphic_allocator<int>>(0);
  
  // ВАЖНО: allocator хранится внутри Array,
  // поэтому лучше сразу создать через копирование
  Array<int, std::pmr::polymorphic_allocator<int>> array;
  
  for (int i = 0; i < 5; ++i) {
    array.push_back(i * 10);
  }

  std::cout << "Array contents: ";
  for (int x : array) {
    std::cout << x << " ";
  }
  std::cout << "\n";

  std::cout << "Size: " << array.size()
            << ", Capacity: " << array.capacity() << std::endl;

  return 0;
}
