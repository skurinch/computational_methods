The program first asks the user to enter the $3\times3$ matrix $A$. It then finds the secular equation $\det(\lambda I-A)=0$, which gives a cubic equation $\lambda^3-c_2\lambda^2+c_1\lambda-c_0=0$. The variables $c_2$, $c_1$, and $c_0$ are the coefficients calculated from the matrix entries. The program then uses the variables $a$, $b$, $c$, $p$, $q$, $d$, $u$, $v$, and $w$ to solve this cubic equation and find the three eigenvalues. These are stored in the array `eigenvalues`. For each eigenvalue $\lambda$, the program looks at $A-\lambda I$ and uses two of its rows to calculate an eigenvector. The components $x$, $y$, and $z$ give the eigenvector, which is then normalized so that its length is $1$. These normalized eigenvectors are stored as the columns of the matrix `P`, so each column matches the eigenvalue in the same position in `eigenvalues`. Finally, the matrix can be diagonalized as $A=PDP^{-1}$, where $D$ contains the three eigenvalues on its diagonal. `std::complex<double>` is used so the program can also work with complex eigenvalues and eigenvectors.


**Example:**

```shell
(base) shalinikv@Shalinis-MacBook-Pro EX10 % ./diagonalize                     
Enter the 3x3 matrix:
0.33333 -0.244017 0.910684
0.910684 0.33333 -0.244017
-0.244017 0.910684 0.33333

Eigenvalues:
[ (1.36602,-0.211325), (-0.366029,0.788675), (-3.5e-06,-0.577351) ]

Eigenvector matrix P:
[ (0.566947,-0.109109), (-0.327327,0.4072), (-0.211325,-0.455342) ]
[ (0.327327,0.0292356), (0.566947,-0.109109), (0.788675,0.122008) ]
[ (0.705291,-0.247454), (0.0506376,-0.625418), (-5.78647e-08,0.333333) ]
```