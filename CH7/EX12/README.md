I used the transformation equation

$$
f(x) = x_\mathrm{min}\left(\frac{x_\mathrm{max}}{x_\mathrm{min}} \right)^x
$$

where $x_\mathrm{min} = 1$ and $x_\mathrm{max} = 10$. Since this equation is obtained by the integration the probability density function $\rho(x)$ and inverting the integral, $\rho(x)$ is equal to:

$$
\rho(x) = \frac{1}{x \ln (x_\mathrm{max} / x_\mathrm{min})} = \frac{1}{x \ln 10}
$$

In the code, I implement this as:

```cpp
double u = uniform(engine);
double x = xmin * std::pow(xmax / xmin, u);
```