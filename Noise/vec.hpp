#pragma once

#include <glm\glm.hpp>
#include <type_traits>

namespace orin {

using namespace glm;

template <size_t D, typename T>
using vec = glm::vec<D, T>;

namespace detail {

template<typename Vec>
struct vec_dim_impl;

template<size_t L, typename T>
struct vec_dim_impl<orin::vec<L, T>> {
    static constexpr size_t value = L;
};

} // detail

template<typename Vec>
constexpr size_t vec_dim = detail::vec_dim_impl<Vec>::value;

// not mathematically correct, but close enough
template <typename T, typename Int = int>
inline auto fast_floor(const T& vec) {
    glm::vec<vec_dim<T>, Int> result;
    for (auto i = 0; i < vec_dim<T>; ++i) {
        result[i] = vec[i] >= 0 ? (Int)vec[i] : (Int)vec[i] - 1;
    }
    return result;
}

} // orin

