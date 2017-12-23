#pragma once

#include <glm\glm.hpp>
#include <type_traits>

namespace orin {

using glm::vec;

namespace detail {

template <typename T, class Func>
inline auto glm_for_each(T val, Func&& func) {
    return func(val);
}

template <size_t D, typename T, class Func>
inline auto glm_for_each(const vec<D, T>& vec, Func&& func) {
    using result_type = std::decay_t<decltype(func(std::declval<T>()))>;
    glm::vec<D, result_type> result;
    for (auto i = 0; i < D; ++i) {
        result[i] = func(vec[i]);
    }
    return result;
}

} // detail

template <typename Int, typename T>
inline auto fast_floor(const T& vec) {
    return detail::glm_for_each(vec, [](auto val) { return (val >= 0 ? (Int)val : (Int)val - 1); });
}

} // orin

