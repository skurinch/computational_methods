The system has 100 identical masses, with fixed ends, so the normal-mode problem can be written as a 
100 × 100 eigenvalue problem.

For displacements $x_i$, the equations of motion are

```math
m\ddot{x}_i = k(x_{i+1}-2x_i+x_{i-1}),
```

with the fixed-end conditions

```math
x_0 = x_{101} = 0.
```

If we assume a normal-mode solution

```math
x_i(t) = A_i e^{i\omega t},
```

then the equations become

```math
\mathbf{K}\mathbf{A} = \omega^2\mathbf{A},
```

where $\mathbf{A}$ contains the amplitudes of the 100 beads.

The matrix is tridiagonal:

```math
K =
\frac{k}{m}
\begin{pmatrix}
2 & -1 & 0 & \cdots & 0\\
-1 & 2 & -1 & \cdots & 0\\
0 & -1 & 2 & \ddots & \vdots\\
\vdots & \vdots & \ddots & \ddots & -1\\
0 & 0 & \cdots & -1 & 2
\end{pmatrix}.
```

The eigenvalues correspond to $\omega^2$, while the corresponding eigenvectors give the normal modes.

I use `Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>
` because the matrix is real and symmetric. The eigenvectors are stored as columns, so `
eigenvectors(i, mode)
` gives the $i$-th element of the corresponding eigenmode.
