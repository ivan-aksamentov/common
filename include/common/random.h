#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <random>

namespace numcpp {
class Random {
    std::mt19937 m_generator;

   public:
    Random() {
        std::array<int, std::mt19937::state_size> seed_data;
        std::random_device r;
        std::generate_n(seed_data.begin(), seed_data.size(), std::ref(r));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        m_generator = std::mt19937(seq);
    }

    explicit Random(int seed) : m_generator(seed) {}

    Random(const Random& other) = delete;

    const Random& operator=(const Random& other) = delete;

    template <typename T>
    inline int uniform(T begin, T end) {
        std::uniform_int_distribution<T> dist(begin, end);
        return dist(m_generator);
    }

    template <typename T>
    inline int uniform(T end) {
        return uniform(T(0), end);
    }

    template <typename T>
    inline float uniformf(T begin, T end) {
        std::uniform_real_distribution<T> dist(begin, end);
        return dist(m_generator);
    }

    template <typename T>
    inline float uniformf(T end) {
        return uniformf(T(0), end);
    }
};
}
