This code models the motion of five masses connected by four springs. I set the masses and spring constants in units of `m` and `k`, so the equations of motion can be written as

$$
M\ddot{x}=-Kx,
$$

where `M` is the mass matrix and `K` is the spring matrix.

I chose to use `Eigen::GeneralizedSelfAdjointEigenSolver` because the masses are not all the same. Instead of trying to first calculate `M^{-1}K`, I can directly solve the generalized eigenvalue problem

$$
Kv_j=\omega_j^2Mv_j.
$$

This gives me the normal-mode frequencies `\omega_j` and eigenvectors `v_j` directly. Since both `K` and `M` are symmetric, this solver is a natural choice for the problem.

The initial displacement is

$$
x(0)=(-2,\\;3,\\;0,\\;-3,\\;2),
$$

with all of the masses initially at rest. I decompose this initial displacement into the normal modes and then evolve each mode independently. Since the initial velocities are zero, each mode just oscillates as

$$
\cos(\omega_j t).
$$

One thing to keep in mind is that there is a zero-frequency mode because the entire system can translate without stretching any of the springs. Because of numerical roundoff, the corresponding eigenvalue can sometimes come out as a very small negative number, so I make sure to treat values very close to zero as zero before taking the square root.

I then reconstruct the positions of the five masses at each time and calculate the center-of-mass position using

$$
x_{\mathrm{CM}}=
\frac{\sum_i m_i x_i}{\sum_i m_i}.
$$

The time is measured in units of the characteristic time `\sqrt{m/k}`, and the code outputs the positions and center-of-mass motion as a function of this dimensionless time.

**Note that I used CodeX to help with visualizing the positions of the balls.**
