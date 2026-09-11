This program numerically solves the time-independent Schrödinger equation for a particle scattering from the piecewise-constant potential

$$
V(x)=
\begin{cases}
0, & x<-2a,\\
V_0, & -2a<x<-a,\\
2V_0, & -a<x<a,\\
V_0, & a<x<2a,\\
0, & x>2a.
\end{cases}
$$

The calculation uses the dimensionless quantities

$$
\frac{V_0}{E}
\qquad\text{and}\qquad
ka.
$$

In the code, `V0` represents $V_0/E$ and `k` represents $ka$, so no separate value of $E$ is needed.

The `Matrix2` structure stores a $2\times2$ transfer matrix. For a region of constant potential, the `propagation` function returns a matrix that propagates the wavefunction and its derivative across the region,

$$
\begin{pmatrix}
\psi(x+L)\\
\psi'(x+L)
\end{pmatrix}
=
M
\begin{pmatrix}
\psi(x)\\
\psi'(x)
\end{pmatrix}.
$$

The function `multiply` performs $2\times2$ matrix multiplication. The three non-zero potential regions are represented by `M1`, `M2`, and `M3`:

```text
M1: V0,   width = 1
M2: 2V0,  width = 2
M3: V0,   width = 1
```

The total transfer matrix is stored in `M`,

$$
M=M_3M_2M_1.
$$

The `propagation` function uses trigonometric functions when $V<E$ and hyperbolic functions when $V>E$. The special case $V=E$ is handled separately to avoid division by zero.

The reflection and transmission amplitudes, `r` and `t`, are obtained from the boundary conditions for the incoming, reflected, and transmitted waves. The reflection and transmission probabilities are then calculated as

$$
R=|r|^2,
\qquad
T=|t|^2.
$$

These are stored in the variables `R` and `T`. Since the potential is zero on both sides of the barrier, probability conservation provides the check

$$
R+T=1.
$$

The `main` function varies `V0` from $0.1$ to $3.0$ while fixing `k = 2.0$. It prints `V0`, `k`, `R`, `T`, and `R+T` to the terminal. These results can be used to investigate tunneling and interference as $V_0/E$ and $ka$ are varied.
 like so:

```shell
(base) shalinikv@Shalinis-MacBook-Pro EX7 % ./schrodinger                     
 V0/E    ka       R        T        R+T
0.100000 2.000000 0.000476 0.999524 1.000000
0.200000 2.000000 0.002399 0.997601 1.000000
0.300000 2.000000 0.008124 0.991876 1.000000
0.400000 2.000000 0.077054 0.922946 1.000000
0.500000 2.000000 0.562216 0.437784 1.000000
0.600000 2.000000 0.936859 0.063141 1.000000
0.700000 2.000000 0.991943 0.008057 1.000000
0.800000 2.000000 0.998732 0.001268 1.000000
0.900000 2.000000 0.999758 0.000242 1.000000
1.000000 2.000000 0.999946 0.000054 1.000000
```
