#include <benchmark/benchmark.h>
#include <memory>
#include <iostream>
#include "words-count.hpp"
#include "array-allocator.hpp"
#include "random.hpp"

using namespace benchmark;

static char *InitializeRandomString(char *array, size_t length) {
    auto random = Random(0);
    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !?,.";
    for (size_t id = 0; id < length; id++) {
        array[id] = alphabet[random.NextInt(0, alphabet.length())];
    }
    return array;
}

static void NaiveWordsCount_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<char> allocator(size);
    auto array = InitializeRandomString(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(CountWordsNaive(array, size));
    }
}

static void SimdWordsCount_Benchmark(State &state) {
    size_t size = static_cast<size_t>(state.range(0));
    ArrayAllocator<char> allocator(size);
    auto array = InitializeRandomString(allocator.GetAligned(), size);
    for (auto _ : state) {
        DoNotOptimize(CountWordsSimd(array, size));
    }
}

static void CustomizeBenchmark(benchmark::internal::Benchmark *benchmark) {
    for (int size = 2; size <= (1 << 24); size *= 2) {
        benchmark->Arg(size);
    }
}

BENCHMARK(NaiveWordsCount_Benchmark)->Apply(CustomizeBenchmark);
BENCHMARK(SimdWordsCount_Benchmark)->Apply(CustomizeBenchmark);

BENCHMARK_MAIN();
