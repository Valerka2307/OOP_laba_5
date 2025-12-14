#include <memory_resource>
#include <list>

class MemoryResource : public std::pmr::memory_resource {
 public:
  MemoryResource(size_t total_size);
  ~MemoryResource() override;
 private:
  struct Chunk {
    Chunk(size_t size, uint8_t* ptr) : size(size), ptr(ptr), is_free(true) {}
    size_t size;
    uint8_t* ptr;
    bool is_free;
  };
 protected:
  void* do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void* p, size_t bytes, size_t alignment) override;
  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

  uint8_t* buffer_;
  std::list<Chunk> allocation_data_;
  size_t total_size_;  
};