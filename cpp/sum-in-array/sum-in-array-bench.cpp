#include <benchmark/benchmark.h>
#include <memory>
#include <iostream>
#include "sum-in-array.hpp"
#include "array-allocator.hpp"
#include "random.hpp"

using namespace benchmark;

static long long *InitializeWithRandomNumbers(long long *array, size_t length) {
    auto random = Random(0);
    for (size_t id = 0; id < length; id++) {
        array[id] = static_cast<long long>(random.NextLong(0, 1LL << 60));
    }
    return array;
}

static void LoadAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimd(array, size));
    }
}

static void SimdVectorAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(HighLevelSumArraySimd(array, size));
    }
}

static void LoaduAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimdUnaligned(array, size));
    }
}

static void NaiveAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArrayNaive(array, size));
    }
}
static void LoaduUnaligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetUnaligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimdUnaligned(array, size));
    }
}
static void NaiveUnaligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetUnaligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArrayNaive(array, size));
    }
}

static void MemoryAccessAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(MemoryAccessBaseline(array, size));
    }
}

static void MemoryAccessUnaligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<long long> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetUnaligned(), size);
    for (auto _ : state) {
        DoNotOptimize(MemoryAccessBaseline(array, size));
    }
}

static void CustomizeBenchmark(benchmark::internal::Benchmark *benchmark) {
    for (int size = 100; size <= 20000000; size *= 2) {
        benchmark->Arg(size);
    }
}

BENCHMARK(SimdVectorAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(LoaduUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(NaiveUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(LoadAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(LoaduAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(NaiveAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(MemoryAccessAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(MemoryAccessUnaligned_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
