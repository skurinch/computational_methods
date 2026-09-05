First, we show that $\sin(ix) = i \sinh(x)$:
```shell
(base) shalinikv@Shalinis-MacBook-Pro EX8 % ./sin_test                  
x = 0     sin(ix) = (0,0)         i*sinh(x) = (0,0)
x = 1     sin(ix) = (0,1.1752)    i*sinh(x) = (0,1.1752)
x = 2     sin(ix) = (0,3.62686)   i*sinh(x) = (0,3.62686)
x = 3     sin(ix) = (0,10.0179)   i*sinh(x) = (0,10.0179)
```
Next, we show that $\cos(ix) = \cosh(x)$:
```shell
(base) shalinikv@Shalinis-MacBook-Pro EX8 % ./cos_test                  
x = 0    cos(ix) = (1,0)          cosh(x) = 1
x = 1    cos(ix) = (1.54308,-0)   cosh(x) = 1.54308
x = 2    cos(ix) = (3.7622,-0)    cosh(x) = 3.7622
x = 3    cos(ix) = (10.0677,-0)   cosh(x) = 10.0677
```
