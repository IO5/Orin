#pragma once

#include "vec.hpp"

#include <limits>
#include <array>
#include <algorithm>
#include <numeric>

namespace orin {


template <size_t D, typename T>
class simplex_base {
public:
    static_assert(D > 0);
    static_assert(std::numeric_limits<T>::is_iec559);

    using vec = orin::vec<D, T>;
    using ivec = orin::vec<D, int>;
    using bvec = orin::vec<D, bool>;

    constexpr T value(const vec& p) {

        constexpr F = (constexpr_sqrt(D + 1.0) - 1.0) / D;
        // [F+1,   F,   F]
        // [F,   F+1,   F] * p   equivalent
        // [F,     F, F+1]
        vec p_skewed = p + vec(F * std::accumulate(&p[0], &p[D - 1], 0));
        return 0;
    }


};

} // orin