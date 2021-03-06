﻿#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>

#include <iostream>
#include <string>
#include <functional>
#include <type_traits>
#include <tuple>

#include <glm/gtx/string_cast.hpp>
#include "perlin.hpp"
#include "simplex.hpp"

template <size_t D, typename T>
T get(const orin::vec<D, T>& p) {
    return p[0];
}

int main() {
    orin::perlin_base<3, float, orin::interpolation::linear, orin::detail::classic_gradients<float>> perlin;
    glm::vec3 a(1.75);
    std::cout << glm::to_string(a) << '\n';

    std::cout << get(a) << '\n';
    std::cout << perlin.value(a) << '\n';
    std::cout << glm::to_string(perlin.derivative(a)) << '\n';

    Trie<bool> trie;

    return 0;
}