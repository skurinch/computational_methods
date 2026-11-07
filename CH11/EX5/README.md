This program simulates a heavy symmetric spinning top using the Hamiltonian approach. The Hamiltonian is

$$
H = \frac{1}{2I_1}
\left(
p_\theta^2+
\frac{(p_\phi-p_\psi\cos\theta)^2}{\sin^2\theta}
\right)
+\frac{p_\psi^2}{2I_3}
+Mgl\cos\theta.
$$

Here, $u=\cos\theta$, $\psi$ is the spin angle, and $\phi$ is the precession angle. The simulation starts with $\theta_0=\pi/3$, $\psi_0=\phi_0=0$, and angular velocities $\dot\theta_0=0.4$, $\dot\psi_0=20$, and $\dot\phi_0=1$. The equations of motion are solved using the classical Runge–Kutta solver from `QatGenericFunctions`. We then plot $u$, $\psi$, and $\phi$ over time and check that the total energy stays approximately constant.