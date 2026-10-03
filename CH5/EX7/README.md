Starting from the EX6, that computes the overlap matrix, I modified the integrand to calculate matrix elements for different operators in the same Hermite basis.

#### `PARTA/operator.cpp` — $\hat{x}$

Changed the integrand from

```cpp
H_i(x) * H_j(x)
```

to

```cpp
H_i(x) * x * H_j(x)
```

This computes

$$
X_{ij} = \langle i|\hat{x}|j\rangle.
$$

#### `PARTB/operator.cpp` — $\hat{x}^2$

Added another factor of `x`:

```cpp
H_i(x) * x * x * H_j(x)
```

This computes

$$
X^2_{ij} = \langle i|\hat{x}^2|j\rangle.
$$

#### `PARTC/operator.cpp` — $\hat{D} = d/dx$

For the derivative, I needed to differentiate the whole Hermite basis function, including the Gaussian:

$$
\psi_j(x) \propto H_j(x)e^{-x^2/2}.
$$

Using

$$
H_j'(x)=2jH_{j-1}(x),
$$

the derivative becomes

$$
\frac{d\psi_j}{dx}
\propto
\left[2jH_{j-1}(x)-xH_j(x)\right]e^{-x^2/2}.
$$

So the integrand was changed to

```cpp
H_i(x) * (dH_j - x * H_j)
```

to compute

$$
D_{ij}=\left\langle i\left|\frac{d}{dx}\right|j\right\rangle.
$$

#### `PARTD/operator.cpp` — $\hat{H}$

For the harmonic oscillator,

$$
\hat{H}
=
-\frac12\frac{d^2}{dx^2}
+\frac12x^2.
$$

I therefore added the second derivative of the Hermite basis function and combined it with the \(x^2\) term:

```cpp
Hpsi = -0.5 * d2psi + 0.5 * x * x * Hj;
```

The integrand then becomes

```cpp
H_i(x) * Hpsi
```

which computes

$$
H_{ij}=\langle i|\hat{H}|j\rangle.
$$

In all cases, the same Gauss-Hermite integrator and normalization from the original code are used. The main change is just the operator inserted between the two basis functions. In this case, the output matrix is

```shell
shalinikv@Shalinis-MacBook-Pro PARTD % ./operator 
0.5             1.92229e-16     -9.0681e-15     -5.9445e-17     2.64545e-15 
2.29562e-17     1.5             7.63357e-17     -1.7734e-14     -1.54411e-16 
-1.78004e-15    -7.41936e-19    2.5             -4.23718e-17    -2.21511e-14 
1.17996e-18     -7.58119e-15    7.8783e-17      3.5             5.54915e-16 
3.03753e-16     -7.38866e-17    -1.21407e-14    3.76377e-16     4.5
```

which is close to the expected matrix where the diagonal elements are the energy eigenvalues $1/2,3/2,5/2,\ldots$, while all off-diagonal elements should be zero, up to tiny numerical integration errors).