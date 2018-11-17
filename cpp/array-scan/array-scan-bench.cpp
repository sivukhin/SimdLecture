#include <benchmark/benchmark.h>
#include <memory>
#include <iostream>
#include "array-scan.hpp"
#include "array-allocator.hpp"
#include "random.hpp"

using namespace benchmark;

template <typename T>
static T *InitializeWithRandomNumbers(T *array, size_t length) {
    auto random = Random(0);
    for (size_t id = 0; id < length; id++) {
        array[id] = static_cast<T>(random.NextLong(0, 1LL << 30));
    }
    return array;
}

static void ScanSumSimdLongLong_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanSumSimd<long long>(array, size));
    }
}

static void ScanSumNaiveLongLong_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanSumNaive<long long>(array, size));
    }
}

static void ScanSumSimdInt_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanSumSimd<int>(array, size));
    }
}

static void ScanSumNaiveInt_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanSumNaive<int>(array, size));
    }
}

static void ScanMaxSimdLongLong_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanMaxSimd<long long>(array, size));
    }
}

static void ScanMaxNaiveLongLong_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanMaxNaive<long long>(array, size));
    }
}

static void ScanMaxSimdInt_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanMaxSimd<int>(array, size));
    }
}

static void ScanMaxNaiveInt_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(ScanMaxNaive<int>(array, size));
    }
}

static void CustomizeBenchmark(benchmark::internal::Benchmark *benchmark) {
    for (int size = 2; size <= (1 << 24); size *= 2) {
        benchmark->Arg(size);
    }
}

BENCHMARK(ScanSumSimdLongLong_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanSumNaiveLongLong_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanSumSimdInt_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanSumNaiveInt_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK(ScanMaxSimdLongLong_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanMaxNaiveLongLong_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanMaxSimdInt_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(ScanMaxNaiveInt_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
