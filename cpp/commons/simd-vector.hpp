#ifndef PROJECT_SIMD_VECTOR_HPP
#define PROJECT_SIMD_VECTOR_HPP

#include <memory>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <immintrin.h>

enum class OperationType { Plus, Minus, Xor, And, Or, Equal, Greater };

template <typename T, OperationType Operation>
T Identity = 0;

template <typename T>
T Identity<T, OperationType::And> = -1;

template <typename T>
T Identity<T, OperationType::Equal> = -1;

template <typename T>
T Identity<T, OperationType::Greater> = -1;

template <typename T, const int id>
T ExtractScalar(const __m256i &value);

template <typename T, int id>
class SimdTypedExtractor {
public:
    static T ScalarFrom(const __m256i &value);
};

template <int id>
class SimdTypedExtractor<long long, id> {
public:
    static long long ScalarFrom(const __m256i &value) {
        return _mm256_extract_epi64(value, id);
    }
};

template <int id>
class SimdTypedExtractor<int, id> {
public:
    static int ScalarFrom(const __m256i &value) {
        return _mm256_extract_epi32(value, id);
    }
};

template <int id>
class SimdTypedExtractor<char, id> {
public:
    static char ScalarFrom(const __m256i &value) {
        return static_cast<char>(_mm256_extract_epi8(value, id));
    }
};

template <typename T>
std::vector<T> ExtractAllScalars(const __m256i &value);

template <>
std::vector<long long> ExtractAllScalars<long long>(const __m256i &value) {
    return {SimdTypedExtractor<long long, 0>::ScalarFrom(value), SimdTypedExtractor<long long, 1>::ScalarFrom(value),
            SimdTypedExtractor<long long, 2>::ScalarFrom(value), SimdTypedExtractor<long long, 3>::ScalarFrom(value)};
}
template <>
std::vector<int> ExtractAllScalars<int>(const __m256i &value) {
    return {
        SimdTypedExtractor<int, 0>::ScalarFrom(value), SimdTypedExtractor<int, 1>::ScalarFrom(value),
        SimdTypedExtractor<int, 2>::ScalarFrom(value), SimdTypedExtractor<int, 3>::ScalarFrom(value),
        SimdTypedExtractor<int, 4>::ScalarFrom(value), SimdTypedExtractor<int, 5>::ScalarFrom(value),
        SimdTypedExtractor<int, 6>::ScalarFrom(value), SimdTypedExtractor<int, 7>::ScalarFrom(value),
    };
}
template <>
std::vector<char> ExtractAllScalars<char>(const __m256i &value) {
    return {
        SimdTypedExtractor<char, 0>::ScalarFrom(value),  SimdTypedExtractor<char, 1>::ScalarFrom(value),
        SimdTypedExtractor<char, 2>::ScalarFrom(value),  SimdTypedExtractor<char, 3>::ScalarFrom(value),
        SimdTypedExtractor<char, 4>::ScalarFrom(value),  SimdTypedExtractor<char, 5>::ScalarFrom(value),
        SimdTypedExtractor<char, 6>::ScalarFrom(value),  SimdTypedExtractor<char, 7>::ScalarFrom(value),
        SimdTypedExtractor<char, 8>::ScalarFrom(value),  SimdTypedExtractor<char, 9>::ScalarFrom(value),
        SimdTypedExtractor<char, 10>::ScalarFrom(value), SimdTypedExtractor<char, 11>::ScalarFrom(value),
        SimdTypedExtractor<char, 12>::ScalarFrom(value), SimdTypedExtractor<char, 13>::ScalarFrom(value),
        SimdTypedExtractor<char, 14>::ScalarFrom(value), SimdTypedExtractor<char, 15>::ScalarFrom(value),
        SimdTypedExtractor<char, 16>::ScalarFrom(value), SimdTypedExtractor<char, 17>::ScalarFrom(value),
        SimdTypedExtractor<char, 18>::ScalarFrom(value), SimdTypedExtractor<char, 19>::ScalarFrom(value),
        SimdTypedExtractor<char, 20>::ScalarFrom(value), SimdTypedExtractor<char, 21>::ScalarFrom(value),
        SimdTypedExtractor<char, 22>::ScalarFrom(value), SimdTypedExtractor<char, 23>::ScalarFrom(value),
        SimdTypedExtractor<char, 24>::ScalarFrom(value), SimdTypedExtractor<char, 25>::ScalarFrom(value),
        SimdTypedExtractor<char, 26>::ScalarFrom(value), SimdTypedExtractor<char, 27>::ScalarFrom(value),
        SimdTypedExtractor<char, 28>::ScalarFrom(value), SimdTypedExtractor<char, 29>::ScalarFrom(value),
        SimdTypedExtractor<char, 30>::ScalarFrom(value), SimdTypedExtractor<char, 31>::ScalarFrom(value),

    };
}

