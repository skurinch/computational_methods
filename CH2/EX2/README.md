When I calculate the stretch factor using $\beta = 0.9, 0.99, 0.999, 0.9999, ...$

```
(base) shalinikv@Shalinis-MacBook-Pro EX2 % ./gamma-1
beta = 0.9000000000    gamma = 2.2941573387
beta = 0.9900000000    gamma = 7.0888120501
beta = 0.9990000000    gamma = 22.3662720421
beta = 0.9999000000    gamma = 70.7124459519
beta = 0.9999900000    gamma = 223.6073567696
beta = 0.9999990000    gamma = 707.1069579531
beta = 0.9999999000    gamma = 2236.0680339900
beta = 0.9999999900    gamma = 7071.0678117779
beta = 0.9999999990    gamma = 22360.6800967897
beta = 0.9999999999    gamma = 70710.6751951088
```

When I calculate the stretch factor using $\epsilon = 0.1, 0.01, 0.001, 0.0001 ...$

```
(base) shalinikv@Shalinis-MacBook-Pro EX2 % ./gamma-2
epsilon = 0.1000000000    gamma = 2.2941573387
epsilon = 0.0100000000    gamma = 7.0888120501
epsilon = 0.0010000000    gamma = 22.3662720421
epsilon = 0.0001000000    gamma = 70.7124459519
epsilon = 0.0000100000    gamma = 223.6073567691
epsilon = 0.0000010000    gamma = 707.1069579633
epsilon = 0.0000001000    gamma = 2236.0680334015
epsilon = 0.0000000100    gamma = 7071.0678295431
epsilon = 0.0000000010    gamma = 22360.6797805881
epsilon = 0.0000000001    gamma = 70710.6781204225
```

For standard C++ double 16 decimal digits of precision, so the practical limit for 0.1% accuracy is roughly $\beta = 1 - 10^{-13}$.
