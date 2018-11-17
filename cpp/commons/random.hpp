#ifndef PROJECT_RANDOM_HPP
#define PROJECT_RANDOM_HPP

#include <random>

class Random {
private:
    std::mt19937_64 _generator;

public:
    explicit Random(int seed) : _generator(seed) {
    }
    template <typename T>
    T Next(T from_inclusive, T to_exclusive) {
        return std::uniform_int_distribution<T>(from_inclusive, to_exclusive - 1)(_generator);
    }
    int32_t NextInt(int32_t from_inclusive, int32_t to_exclusive) {
        return Next<int32_t>(from_inclusive, to_exclusive);
    }
    int64_t NextLong(int64_t from_inclusive, int64_t to_exclusive) {
        return Next<int64_t>(from_inclusive, to_exclusive);
    }
    template <typename T>
    T Sample(std::vector<T> sequence) {
        return sequence[NextInt(0, static_cast<int32_t>(sequence.size()))];
    }
    std::mt19937_64 Generator() const {
        return _generator;
    }
};

#endif  // PROJECT_RANDOM_HPP
