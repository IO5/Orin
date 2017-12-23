#pragma once

#include "vec.hpp"

namespace orin {

namespace interpolation {
    struct linear {
        template <typename T>
        T operator()(const T& v) { return v; }
    };
    struct cubic {
        template <typename T>
        T operator()(const T& v) { return (3 - 2 * v) * v * v; }
    };
    struct quintic {
        template <typename T>
        T operator()(const T& v) { return ((6 * v - 15) * v + 10) * v * v * v; }
    };
    struct wide {
        template <typename T>
        T operator()(const T& v) {
            return detail::glm_for_each(v, [](const T& v) {
                v = 1 - v * v / 4;
                auto v_sqr = v * v;
                if (v > 0) {
                    return (4 * v - 3) * v_sqr * v_sqr;
                } else {
                    return 0;
                }
            });
        }
    };
} // interpolation


namespace detail {

template <size_t D, typename T, typename interpolation_method>
class perlin_base_impl {
public:
    using vec = orin::vec<D, T>;
    using ivec = orin::vec<D, int>;

    static T get(const vec& p) {
        // grid points
        auto pi0 = fast_floor<int>(p);
        auto pi1 = pi0 + 1;

        auto pf = fract(p);

        auto influence_vecs = get_influence_vecs(pi0, pi1, p);

        return average(influence_vecs, interpolate(pf));
    }

private:
    vec interpolate(const vec& v) {
        return std::invoke(interpolation_method(), v);
    }

    ivec nth_variation(size_t n, const ivec& a, const ivec& b) {
        ivec result;
        for (auto i = 0; i < D; ++i) {
            result[i] = (n & (1 << i)) ? b[i] : a[i];
        }
        return result;
    }

    T influence(const vec& p, const ivec& grid_p) {
        vec dist = p - grid_p;
        vec grad = 0; // gradient[grid_p];
        return dot(dist, grad);
    }

    auto get_influence_vecs(const ivec& a, const ivec& b, const vec& p) {
        constexpr size_t N = (1 << D);
        std::array<T, N> result;
        for (size_t n = 0; n < N; ++n) {
            auto grid_p = nth_variation(n, a, b);
            result[n] = influence(p, grid_p);
        }
        return result;
    }

    template <size_t N>
    T average(const std::array<T, N>& interm, const vec& weights, size_t idx = 0) {
        std::array<T, N / 2> result;

        for (auto i = 0; i < (N / 2); ++i) {
            result[i] = lerp(interm[2 * i], interm[2 * i + 1], weights[idx]);
        }

        if constexpr (N == 2) {
            return result[0];
        } else {
            return average(result, weights, idx + 1);
        }
    }
};

} // detail

// TODO
template <size_t val> struct seed {};
struct dynamic_seed {};

template <size_t D, typename seed, typename T = double, typename interpolation_method = interpolation::quintic>
class perlin_base : private seed {};

} // orin
