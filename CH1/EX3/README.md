## _observations_
By switching between my version and the math libary `exp`, `nm | c++filt` shows that `exp` is external because it appears with an 
undefined `U` symbol EX2. Here, `_exp` disappears because the exponential calculation is done using the Taylor series. Below, I have 
tabulated the values $x$ versus $\exp(x)$ for $x = {-10, -8, -6, ..., 6, 8, 10}$, which converge between both methods.

| **x** | **EX2: `exp(x)`** | **EX3: Taylor Series** |
| ----: | ----------------: | ---------------------: |
|   –10 |       4.54 × 10⁻⁵ |            4.54 × 10⁻⁵ |
|    –8 |       3.35 × 10⁻⁴ |            3.35 × 10⁻⁴ |
|    –6 |       2.48 × 10⁻³ |            2.48 × 10⁻³ |
|    –4 |       1.83 × 10⁻² |            1.83 × 10⁻² |
|    –2 |             0.135 |                  0.135 |
|     0 |             1.000 |                  1.000 |
|     2 |             7.389 |                  7.389 |
|     4 |            54.598 |                 54.598 |
|     6 |            403.42 |                 403.42 |
|     8 |           2,980.9 |                2,980.9 |
|    10 |            22,026 |                 22,026 |


