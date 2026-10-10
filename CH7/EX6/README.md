The probability distribution is now defined by:

```cpp
    double P1 = mu;
    double P3 = 0.5 * (5.0 * pow(mu, 3) - 3.0 * mu);
    double P5 = 0.125 * (63.0 * pow(mu, 5) - 70.0 * pow(mu, 3) + 15.0 * mu);
    double P7 = (1.0 / 16.0) * (429.0 * pow(mu, 7) - 693.0 * pow(mu, 5) + 315.0 * pow(mu, 3) - 35.0 * mu);

    double amplitude = sqrt(3.0 / 8.0)  * P1 - sqrt(7.0 / 6.0)  * P3 + sqrt(11.0 / 24.0) * P5
                           - sqrt(15.0 / 6.0)  * P7;
```
