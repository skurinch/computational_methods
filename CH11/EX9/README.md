The integrator uses the Störmer–Verlet method to solve Hamilton's equations. It works with Hamiltonians that can be split into kinetic and potential energy, $H(q,p)=T(p)+V(q)$. It uses `QatGenericFunctions` to automatically calculate the derivatives of the energy functions. Each time step updates the momentum, then the position, and finally the momentum again.

To check that everything is working, I use a simple harmonic oscillator and run three tests:

1. I compare the numerical position and momentum after approximately one complete oscillation with the exact analytical solution.

2. I track the total energy throughout the simulation and measure its maximum deviation from the initial energy.

3. I repeat the calculation using smaller time steps (0.1, 0.05, and 0.025) and check that the error decreases as expected.
