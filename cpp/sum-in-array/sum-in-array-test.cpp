#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "sum-in-array.hpp"
#include "array-allocator.hpp"
#include <vector>

using namespace ::testing;

struct SimdArrayProcessor {
    int64_t operator()(const int64_t *data, size_t length) const {
        return SumArraySimd(data, length);
    }
};

struct NaiveArrayProcessor {
    int64_t operator()(const int64_t *data, size_t length) const {
        return SumArrayNaive(data, length);
    }
};

template <typename T>
class SumInArrayTest : public Test {};
using ArrayProcessorTypes = Types<SimdArrayProcessor, NaiveArrayProcessor>;
TYPED_TEST_CASE(SumInArrayTest, ArrayProcessorTypes);

TYPED_TEST(SumInArrayTest, EmptyArray) {
    auto data = std::vector<int64_t>(0);
    EXPECT_EQ(TypeParam()(data.data(), 0), 0);
}

TYPED_TEST(SumInArrayTest, FourEqualElements) {
    const int64_t value = -1231571212371123LL;
    ArrayAllocator<int64_t> allocator(4);
    auto data = allocator.GetAligned();
    for (size_t id = 0; id < 4; id++) {
        data[id] = value;
    }
    EXPECT_EQ(TypeParam()(data, 4), 4 * value);
}
