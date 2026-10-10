The Maxwell-Boltzmann speed distribution is

$$
\rho(v) \propto v^2 \exp\left(-\frac{mv^2}{2k_BT}\right).
$$

If we define the dimensionless variable

$$
x = \frac{mv^2}{2k_BT}.
$$

The distribution then becomes

$$
\rho(x) =
\frac{2}{\sqrt{\pi}}x^{1/2}e^{-x},
$$

which is a Gamma distribution with

$$
\boxed{x\sim\mathrm{Gamma}(3/2,1)}.
$$

Therefore, we can sample $x$ using

```cpp
std::gamma_distribution<double> gamma(1.5, 1.0);
```

and convert back to the physical speed using

```cpp
double x = gamma(engine);
double v = sqrt(2.0 * kT / m * x);
```
