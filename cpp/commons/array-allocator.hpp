#ifndef PROJECT_ARRAY_ALLOCATOR_HPP
#define PROJECT_ARRAY_ALLOCATOR_HPP
#include <cstddef>
#include <cstdlib>

template <typename T>
class ArrayAllocator {
private:
    T *array_;
    size_t array_size_;

public:
    ArrayAllocator(size_t array_size) : array_(nullptr), array_size_(array_size) {
        array_ = reinterpret_cast<T *>(aligned_alloc(256 / 8, (array_size + 1) * sizeof(T)));
    }
    ArrayAllocator(const ArrayAllocator &other) = delete;
    ArrayAllocator(ArrayAllocator &&other) = delete;
    ArrayAllocator &operator=(const ArrayAllocator &other) = delete;
    ArrayAllocator &operator=(ArrayAllocator &&other) = delete;
    T *GetAligned() {
        return array_;
    }
    T *GetUnaligned() {
        return array_ + 1;
    }
    ~ArrayAllocator() {
        free(array_);
    }
};

#endif  // PROJECT_ARRAY_ALLOCATOR_HPP
