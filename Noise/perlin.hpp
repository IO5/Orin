#pragma once

#include "vec.hpp"

#include <limits>
#include <array>
#include <algorithm>
#include <random>

namespace orin {

namespace interpolation {
    struct linear {
        template <typename T>
        T interpolate(const T& v) { return v; }
        template <typename T>
        T derivative(const T& v) { return T(1); }
    };
    struct cubic {
        template <typename T>
        T interpolate(const T& v) { return (3 - 2 * v) * v * v; }
        template <typename T>
        T derivative(const T& v) { return (6 - 6 * v) * v; }
    };
    using hermite = cubic;
    struct quintic {
        template <typename T>
        T interpolate(const T& v) { return ((6 * v - 15) * v + 10) * v * v * v; }
        template <typename T>
        T derivative(const T& v) { return ((30 * v - 60) * v + 30) * v * v; }
    };
    // http://www.cs.utah.edu/~aek/research/noise.pdf
    // http://eastfarthing.com/blog/2015-04-21-noise/
    struct wide {
        template <typename T>
        T interpolate(const T& v) {
            // I don't think max is needed actually
            //v = max(1 - v * v / 4, 0);
            v = 1 - v * v / 4;
            auto v_quad = v * v;
            return (4 * v - 3) * v_quad * v_quad;
        }
        template <typename T>
        T derivative(const T& v) {
            auto v_sqr = (4 - v * v);
            return (12 - 5 * v_sqr) * v_sqr * v_sqr * v_sqr * v / 128;
        }
    };
} // interpolation

namespace detail {

// arranged array of all numbers from 0-255 inclusive.
inline static constexpr uint8_t classic_lut[] = {
   151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
   140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
   247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
    57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
    74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
    60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
    65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
   200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
    52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
   207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
   119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
   129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
   218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
    81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
   184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
   222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180
};

template <typename T>
class classic_gradients {
public:
    classic_gradients(size_t seed = 0) {
        set_seed(seed);
    }

    constexpr void set_seed(size_t s) {
        seed = s;
        if (seed == 0) {
            lut_ref = classic_lut;
        } else {
            lut_ref = lut;
            std::copy(classic_lut, std::end(classic_lut), lut);
            // TODO replace with something light like xorshift:
            std::mt19937 gen(seed);
            std::shuffle(lut, std::end(lut), gen);
        }
    }
protected:
    //template <size_t D>
    constexpr vec<3, int> gradient(const vec<3, T>& grid_p) const {
        using ivec = vec<3, int>;
        switch (hash<3>(grid_p) & 0xF) { // TODO remove <3>
        case 0x0: return ivec( 1,  1,  0);
        case 0x1: return ivec(-1,  1,  0);
        case 0x2: return ivec( 1, -1,  0);
        case 0x3: return ivec(-1, -1,  0);
        case 0x4: return ivec( 1,  0,  1);
        case 0x5: return ivec(-1,  0,  1);
        case 0x6: return ivec( 1,  0, -1);
        case 0x7: return ivec(-1,  0, -1);
        case 0x8: return ivec( 0,  1,  1);
        case 0x9: return ivec( 0, -1,  1);
        case 0xA: return ivec( 0,  1, -1);
        case 0xB: return ivec( 0, -1, -1);
        case 0xC: return ivec( 1,  1,  0);
        case 0xD: return ivec( 0, -1,  1);
        case 0xE: return ivec(-1,  1,  0);
        case 0xF: return ivec( 0, -1, -1);
        default: return ivec(0);
        }
    }
private:
    template <size_t D>
    constexpr uint8_t hash(const vec<D, int>& grid_p) const {
        //template <int repeat = 0>
        /*auto tile = [n](int v) {
            if (repeat > 0)
                v %= repeat;
            return v;
        };*/
        uint8_t idx = 0;
        for (auto i = 0; i < D; ++i) {
            idx = lut_ref[static_cast<uint8_t>(idx + grid_p[i])]; // cmp speed with nocast/doubled lut version
        }
        return idx;
    }


    size_t seed;
    uint8_t lut[256];
    const uint8_t *lut_ref = nullptr;
};

} // detail

template <size_t D, typename T, typename interpolant, typename gradient_source>
class perlin_base : public interpolant, public gradient_source {
public:
    static_assert(D > 0);
    static_assert(std::numeric_limits<T>::is_iec559);

