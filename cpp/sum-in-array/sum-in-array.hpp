#ifndef CPP_SUM_IN_ARRAY_HPP
#define CPP_SUM_IN_ARRAY_HPP

#include <immintrin.h>
#include <iostream>
#include <simd-vector.hpp>

int64_t SumArraySimdUnaligned(long long *data, size_t length) {
    auto sum_block = _mm256_setzero_si256();
    for (size_t id = 0; id < length; id += 4) {
        auto current_block = _mm256_loadu_si256((__m256i *)(data + id));
        sum_block = _mm256_add_epi64(sum_block, current_block);
    }
    auto block_sums = _mm256_extract_epi64(sum_block, 0) + _mm256_extract_epi64(sum_block, 1) + _mm256_extract_epi64(sum_block, 2) +
                      _mm256_extract_epi64(sum_block, 3);
    for (size_t id = length - length % 4; id < length; id++) {
        block_sums += data[id];
    }
    return block_sums;
}

int64_t SumArraySimd(long long *data, size_t length) {
    auto sum_block = _mm256_setzero_si256();
    for (size_t id = 0; id < length; id += 4) {
        auto current_block = _mm256_load_si256((__m256i *)(data + id));
        sum_block = _mm256_add_epi64(sum_block, current_block);
    }
    auto block_sums = _mm256_extract_epi64(sum_block, 0) + _mm256_extract_epi64(sum_block, 1) + _mm256_extract_epi64(sum_block, 2) +
                      _mm256_extract_epi64(sum_block, 3);
    for (size_t id = length - length % 4; id < length; id++) {
        block_sums += data[id];
    }
    return block_sums;
}

int64_t HighLevelSumArraySimd(long long *data, size_t length) {
    return SimdVector<long long>(data, length).AggregateSum();
}

int64_t SumArrayNaive(long long *data, size_t length) {
    int64_t sum = 0;
    for (size_t id = 0; id < length; id++) {
        sum += data[id];
    }
    return sum;
}

int64_t MemoryAccessBaseline(long long *data, size_t length) {
    for (size_t id = 0; id < length; id += 4) {
        volatile int64_t ignored = data[id];
    }
    return 0;
}

#endif  // CPP_SUM_IN_ARRAY_HPP