template <typename T>
__m256i CreateSingleValueIntegerSimd(const T &value);

template <>
__m256i CreateSingleValueIntegerSimd(const long long &value) {
    return _mm256_set1_epi64x(value);
}
template <>
__m256i CreateSingleValueIntegerSimd(const int &value) {
    return _mm256_set1_epi32(value);
}
template <>
__m256i CreateSingleValueIntegerSimd(const char &value) {
    return _mm256_set1_epi8(value);
}

template <typename T>
static auto LoadIntegerSimd(T *data) {
    return _mm256_load_si256((__m256i *)(data));  // todo (umqra, 11.11.18): Replace LoadU with simple Load in some cases
}

template <typename T>
void StoreIntegerSimd(T *data, const __m256i &value) {
    _mm256_store_si256(reinterpret_cast<__m256i *>(data), value);
}

template <typename T, OperationType Operation>
class SimdTypedExtensions {
public:
    static T ApplyScalarOperation(T &first, T &second);
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second);
};

template <typename T>
class SimdTypedExtensions<T, OperationType::Xor> {
public:
    static T ApplyScalarOperation(T &first, T &second) {
        return first ^ second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_xor_si256(first, second);
    }
};
template <typename T>
class SimdTypedExtensions<T, OperationType::And> {
public:
    static T ApplyScalarOperation(T &first, T &second) {
        return first & second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_and_si256(first, second);
    }
};
template <typename T>
class SimdTypedExtensions<T, OperationType::Or> {
public:
    static T ApplyScalarOperation(T &first, T &second) {
        return first | second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_or_si256(first, second);
    }
};

template <>
class SimdTypedExtensions<long long, OperationType::Plus> {
public:
    static long long ApplyScalarOperation(long long &first, long long &second) {
        return first + second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_add_epi64(first, second);
    }
};
template <>
class SimdTypedExtensions<long long, OperationType::Minus> {
public:
    static long long ApplyScalarOperation(long long &first, long long &second) {
        return first - second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_sub_epi64(first, second);
    }
};
template <>
class SimdTypedExtensions<long long, OperationType::Equal> {
public:
    static long long ApplyScalarOperation(long long &first, long long &second) {
        return first == second ? -1 : 0;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpeq_epi64(first, second);
    }
};
template <>
class SimdTypedExtensions<long long, OperationType::Greater> {
public:
    static long long ApplyScalarOperation(long long &first, long long &second) {
        return first > second ? -1 : 0;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpgt_epi64(first, second);
    }
};
template <>
class SimdTypedExtensions<int, OperationType::Plus> {
public:
    static int ApplyScalarOperation(int &first, int &second) {
        return first + second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_add_epi32(first, second);
    }
};
template <>
class SimdTypedExtensions<int, OperationType::Minus> {
public:
    static int ApplyScalarOperation(int &first, int &second) {
        return first - second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_sub_epi32(first, second);
    }
};
template <>
class SimdTypedExtensions<int, OperationType::Equal> {
public:
    static int ApplyScalarOperation(int &first, int &second) {
        return first == second ? -1 : 0;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpeq_epi32(first, second);
    }
};
template <>
class SimdTypedExtensions<int, OperationType::Greater> {
public:
    static int ApplyScalarOperation(int &first, int &second) {
        return first > second ? -1 : 0;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpgt_epi32(first, second);
    }
};
template <>
class SimdTypedExtensions<char, OperationType::Plus> {
public:
    static char ApplyScalarOperation(char &first, char &second) {
        return first + second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_add_epi8(first, second);
    }
};
template <>
class SimdTypedExtensions<char, OperationType::Minus> {
public:
    static char ApplyScalarOperation(char &first, char &second) {
        return first - second;
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_sub_epi8(first, second);
    }
};
template <>
class SimdTypedExtensions<char, OperationType::Equal> {
public:
    static char ApplyScalarOperation(char &first, char &second) {
        return static_cast<char>(first == second ? -1 : 0);
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpeq_epi8(first, second);
    }
};
template <>
class SimdTypedExtensions<char, OperationType::Greater> {
public:
    static char ApplyScalarOperation(char &first, char &second) {
        return static_cast<char>(first > second ? -1 : 0);
    }
    static __m256i ApplyVectorOperation(__m256i &first, __m256i &second) {
        return _mm256_cmpgt_epi8(first, second);
    }
};

