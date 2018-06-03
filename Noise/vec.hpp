#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <type_traits>

namespace orin {

using namespace glm;

template <size_t D, typename T>
using vec = glm::vec<D, T>;

// not mathematically correct, but close enough
template <typename T, typename Int = int>
inline auto fast_floor(const T& val) {
    return val >= 0 ? (Int)val : (Int)val - 1;
}

template <size_t D, typename T, typename Int = int>
inline auto fast_floor(const vec<D, T>& v) {
    vec<D, Int> result;
    for (auto i = 0; i < D; ++i) {
        result[i] = fast_floor<T, Int>(v[i]);
    }
    return result;
}

} // orin

