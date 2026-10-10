For this exercise, I changed the sampling method from direct Gamma sampling to Metropolis-Hastings MCMC.

The chain samples the dimensionless variable

$$
x = \frac{mv^2}{2k_BT},
$$

using the target distribution

$$
p(x) \propto x^{1/2}e^{-x}.
$$

At each step, a new $x$ is proposed using a Gaussian distribution. The proposal is accepted if it has a higher probability, or otherwise accepted with probability $p(x_\mathrm{new})/p(x_\mathrm{old})$. Negative proposals are rejected.
