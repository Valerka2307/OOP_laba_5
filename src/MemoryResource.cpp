#include <MemoryResource.hpp>

MemoryResource::MemoryResource(size_t total_size) : total_size_(total_size) {
  buffer_ = reinterpret_cast<uint8_t*>(::operator new(total_size_));
  allocation_data_.emplace_back(Chunk{total_size_, buffer_});
}

MemoryResource::~MemoryResource() {
  ::operator delete(buffer_);
}

void* MemoryResource::do_allocate(size_t bytes, size_t alignment) {
  for (auto& chunk : allocation_data_) {
    if (chunk.is_free && chunk.size >= bytes) {
      uintptr_t ptr_val = reinterpret_cast<uintptr_t>(chunk.ptr);
      size_t space = chunk.size;

      uintptr_t aligned_ptr_val = (ptr_val + alignment - 1) & ~(alignment - 1);
      size_t padding = aligned_ptr_val - ptr_val;

      if (space >= bytes + padding) {
        if (padding > 0) {
          allocation_data_.emplace_back(Chunk{padding, chunk.ptr});
          chunk.ptr += padding;
          chunk.size -= padding;
        }

        if (chunk.size > bytes) {
          allocation_data_.emplace_back(Chunk{chunk.size - bytes, chunk.ptr + bytes});
          chunk.size = bytes;
        }

        chunk.is_free = false;
        return reinterpret_cast<void*>(aligned_ptr_val);
      }
    }
  }
  throw std::bad_alloc();
}

void MemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
  uint8_t* ptr = reinterpret_cast<uint8_t*>(p);
  for (auto& chunk : allocation_data_) {
    if (chunk.ptr == ptr) {
      chunk.is_free = true;
      break;
    }
  }
}

bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
  return this == &other;
}