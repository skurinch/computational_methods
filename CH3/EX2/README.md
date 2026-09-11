or a radioactive species with half-life $T_{1/2}$,

$$\lambda=\frac{\ln 2}{T_{1/2}}.$$

So the three decay constants, in units of $\mathrm{min}^{-1}$, are

$$
\begin{align*}
\lambda_b &= \frac{\ln 2}{45}=0.0152 \\
\lambda_t &= \frac{\ln 2}{2.2}=0.315 \\
\lambda_p &= \frac{\ln 2}{195}=0.003.
\end{align*}
$$

The equations for the three populations are

$$
\begin{align*}
\frac{dx_b}{dt} &= -\lambda_b x_b \\
\frac{dx_t}{dt} &= \lambda_bx_b-\lambda_tx_t \\
\frac{dx_p}{dt} &= \lambda_tx_t-\lambda_px_p
\end{align*}
$$


Therefore,

$$
\begin{align*}
\frac{d}{dt}
\begin{pmatrix}
    x_b\\
    x_t\\
    x_p
\end{pmatrix}
&=
\begin{pmatrix}
    -\lambda_b & 0 & 0\\
    \lambda_b & -\lambda_t & 0\\
    0 & \lambda_t & -\lambda_p
\end{pmatrix}
\begin{pmatrix}
    x_b\\
    x_t\\
    x_p
\end{pmatrix} \\

\implies

\Lambda
&=
\begin{pmatrix}
    -0.015 & 0 & 0\\
    0.015 & -0.315 & 0\\
    0 & 0.315 & -0.003
\end{pmatrix}

\end{align*}
$$

Since the sample initially contains only $^{213}\mathrm{Bi}$,

$$
\vec{x}_0=
\begin{pmatrix}
1\\
0\\
0
\end{pmatrix}.
$$

The output is:

```
(base) shalinikv@Shalinis-MacBook-Pro EX2 % ./species                 
t = 50 min
Bi-213 = 0.467652
Tl-209 = 0.0237059
Pb-209 = 0.463827
----------------------
t = 150 min
Bi-213 = 0.102275
Tl-209 = 0.00518445
Pb-209 = 0.634293
----------------------
t = 200 min
Bi-213 = 0.0478292
Tl-209 = 0.00242452
Pb-209 = 0.582837
----------------------
t = 500 min
Bi-213 = 0.000500302
Tl-209 = 2.53609e-05
Pb-209 = 0.222537
----------------------
t = 1000 min
Bi-213 = 2.50302e-07
Tl-209 = 1.26881e-08
Pb-209 = 0.0377454
----------------------
```