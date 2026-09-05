#include <iostream>
#include <complex>
#include <cmath>

int main() {
    for (double x : {0.0, 1.0, 2.0, 3.0}) {
        std::complex<double> i(0, 1);

        std::cout << "x = " << x
                  << "\t cos(ix) = " << std::cos(i * x)
                  << "\t  cosh(x) = " << std::cosh(x)
                  << '\n';
    }
}