    using vec = orin::vec<D, T>;
    using ivec = orin::vec<D, int>;

    using gradient_source::gradient_source;

    // noise value 
    constexpr T value(const vec& p) {
        // grid points
        auto pi0 = fast_floor(p);
        auto pi1 = pi0 + 1;

        // calculate influence for each verticle in D-dimensional cube
        constexpr size_t N = (1 << D);
        std::array<T, N> influences;
        for (size_t n = 0; n < N; ++n) {
            auto vert = nth_vert(n, pi0, pi1);
            influences[n] = influence(p, vert);
        }

        return average(influences, interpolant::interpolate(fract(p)));
    }

    template <typename... Args>
    constexpr T value(T p0, Args... pn) {
        static_assert(sizeof...(Args) + 1 == D);
        // TODO check types
        return value(vec(p0, pn...));
    }
    
    // analytical derivative at point p
    constexpr vec derivative(const vec& p) {
        // grid points
        auto pi0 = fast_floor(p);
        auto pi1 = pi0 + 1;

        // same as above + derivatives of influences which just are equal to gradients
        constexpr size_t N = (1 << D);
        std::array<T, N> influences;
        std::array<vec, N> derivatives;
        for (size_t n = 0; n < N; ++n) {
            auto vert = nth_vert(n, pi0, pi1);
            influences[n] = influence(p, vert);
            derivatives[n] = gradient(vert);
        }

        return d_average(
            influences,
            derivatives,
            interpolant::interpolate(fract(p)),
            interpolant::derivative(fract(p))
        );
    }


private:
    constexpr bool ith_bit(uint8_t i, size_t n) {
        return (bool)(n & (1 << i));
    }
    constexpr ivec nth_vert(size_t n, const ivec& a, const ivec& b) {
        ivec result;
        for (auto i = 0; i < D; ++i) {
            result[i] = !ith_bit(i, n) ? a[i] : b[i];
        }
        return result;
    }

    // aka fade function
    constexpr T influence(const vec& p, const ivec& grid_p) {
        vec dist = p - vec(grid_p);
        vec grad = gradient(grid_p);
        return dot(dist, grad);
    }

    // average pairs of influences using lerp, down to a single value
    template <size_t N>
    constexpr T average(const std::array<T, N>& interm, const vec& weights, size_t dim = 0) {
        static_assert(N >= 2);
        static_assert((N & (N - 1)) == 0, "N is not a power of 2");

        std::array<T, N / 2> next_interm;

        for (auto i = 0; i < (N / 2); ++i) {
            next_interm[i] = lerp(interm[2 * i], interm[2 * i + 1], weights[dim]);
        }

        if constexpr (N == 2) {
            return next_interm[0];
        } else {
            return average(next_interm, weights, dim + 1);
        }
    }

    // derivative of the above
    // since (lerp(f(x), g(x), w(x)))' = lerp(f'(x), g'(x), w(x)) + (g(x) - f(x))*w'(x)
    // we do all the steps the same + some additional stuff
    // TODO write down the math in some README
    template <size_t N>
    constexpr vec d_average(const std::array<T, N>& interm, const std::array<vec, N>& deriv, const vec& weights, const vec& d_weights, size_t dim = 0) {
        static_assert(N >= 2);
        static_assert((N & (N - 1)) == 0, "N is not a power of 2");

        std::array<T, N / 2> next_interm;
        std::array<vec, N / 2> next_deriv;

        for (auto i = 0; i < (N / 2); ++i) {
            next_interm[i] = lerp(interm[2 * i], interm[2 * i + 1], weights[dim]);
            next_deriv[i]  = lerp(deriv[2 * i], deriv[2 * i + 1], weights[dim]);
            // derivative of the interpolant is 0 for all but current dimension
            next_deriv[i][dim] += (interm[2 * i + 1] - interm[2 * i]) * d_weights[dim];
        }

        if constexpr (N == 2) {
            return next_deriv[0];
        } else {
            return d_average(next_interm, next_deriv, weights, d_weights, dim + 1);
        }
    }
};

template <class base_noise>
class fractal : public base_noise {
    //TODO
};
/*
template <typename T = double, int seed = 0, typename interpolation_method = interpolation::quintic>
using perlin = fractal<perlin_base<T, seed, interpolation_method>>;
*/
} // orin
