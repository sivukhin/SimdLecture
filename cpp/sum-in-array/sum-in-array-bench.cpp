#include <benchmark/benchmark.h>
#include <memory>
#include <iostream>
#include "sum-in-array.hpp"

using namespace benchmark;

template <typename T>
class UnalignedArray {
private:
    T *array_;
    size_t array_size_;

public:
    UnalignedArray(size_t array_size) : array_(nullptr), array_size_(array_size) {
        array_ = new T[array_size + 1];
    }
    UnalignedArray(const UnalignedArray &other) = delete;
    UnalignedArray(UnalignedArray &&other) : array_(other.array_), array_size_(other.array_size_) {
        other.array_ = nullptr;
        other.array_size_ = 0;
    }
    UnalignedArray &operator=(const UnalignedArray &other) = delete;
    UnalignedArray &operator=(UnalignedArray &&other) = delete;
    T *get() const {
        return array_ + 1;
    }
    ~UnalignedArray() {
        delete[] array_;
    }
};

UnalignedArray<int64_t> AllocateUnalignedArray(size_t array_size) {
    return UnalignedArray<int64_t>(array_size);
}

std::unique_ptr<int64_t[]> AllocateAlignedArray(size_t array_size) {
    return std::unique_ptr<int64_t[]>(
        reinterpret_cast<int64_t *>(aligned_alloc(256 / 8, array_size)));
}

static void SumArraySimdAligned_Benchmark(State &state) {
    auto array = AllocateAlignedArray(state.range(0));
    for (auto _ : state) {
        DoNotOptimize(SumArraySimd(array.get(), state.range(0)));
    }
}
static void SumArraySisdAligned_Benchmark(State &state) {
    auto array = AllocateAlignedArray(state.range(0));
    for (auto _ : state) {
        DoNotOptimize(SumArraySisd(array.get(), state.range(0)));
    }
}
static void SumArraySimdUnaligned_Benchmark(State &state) {
    auto array = AllocateUnalignedArray(state.range(0));
    for (auto _ : state) {
        DoNotOptimize(SumArraySimd(array.get(), state.range(0)));
    }
}
static void SumArraySisdUnaligned_Benchmark(State &state) {
    auto array = AllocateUnalignedArray(state.range(0));
    for (auto _ : state) {
        DoNotOptimize(SumArraySisd(array.get(), state.range(0)));
    }
}

static void CustomizeBenchmark(benchmark::internal::Benchmark *benchmark) {
    for (int size = 100; size <= 1000000; size *= 2) {
        benchmark->Arg(size);
    }
}

BENCHMARK(SumArraySimdUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(SumArraySisdUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(SumArraySimdAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(SumArraySisdAligned_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
