I used QAT's quadrature routines to compute the integrals using the midpoint, trapezoidal, and Simpson's rules.

The exact solutions are, with the help of Mathematica:

$$
\begin{align*}
\int_0^1 \tanh(x)\\,dx &= \ln(\cosh 1) \\
\int_0^1 \sqrt{\coth(x)}\\,dx &=
  \frac{\pi}{2}
  +\mathrm{ArcCoth}\left(\sqrt{\coth 1}\right)
  -\mathrm{ArcTan}\left(\sqrt{\coth 1}\right)
\end{align*}
$$

For the second integral, $\sqrt{\coth(x)}$ is singular at $x=0$. I therefore wrote a separate function to handle the integrand and explicitly took care of the singularity at the endpoint before applying the quadrature routines.
