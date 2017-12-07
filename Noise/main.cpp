#include <glm\glm.hpp>
//#include <glm\gtx\compatibility.hpp>

#include <iostream>
#include <string>
#include <functional>
#include <type_traits>
#include <tuple>

#include "perlin.hpp"
/*
namespace std {
namespace detail {
template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
    return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    // Note: std::invoke is a C++17 feature
}
} // namespace detail

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t) {
    return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
}
}




namespace orin {

template <class Outer, class Inner>
class Composition {
public:
    template <class Outer_, class Inner_>
    Composition(Outer_&& outer, Inner_&& inner) : outer(std::forward<Outer_>(outer)), inner(std::forward<Inner_>(inner)) {}
    
    template <typename... Args>
    auto operator()(Args... args) {
        using R = std::result_of_t<Outer(std::result_of_t<Inner(Args...)>)
        return std::invoke(outer, std::invoke(inner, args...));
    }
private:
    Outer outer;
    Inner inner;
};
template <class Outer, class Inner>
auto compose(Outer&& outer, Inner&& inner) {
    return Composition<Outer, Inner>(std::forward<Outer>(outer), std::forward<Inner>(inner));
}

template <class Outer, class Inner>
Composition<Outer, Inner> operator| (Inner&& inner, Outer&& outer) {
    return Composition<Outer, Inner>(std::forward<Outer>(outer), std::forward<Inner>(inner));
}

template <typename Real>
int fast_floor(Real val) { return (val >= 0 ? (int)val : (int)val - 1); }


//template <typename T, typename U>
//T lerp(T a, T b, U w) {
//    return (U(1) - w) * a + w * b;
//}

template <size_t D, typename T>
class PerlinBaseD {
    using VecT = Vec<D, T>;
    using VecI = Vec<D, int>;
public:
    
    //template <size_t D>
    //T operator() (const VecT<D>& point);
    //template <typename... Args>
    //T operator() (Args... coords) {
    //    constexpr size_t D = sizeof...(Args);
    //    static_assert(D > 0);
    //    return operator()(VecT<D>(T(coords)...));
    //}
    T operator() (const VecT& p);
private:
};


template <size_t D, typename T, size_t Size = 256>
class GradTable {
public:
    const Vec<D, T>& operator[] (const Vec<D, T>& idxs) {
        auto offsets = Vec<D, T>(1);
        for (size_t i = 1; i < D; ++i) {
            offsets[i] = offsets[i - 1] * Size;
        }
        return table[glm::dot(idx, offsets)];
    }

    GradTable() {
        // TODO init
    }

private:
    std::array<Vec<D, T>, Size> table;
};

// dot product of the distance and gradient vectors
template <size_t D, typename T>
T dotGardDist(const Vec<D, size_t> verticle, const Vec<D, T> vec) {
    T distance = vec - verticle;
    return glm::dot(distance, grad[verticle]);
}

template <size_t D>
Vec<D, int> variation(size_t idx, const Vec<D, int>& a, const Vec<D, int>& b) {
    Vec<D, int> result;
    auto nthBit = [](size_t n, size_t val) {
        return 1 & (val >> n);
    };
    for (size_t i = 0; i < N; ++i) {
        // would multiplication be faster?
        if (nthBit(i, idx)) {
            result[i] = b[i];
        } else {
            result[i] = a[i];
        }
    }
    return result;
}

template <size_t D, typename T>
auto dotGradDistForAllVerticles(const Vec<D, int>& a, const Vec<D, int>& b, const Vec<D, T>& p) {
    constexpr size_t N = (1 << D);
    std::array<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        auto verticle = variation(i, a, b);
        result[i] = dotGradDist(verticle, p);
    }
    return result;
}

template <typename T, size_t N, size_t D>
T lerpReduce(const std::array<T, N>& interm, const Vec<D, T>& weights, size_t idx = 0) {
    std::array<T, N / 2> result;
    for (size_t i = 0; i < (N / 2); ++i) {
        result[i] = glm::lerp(interm[2 * i], interm[2 * i + 1], weights[idx]);
    }
    if (N == 2) {
        return result[0];
    } else {
        return lerpReduce(result, weights, ++idx);
    }
}

template <size_t D, typename T>
T perlin(const Vec<D, T>& p)
{
    // integer part
    // change to int or size_t
    auto pi0 = glm::floor(p); // maybe use other floor since defualt is extremly slow
    auto pi1 = pi0 + Vec<D, T>(1);
    // mod here or in garient table access

    // fractional part
    auto pf0 = glm::fract(p);

    auto in = interpolate(pf0);

    for (auto comb : allCombinationsOfCoordsOf(pi0, pi1)) {
        auto n = dotGrad(comb, p);
    }
    return lerpReduce(dotGrads, )
}

}
*/

int main() {
    return 0;
}