#ifndef PROJECT_ARRAY_SCAN_HPP
#define PROJECT_ARRAY_SCAN_HPP

#include <simd-vector.hpp>

template <typename T>
T ScanSumSimd(T* data, size_t length) {
    SimdVector<T>(data, length).ScanSum();
    return data[length - 1];
}

template <typename T>
T ScanSumNaive(T* data, size_t length) {
    for (size_t id = 1; id < length; id++) {
        data[id] += data[id - 1];
    }
    return data[length - 1];
}

#endif  // PROJECT_ARRAY_SCAN_HPP
