#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "simd-vector.hpp"
#include <vector>

using namespace ::testing;

template <typename T>
class SimdVectorTest : public Test {};
using BasicTypes = Types<char, int, long long>;

template <typename T>
auto GenerateVector(T start_value, size_t count) {
    auto result = std::vector<T>(count);
    for (size_t id = 0; id < count; id++) {
        result[id] = start_value + id;
    }
    return result;
}

TYPED_TEST_CASE(SimdVectorTest, BasicTypes);

TYPED_TEST(SimdVectorTest, TestSumWithItself) {
    auto data = GenerateVector<TypeParam>(0, 101);
    auto simd_vector = SimdVector<TypeParam>(data);
    simd_vector += simd_vector;
    for (TypeParam i = 0; i < data.size(); i++)
        EXPECT_EQ(simd_vector[i], (TypeParam)(2 * i));
}

TYPED_TEST(SimdVectorTest, TestSubtractFromItself) {
    auto data = GenerateVector<TypeParam>(0, 101);
    auto simd_vector = SimdVector<TypeParam>(data);
    simd_vector -= simd_vector;
    for (size_t i = 0; i < data.size(); i++)
        EXPECT_EQ(simd_vector[i], 0);
}

TYPED_TEST(SimdVectorTest, TestSumDifferentVectors) {
    auto data1 = GenerateVector<TypeParam>(0, 101);
    auto data2 = GenerateVector<TypeParam>(-100, 101);
    auto simd_vector1 = SimdVector<TypeParam>(data1);
    auto simd_vector2 = SimdVector<TypeParam>(data2);
    simd_vector1 += simd_vector2;
    for (size_t i = 0; i < data1.size(); i++)
        EXPECT_EQ(simd_vector1[i], data1[i] + data2[i]);
}

TYPED_TEST(SimdVectorTest, TestSubtractDifferentVectors) {
    auto data1 = GenerateVector<TypeParam>(0, 101);
    auto data2 = GenerateVector<TypeParam>(-100, 101);
    auto simd_vector1 = SimdVector<TypeParam>(data1);
    auto simd_vector2 = SimdVector<TypeParam>(data2);
    simd_vector1 -= simd_vector2;
    for (size_t i = 0; i < data1.size(); i++)
        EXPECT_EQ(simd_vector1[i], data1[i] - data2[i]);
}

TYPED_TEST(SimdVectorTest, TestMax) {
    auto data1 = GenerateVector<TypeParam>(0, 101);
    auto data2 = GenerateVector<TypeParam>(0, 101);
    std::reverse(data2.begin(), data2.end());
    auto simd_vector1 = SimdVector<TypeParam>(data1);
    auto simd_vector2 = SimdVector<TypeParam>(data2);
    simd_vector1.MaxWith(simd_vector2);
    for (size_t id = 0; id < data1.size(); id++) {
        EXPECT_EQ(simd_vector1[id], std::max(data1[id], data2[id]));
    }
}

TYPED_TEST(SimdVectorTest, TestMin) {
    auto data1 = GenerateVector<TypeParam>(0, 101);
    auto data2 = GenerateVector<TypeParam>(0, 101);
    std::reverse(data2.begin(), data2.end());
    auto simd_vector1 = SimdVector<TypeParam>(data1);
    auto simd_vector2 = SimdVector<TypeParam>(data2);
    simd_vector1.MinWith(simd_vector2);
    for (size_t id = 0; id < data1.size(); id++) {
        EXPECT_EQ(simd_vector1[id], std::min(data1[id], data2[id]));
    }
}

TYPED_TEST(SimdVectorTest, TestAggregateSum) {
    auto data = std::vector<TypeParam>(101);
    for (size_t id = 0; id < data.size(); id++)
        data[id] = (id % 2 == 0 ? 1 : -1);
    auto simd_vector = SimdVector<TypeParam>(data);
    TypeParam expected_sum = 0;
    for (auto&& value : data)
        expected_sum = (TypeParam)(expected_sum + value);
    EXPECT_EQ(simd_vector.AggregateSum(), expected_sum);
}

TYPED_TEST(SimdVectorTest, TestAggregateXor) {
    auto data = GenerateVector<TypeParam>(0, 101);
    auto simd_vector = SimdVector<TypeParam>(data);
    auto expected_xor = 0;
    for (auto&& value : data)
        expected_xor ^= value;
    EXPECT_EQ(simd_vector.AggregateXor(), expected_xor);
}

TYPED_TEST(SimdVectorTest, TestAggregateAnd) {
    auto data = GenerateVector<TypeParam>(0, 101);
    auto simd_vector = SimdVector<TypeParam>(data);
    auto expected_and = -1;
    for (auto&& value : data)
        expected_and &= value;
    EXPECT_EQ(simd_vector.AggregateAnd(), expected_and);
}

TYPED_TEST(SimdVectorTest, TestAggregateOr) {
    auto data = GenerateVector<TypeParam>(-50, 101);
    auto simd_vector = SimdVector<TypeParam>(data);
    TypeParam expected_or = 0;
    for (auto&& value : data)
        expected_or = (TypeParam)(expected_or | value);
    EXPECT_EQ(simd_vector.AggregateOr(), expected_or);
}

TYPED_TEST(SimdVectorTest, TestEmptyAggregateAnd) {
    auto simd_vector = SimdVector<TypeParam>(std::vector<TypeParam>{});
    EXPECT_EQ(simd_vector.AggregateAnd(), -1);
}

