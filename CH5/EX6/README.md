This program calculates the overlap matrix

$$
A_{ij} = \langle \phi_i | \phi_j \rangle
$$

for the first six harmonic oscillator eigenfunctions. The normalized eigenfunctions are

$$
\phi_n(x) =
\frac{1}{\sqrt{2^n n! \sqrt{\pi}}}
H_n(x)e^{-x^2/2},
$$

where $H_n(x)$ are the Hermite polynomials. When calculating the overlap, we multiply two eigenfunctions:

$$
\phi_i(x)\phi_j(x)
\propto H_i(x)H_j(x)e^{-x^2}.
$$

This is why the program calculates the product of two Hermite polynomials, $H_i(x)H_j(x)$. The factor $e^{-x^2}$ is already the weight used by Gauss-Hermite quadrature, so I did not include it in the function passed to the integrator.

The program uses QAT's `GaussHermiteRule` and `GaussIntegrator`:

```
GaussHermiteRule rule(N);
GaussIntegrator  integrator(rule,GaussIntegrator::INTEGRATE_WX_DX);
```

`INTEGRATE_WX_DX` tells QAT to include the Hermite weight $e^{-x^2}$ in the integral. I found that at least six points were needed to show that the functions are orthonormal.

```
shalinikv@Shalinis-MacBook-Pro EX6 % ./hermite 6
1 7.09709e-17 -3.60172e-18 -6.39519e-17 4.34679e-16 1.01934e-16 
7.09709e-17 1 -1.13377e-17 8.61267e-16 6.38626e-17 -1.86241e-16 
-3.60172e-18 -1.13377e-17 1 8.64894e-17 5.26223e-16 3.36673e-17 
-6.39519e-17 8.61267e-16 8.64894e-17 1 1.5496e-16 -4.63921e-16 
4.34679e-16 6.38626e-17 5.26223e-16 1.5496e-16 1 7.9041e-17 
1.01934e-16 -1.86241e-16 3.36673e-17 -4.63921e-16 7.9041e-17 1 
```