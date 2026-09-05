#include <iostream>
#include <complex>
#include <cmath>

int main() {
    for (double x : {0.0, 1.0, 2.0, 3.0}) {
        std::complex<double> i(0, 1);

        std::cout << "x = " << x
                  << "\t  sin(ix) = " << std::sin(i * x)
                  << "\t  i*sinh(x) = " << i * std::sinh(x)
                  << '\n';
    }
}