template <typename T, OperationType Operation>
void ElementwiseOperation(T first[], T second[], size_t length) {
    constexpr size_t block_size = 32 / sizeof(T);
    for (size_t id = 0; id + block_size < length; id += block_size) {
        auto current_block = LoadIntegerSimd(first + id);
        auto other_block = LoadIntegerSimd(second + id);
        current_block = SimdTypedExtensions<T, Operation>::ApplyVectorOperation(current_block, other_block);
        StoreIntegerSimd(first + id, current_block);
    }
    for (size_t id = length - length % block_size; id < length; id++) {
        first[id] = SimdTypedExtensions<T, Operation>::ApplyScalarOperation(first[id], second[id]);
    }
}

template <typename T, OperationType Operation>
T AggregateOperation(T target[], size_t length) {
    auto blocks_value = CreateSingleValueIntegerSimd(Identity<T, Operation>);
    constexpr size_t block_size = 32 / sizeof(T);
    for (size_t id = 0; id + block_size < length; id += block_size) {
        auto current_value = LoadIntegerSimd(target + id);
        blocks_value = SimdTypedExtensions<T, Operation>::ApplyVectorOperation(blocks_value, current_value);
    }
    T aggregated_value = Identity<T, Operation>;
    auto all_scalars = ExtractAllScalars<T>(blocks_value);
    for (auto &&scalar : all_scalars) {
        aggregated_value = SimdTypedExtensions<T, Operation>::ApplyScalarOperation(aggregated_value, scalar);
    }
    for (size_t id = length - length % block_size; id < length; id++) {
        aggregated_value = SimdTypedExtensions<T, Operation>::ApplyScalarOperation(aggregated_value, target[id]);
    }
    return aggregated_value;
}

template <typename T>
class SimdMaskedVector;

template <typename T>
class SimdVector {
private:
    std::unique_ptr<T[]> data_;
    size_t length_;

public:
    template <typename P>
    friend class SimdMaskedVector;
    SimdVector(const std::vector<T> &other) : data_(), length_() {
        length_ = other.size();
        auto data_ptr = aligned_alloc(256 / 8, length_ * sizeof(T));
        data_ = std::unique_ptr<T[]>(reinterpret_cast<T *>(data_ptr));
        memcpy(data_.get(), other.data(), sizeof(T) * length_);
    }
    SimdVector(const SimdVector &other) : data_(nullptr), length_(0) {
        length_ = other.length_;
        auto data_ptr = aligned_alloc(256 / 8, length_ * sizeof(T));
        data_ = std::unique_ptr<T[]>(reinterpret_cast<T *>(data_ptr));
        memcpy(data_.get(), other.data_.get(), sizeof(T) * length_);
    }
    SimdVector &operator+=(const SimdVector<T> &other) {
        ElementwiseOperation<T, OperationType::Plus>(data_.get(), other.data_.get(), length_);
        return *this;
    }
    SimdVector &operator-=(const SimdVector<T> &other) {
        ElementwiseOperation<T, OperationType::Minus>(data_.get(), other.data_.get(), length_);
        return *this;
    }
    SimdVector &operator^=(const SimdVector<T> &other) {
        ElementwiseOperation<T, OperationType::Xor>(data_.get(), other.data_.get(), length_);
        return *this;
    }
    SimdVector &operator&=(const SimdVector<T> &other) {
        ElementwiseOperation<T, OperationType::And>(data_.get(), other.data_.get(), length_);
        return *this;
    }
    SimdVector &operator|=(const SimdVector<T> &other) {
        ElementwiseOperation<T, OperationType::Or>(data_.get(), other.data_.get(), length_);
        return *this;
    }
    SimdVector operator+(const SimdVector<T> &other) const {
        auto new_vector = SimdVector(other);
        return (new_vector += other);
    }
    SimdVector operator-(const SimdVector<T> &other) const {
        auto new_vector = SimdVector(other);
        return (new_vector -= other);
    }
    SimdVector operator^(const SimdVector<T> &other) const {
        auto new_vector = SimdVector(other);
        return (new_vector ^= other);
    }
    SimdVector operator&(const SimdVector<T> &other) const {
        auto new_vector = SimdVector(other);
        return (new_vector &= other);
    }
    SimdVector operator|(const SimdVector<T> &other) const {
        auto new_vector = SimdVector(other);
        return (new_vector |= other);
    }
    T AggregateSum() const {
        return AggregateOperation<T, OperationType::Plus>(data_.get(), length_);
    }
    T AggregateXor() const {
        return AggregateOperation<T, OperationType::Xor>(data_.get(), length_);
    }
    T AggregateAnd() const {
        return AggregateOperation<T, OperationType::And>(data_.get(), length_);
    }
    T AggregateOr() const {
        return AggregateOperation<T, OperationType::Or>(data_.get(), length_);
    }
    const T &operator[](size_t index) const {
        return data_[index];
    }
    SimdMaskedVector<T> operator==(const SimdVector<T> &other) const;
    SimdMaskedVector<T> operator<(const SimdVector<T> &other) const;
    SimdMaskedVector<T> operator!=(const SimdVector<T> &other) const;
    SimdMaskedVector<T> operator>(const SimdVector<T> &other) const;
    SimdMaskedVector<T> operator<=(const SimdVector<T> &other) const;
    SimdMaskedVector<T> operator>=(const SimdVector<T> &other) const;
};

