I calculate the reflection $R$ and transmission $T$ probabilities for a particle moving through a three-region potential:

$V_0 \rightarrow 2V_0 \rightarrow V_0$.

I use the transfer-matrix method for this calculation. I represent each region of the potential with a $2\times2$ matrix that describes how the wavefunction changes as it passes through that region. I then multiply the three matrices together to get the total transfer matrix:

$M=M_3 M_2 M_1$.

For a region where $E>V$, the wavefunction is oscillatory, so I use `sin` and `cos` in the transfer matrix. When $E<V$, the wavefunction is exponentially growing or decaying, so I instead use `sinh` and `cosh`.

After calculating the total matrix, I use its elements to find the complex reflection amplitude $r$ and transmission amplitude $t$:

```cpp
r = -(B - I*k*A) / (B + I*k*A);
t = 2.0*I*k / (B + I*k*A);
```

Since $r$ and $t$ are amplitudes, I calculate the corresponding probabilities from their squared magnitudes:

$R=|r|^2$ and $T=|t|^2$.

I also calculate $R+T$ as a check on the result. Since the potential is real and there is no absorption, probability should be conserved, so I expect:

$R+T\approx1$.

Finally, I repeat the calculation for different values of the potential, from $V_0=0.1$ to $V_0=1.0$, to see how the reflection and transmission probabilities change as the potential height increases.
