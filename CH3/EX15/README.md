This script uses the Newton-Raphson method to find the zeros of the Legendre polynomial $P_l(x)$, where $l$ is specified from the command line. The Legendre polynomials are plotted over $[-1,1]$, with their zeros marked on the plot, and the calculated roots are also printed to `std::cout`. 
The resulting plot is saved with the filename `root_l*.jpg`.

```shell
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 1
zero 1 = 0
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 2
zero 1 = 0.57735
zero 2 = 0.57735
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 3
zero 1 = 0.774597
zero 2 = 0.774597
zero 3 = -1.0111e-28
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 4
zero 1 = 0.861136
zero 2 = 0.861136
zero 3 = 0.339981
zero 4 = -0.339981
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 5
zero 1 = 0.90618
zero 2 = 0.90618
zero 3 = 0.538469
zero 4 = -1.31671e-27
zero 5 = -0.538469
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 6
zero 1 = 0.93247
zero 2 = 0.93247
zero 3 = 0.661209
zero 4 = 0.238619
zero 5 = -0.238619
zero 6 = -0.661209
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 7
zero 1 = 0.949108
zero 2 = 0.949108
zero 3 = 0.741531
zero 4 = 0.405845
zero 5 = -2.54393e-27
zero 6 = -0.405845
zero 7 = -0.741531
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 8
zero 1 = 0.96029
zero 2 = 0.96029
zero 3 = 0.796666
zero 4 = 0.525532
zero 5 = 0.183435
zero 6 = -0.183435
zero 7 = -0.525532
zero 8 = -0.796666
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 9
zero 1 = 0.96816
zero 2 = 0.96816
zero 3 = 0.836031
zero 4 = 0.613371
zero 5 = 0.324253
zero 6 = -8.96836e-28
zero 7 = -0.324253
zero 8 = -0.613371
zero 9 = -0.836031
(base) shalinikv@Shalinis-MacBook-Pro EX15 % ./newton_raphson 10
zero 1 = 0.973907
zero 2 = 0.973907
zero 3 = 0.865063
zero 4 = 0.67941
zero 5 = 0.433395
zero 6 = 0.148874
zero 7 = -0.148874
zero 8 = -0.433395
zero 9 = -0.67941
zero 10 = -0.865063
```