#include <algorithm>
#include <iterator>
#include <memory>
#include <memory_resource>

template <typename T, typename Alloc = std::pmr::polymorphic_allocator<T>>
class Array {

 private:

  template <bool IsConst>
  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;

    Iterator(T* ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }

    Iterator& operator++() {
      ++ptr_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    Iterator& operator--() {
      --ptr_;
      return *this;
    }

    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    difference_type operator-(const Iterator& other) const {
      return ptr_ - other.ptr_;
    }

    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }

   private:
    T* ptr_;
  };

 public:
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using value_type = T;
  using allocator_type = Alloc;

  Array() : data_(nullptr), size_(0), capacity_(0) {}

  Array(size_t size, const T& value = T()) {
    data_ = allocator_.allocate(size * 2);
    for (size_t i = 0; i < size; ++i) {
      allocator_.construct(&data_[i], value);
    }
    size_ = size; 
    capacity_ = size * 2;
  }

  Array(std::initializer_list<T> lst) {
    size_ = lst.size();
    capacity_ = size_ * 2;
    data_ = allocator_.allocate(capacity_);
    size_t index = 0;
    for (const auto& item : lst) {
      allocator_.construct(&data_[index++], item);
    }
  }

  Array(const Array& other) {
    data_ = allocator_.allocate(other.capacity_);
    for (size_t i = 0; i < other.size_; ++i) {
      allocator_.construct(&data_[i], other.data_[i]);
    }
    size_ = other.size_;
    capacity_ = other.capacity_;
  }

  Array(Array&& other) noexcept 
      : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Array& operator=(const Array& other) {
    if (this != &other) {
      if (data_) {
        for (size_t i = 0; i < size_; ++i) {
          allocator_.destroy(&data_[i]);
        }
        allocator_.deallocate(data_, capacity_);
      }
      data_ = allocator_.allocate(other.capacity_);
      for (size_t i = 0; i < other.size_; ++i) {
        allocator_.construct(&data_[i], other.data_[i]);
      }
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }

  Array& operator=(Array&& other) noexcept {
    if (this != &other) {
      if (data_) {
        for (size_t i = 0; i < size_; ++i) {
          allocator_.destroy(&data_[i]);
        }
        allocator_.deallocate(data_, capacity_);
      }
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  T& operator[](size_t index) { return data_[index]; }

  const T& operator[](size_t index) const { return data_[index]; }

  size_t size() const { return size_; }

  size_t capacity() const { return capacity_; }

  void push_back(const T& value) {
    if (size_ >= capacity_) {
      size_t new_capacity = std::max<size_t>(1, capacity_ * 2);
      T* new_data = allocator_.allocate(new_capacity);

      for (size_t i = 0; i < size_; ++i) {
        allocator_.construct(&new_data[i], std::move(data_[i]));
        allocator_.destroy(&data_[i]);
      }
      if (data_) {
        allocator_.deallocate(data_, capacity_);
      }
      data_ = new_data;
      capacity_ = new_capacity;
    }
    allocator_.construct(&data_[size_], value);
    ++size_;
  }

  void pop_back() {
    if (size_ > 0) {
      --size_;
      allocator_.destroy(&data_[size_]);
    }
  }

  bool empty() const { return size_ == 0; }

  ~Array() {
    if (data_) {
      Alloc alloc;
      alloc.deallocate(data_, capacity_);
    }
  }


  iterator begin() noexcept { return iterator(data_); }
  iterator end() noexcept { return iterator(data_ + size_); }
    
  const_iterator begin() const noexcept { return const_iterator(data_); }
  const_iterator end() const noexcept { return const_iterator(data_ + size_); }
    
  const_iterator cbegin() const noexcept { return const_iterator(data_); }
  const_iterator cend() const noexcept { return const_iterator(data_ + size_); }
    
  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    
  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    
  const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
  const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


 private:
  T* data_;
  size_t size_;
  size_t capacity_;
  Alloc allocator_;
};