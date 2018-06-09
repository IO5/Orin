\text{given n-dimensional point } p := [p_0, p_1, ..., p_n]
\\ \text{and knowing:}\newline
p \in \text{some cube in integer lattice}
\\
vert_i \text{ - i-th verticle of said cube, }
\\
grad(v) \text{ - (not necessarily) unit vector for every lattice point v}
\\
int(x) := P(fract(x)) \text{ - interpolant, P - some polynomial}
\\\\
perlin(p) := L^{n-1}_0(p)
\\\\
\left\{\begin{array}{ll}
L^d_i(p) := lerp(L^{d-1}_{2i}(p),\, L^{d-1}_{2i+1}(p),\, int(p_d))
\\ 
L^0_i(p) := lerp(dot_{2i}(p),\, dot_{2i+1}(p),\, int(p_0))
\end{array}\right.
\\\\
dot_i(p) := grad(vert_i) \cdot (p-vert_i)
\\\\
\text{so in 3 dimensional case:}
\\\\
L^0_0(p) := lerp(dot_0_0_0(p),\, dot_1_0_0(p),\, int(p_0))
\\
L^0_1(p) := lerp(dot_0_1_0(p),\, dot_1_1_0(p),\, int(p_0))
\\
L^0_2(p) := lerp(dot_0_0_1(p),\, dot_1_0_1(p),\, int(p_0))
\\
L^0_3(p) := lerp(dot_1_1_0(p),\, dot_1_1_1(p),\, int(p_0))
\\\\
L^1_0(p) := lerp(L^0_0(p),\, L^0_1(p),\, int(p_1))
\\
L^1_1(p) := lerp(L^0_2(p),\, L^0_3(p),\, int(p_1))
\\\\
L^2_0(p) := lerp(L^1_0(p),\, L^1_1(p),\, int(p_2))
\\\\
\text{now for the derivative:}
\\\\
lerp(f(x),\,g(x),\,w(x)) := f(x) + w(x)(g(x) - f(x))
\\\text{so:}\\
lerp'(f(x),\,g(x),\,w(x)) = lerp(f'(x),\,g'(x),\,w(x)) + w'(x)(g(x) - f(x))
\\\text{and derivative for the dot product is just:}\\
\nabla dot_i(p) = grad(vert_i)
\\\\
\nabla L^d_i(p) = lerp(\nabla L^{d-1}_{2i}(p),\, \nabla L^{d-1}_{2i+1}(p),\, int(p_d)) + \nabla int(p_d) (L^{d-1}_{2i+1}(p) - L^{d-1}_{2i}(p))
\\\text{note: all coordinates of } \nabla int(p_d) \text{ are 0 except d-th}\\

