For a pendulum with maximum angle $\theta_{\max}$, the equation of motion is

$$
\ddot{\theta} = -\frac{g}{l}\sin\theta.
$$

This is the non-linear pendulum problem, described in [this article](https://math.libretexts.org/Bookshelves/Differential_Equations/A_First_Course_in_Differential_Equations_for_Scientists_and_Engineers_(Herman)/07%3A_Nonlinear_Systems/7.09%3A_The_Period_of_the_Nonlinear_Pendulum). I used conservation of energy to write the period as an integral:

$$
T = 4\sqrt{\frac{l}{2g}}
\int_0^{\theta_{\max}}
\frac{d\theta}
{\sqrt{\cos\theta-\cos\theta_{\max}}}.
$$

The small-angle period is

$$
T_0 = 2\pi\sqrt{\frac{l}{g}},
$$

so the quantity I need is

$$
f(\theta_{\max}) = \frac{T}{T_0}.
$$

The original integrand has a square-root singularity at $\theta=\theta_{\max}$. I removed this by making the substitution

$$
\sin\frac{\theta}{2} = \sin\frac{\theta_{\max}}{2}\sin\phi.
$$

This gives

$$
f(\theta_{\max}) = \frac{2}{\pi} \int_0^{\pi/2} \frac{d\phi} {\sqrt{1-\sin^2(\theta_{\max}/2)\sin^2\phi}}.
$$

I then evaluated this integral numerically using QAT's Simpson quadrature for

$$
\theta_{\max} = \frac{\pi}{12}, \frac{\pi}{6}, \frac{\pi}{3}, \frac{2\pi}{3}, \frac{5\pi}{6}, \frac{11\pi}{12}.
$$

I also plotted the dimensionless integrand as a function of $\phi$. For small amplitudes the integrand is nearly flat, while for larger amplitudes it increases strongly near $\phi=\pi/2$. This reflects the fact that a pendulum moves more slowly near its turning points at large amplitudes, so its period becomes longer than the small-angle period.
