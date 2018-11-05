#ifndef PROJECT_RANDOM_HPP
#define PROJECT_RANDOM_HPP

#include <random>

class Random {
private:
    std::mt19937_64 _generator;

public:
    explicit Random(int seed) : _generator(seed) {
    }
    size_t NextInt(size_t from_inclusive, size_t to_exclusive) {
        return std::uniform_int_distribution<size_t>(from_inclusive, to_exclusive - 1)(_generator);
    }
    uint64_t NextLong(uint64_t from_inclusive, uint64_t to_exclusive) {
        return std::uniform_int_distribution<uint64_t>(from_inclusive, to_exclusive - 1)(_generator);
    }
    template <typename T>
    T Sample(std::vector<T> sequence) {
        return sequence[NextInt(0, sequence.size())];
    }
    std::mt19937_64 Generator() const {
        return _generator;
    }
};

#endif  // PROJECT_RANDOM_HPP
