#include <benchmark/benchmark.h>
#include <memory>
#include <iostream>
#include "sum-in-array.hpp"
#include "array-allocator.hpp"
#include "random.hpp"

using namespace benchmark;

static int64_t *InitializeWithRandomNumbers(int64_t *array, size_t length) {
    auto random = Random(0);
    for (size_t id = 0; id < length; id++) {
        array[id] = random.NextLong(0, 1LL << 60);
    }
    return array;
}

static void LoadAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int64_t> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimd(array, size));
    }
}

static void LoaduAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int64_t> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimdUnaligned(array, size));
    }
}

static void NaiveAligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int64_t> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArrayNaive(array, size));
    }
}
static void LoaduUnaligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int64_t> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetUnaligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArraySimdUnaligned(array, size));
    }
}
static void NaiveUnaligned_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<int64_t> allocator(size);
    auto array = InitializeWithRandomNumbers(allocator.GetUnaligned(), size);
    for (auto _ : state) {
        DoNotOptimize(SumArrayNaive(array, size));
    }
}

static void CustomizeBenchmark(benchmark::internal::Benchmark *benchmark) {
    for (int size = 100; size <= 1000000; size *= 2) {
        benchmark->Arg(size);
    }
}

BENCHMARK(LoaduUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(NaiveUnaligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(LoadAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(LoaduAligned_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(NaiveAligned_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
