#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "sum-in-array.hpp"
#include "array-allocator.hpp"
#include <vector>

using namespace ::testing;

struct HighLevelSimdArrayProcessor {
    int64_t operator()(long long *data, size_t length) const {
        return HighLevelSumArraySimd(data, length);
    }
};

struct SimdArrayProcessor {
    int64_t operator()(long long *data, size_t length) const {
        return SumArraySimd(data, length);
    }
};

struct NaiveArrayProcessor {
    int64_t operator()(long long *data, size_t length) const {
        return SumArrayNaive(data, length);
    }
};

template <typename T>
class SumInArrayTest : public Test {};
using ArrayProcessorTypes = Types<SimdArrayProcessor, NaiveArrayProcessor, HighLevelSimdArrayProcessor>;
TYPED_TEST_CASE(SumInArrayTest, ArrayProcessorTypes);

TYPED_TEST(SumInArrayTest, EmptyArray) {
    auto data = std::vector<long long>(0);
    EXPECT_EQ(TypeParam()(data.data(), 0), 0);
}

TYPED_TEST(SumInArrayTest, FourEqualElements) {
    const long long value = -1231571212371123LL;
    ArrayAllocator<long long> allocator(4);
    auto data = allocator.GetAligned();
    for (size_t id = 0; id < 4; id++) {
        data[id] = value;
    }
    EXPECT_EQ(TypeParam()(data, 4), 4 * value);
}