template <typename T>
class SimdMaskedVector {
private:
    bool inverted_;
    std::unique_ptr<T[]> data_;
    size_t length_;
    SimdMaskedVector(SimdVector<T> &&other) : inverted_(false), data_(std::move(other.data_)), length_(other.length_) {
    }
    SimdMaskedVector &Invert() {
        inverted_ = true;
        return *this;
    }

public:
    SimdMaskedVector(SimdMaskedVector<T> &&other) : inverted_(other.inverted_), data_(std::move(other.data_)), length_(other.length_) {
    }
    SimdMaskedVector(const SimdMaskedVector<T> &other) : inverted_(false), data_(), length_() {
        length_ = other.length_;
        inverted_ = other.inverted_;

        auto data_ptr = aligned_alloc(256 / 8, length_ * sizeof(T));
        data_ = std::unique_ptr<T[]>(reinterpret_cast<T *>(data_ptr));
        memcpy(data_.get(), other.data_.get(), sizeof(T) * length_);
    }
    template <typename P>
    friend class SimdVector;
    bool Any() const {
        if (!inverted_) {
            return AggregateOperation<T, OperationType::Or>(data_.get(), length_) != 0;
        }
        return AggregateOperation<T, OperationType::And>(data_.get(), length_) == 0;
    }
    bool All() const {
        if (!inverted_) {
            return AggregateOperation<T, OperationType::And>(data_.get(), length_) != 0;
        }
        return AggregateOperation<T, OperationType::Or>(data_.get(), length_) == 0;
    }
    bool None() const {
        return !All();
    }
};

template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator==(const SimdVector<T> &other) const {
    auto mask = SimdVector<T>(*this);
    ElementwiseOperation<T, OperationType::Equal>(mask.data_.get(), other.data_.get(), mask.length_);
    return SimdMaskedVector<T>(std::move(mask));
}

template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator>(const SimdVector<T> &other) const {
    auto mask = SimdVector<T>(*this);
    ElementwiseOperation<T, OperationType::Greater>(mask.data_.get(), other.data_.get(), mask.length_);
    return SimdMaskedVector<T>(std::move(mask));
}

template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator<(const SimdVector<T> &other) const {
    return (other > *this);
}

template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator!=(const SimdVector<T> &other) const {
    return (*this == other).Invert();
}
template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator<=(const SimdVector<T> &other) const {
    return (*this > other).Invert();
}
template <typename T>
SimdMaskedVector<T> SimdVector<T>::operator>=(const SimdVector<T> &other) const {
    return (*this < other).Invert();
}

#endif  // PROJECT_SIMD_VECTOR_HPP