TYPED_TEST(SimdVectorTest, TestExactlyOneSimdBlockAggregate) {
    auto simd_vector = SimdVector<TypeParam>(std::vector<TypeParam>{0, 1, 2, 3});
    EXPECT_EQ(simd_vector.AggregateSum(), 6);
}

TYPED_TEST(SimdVectorTest, TestExactlyOneSimdBlockElementwise) {
    auto simd_vector = SimdVector<TypeParam>(std::vector<TypeParam>{0, 1, 2, 3});
    simd_vector += simd_vector;
    for (size_t id = 0; id < 4; id++) {
        EXPECT_EQ(simd_vector[id], 2 * id);
    }
}

TYPED_TEST(SimdVectorTest, TestEqualVectors) {
    auto simd_vector = SimdVector<TypeParam>(GenerateVector<TypeParam>(0, 101));
    EXPECT_TRUE((simd_vector == simd_vector).All());
    EXPECT_TRUE((simd_vector == simd_vector).Any());

    EXPECT_FALSE((simd_vector != simd_vector).All());
    EXPECT_FALSE((simd_vector != simd_vector).Any());

    EXPECT_FALSE((simd_vector < simd_vector).All());
    EXPECT_FALSE((simd_vector < simd_vector).Any());

    EXPECT_FALSE((simd_vector > simd_vector).All());
    EXPECT_FALSE((simd_vector > simd_vector).Any());

    EXPECT_TRUE((simd_vector <= simd_vector).All());
    EXPECT_TRUE((simd_vector <= simd_vector).Any());

    EXPECT_TRUE((simd_vector >= simd_vector).All());
    EXPECT_TRUE((simd_vector >= simd_vector).Any());
}

TYPED_TEST(SimdVectorTest, TestLesserVectors) {
    auto simd_vector1 = SimdVector<TypeParam>(GenerateVector<TypeParam>(0, 101));
    auto simd_vector2 = SimdVector<TypeParam>(GenerateVector<TypeParam>(1, 101));
    EXPECT_FALSE((simd_vector1 == simd_vector2).All());
    EXPECT_FALSE((simd_vector1 == simd_vector2).Any());

    EXPECT_TRUE((simd_vector1 != simd_vector2).All());
    EXPECT_TRUE((simd_vector1 != simd_vector2).Any());

    EXPECT_TRUE((simd_vector1 < simd_vector2).All());
    EXPECT_TRUE((simd_vector1 < simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 > simd_vector2).All());
    EXPECT_FALSE((simd_vector1 > simd_vector2).Any());

    EXPECT_TRUE((simd_vector1 <= simd_vector2).All());
    EXPECT_TRUE((simd_vector1 <= simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 >= simd_vector2).All());
    EXPECT_FALSE((simd_vector1 >= simd_vector2).Any());
}

TYPED_TEST(SimdVectorTest, TestDifferentVectorRelations) {
    auto data1 = GenerateVector<TypeParam>(-50, 101);
    auto data2 = GenerateVector<TypeParam>(-50, 101);
    std::reverse(data2.begin(), data2.end());
    auto simd_vector1 = SimdVector<TypeParam>(data1);
    auto simd_vector2 = SimdVector<TypeParam>(data2);
    EXPECT_FALSE((simd_vector1 == simd_vector2).All());
    EXPECT_TRUE((simd_vector1 == simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 != simd_vector2).All());
    EXPECT_TRUE((simd_vector1 != simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 < simd_vector2).All());
    EXPECT_TRUE((simd_vector1 < simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 > simd_vector2).All());
    EXPECT_TRUE((simd_vector1 > simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 <= simd_vector2).All());
    EXPECT_TRUE((simd_vector1 <= simd_vector2).Any());

    EXPECT_FALSE((simd_vector1 >= simd_vector2).All());
    EXPECT_TRUE((simd_vector1 >= simd_vector2).Any());
}

TEST(SimdVectorTest, TestBigLongLongMax) {
    auto data1 = std::vector<long long>{(1LL << 45LL) - 1, -(1LL << 50LL) + 1, (1LL << 55LL) - 1, -(1LL << 60LL) + 1};
    auto data2 = std::vector<long long>{-(1LL << 45LL) + 1, (1LL << 50LL) - 1, -(1LL << 55LL) + 1, (1LL << 60LL) - 1};
    auto simd_vector = SimdVector<long long>(data1);
    simd_vector.MaxWith(SimdVector<long long>(data2));
    for (size_t id = 0; id < data1.size(); id++) {
        EXPECT_EQ(simd_vector[id], std::max(data1[id], data2[id]));
    }
}

TEST(SimdVectorTest, TestBigLongLongMin) {
    auto data1 = std::vector<long long>{(1LL << 45LL) - 1, -(1LL << 50LL) + 1, (1LL << 55LL) - 1, -(1LL << 60LL) + 1};
    auto data2 = std::vector<long long>{-(1LL << 45LL) + 1, (1LL << 50LL) - 1, -(1LL << 55LL) + 1, (1LL << 60LL) - 1};
    auto simd_vector = SimdVector<long long>(data1);
    simd_vector.MinWith(SimdVector<long long>(data2));
    for (size_t id = 0; id < data1.size(); id++) {
        EXPECT_EQ(simd_vector[id], std::min(data1[id], data2[id]));
    }
}