// solve the Schrodinger equation for E > 0, impinging upon a potential barrier which has a piecewise continuous form.

#include <iostream>
#include <cmath>
#include <complex>
#include <iomanip>

int main(){
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "# V0/E   ka       R        T        R+T\n";

    for (double V0 = 0.1; V0 <= 1.0; V0 += 0.1) {

        double k = 2.0;

        // First region
        double q = k * std::sqrt(1.0 - V0);

        std::complex<double> a = std::cos(q);
        std::complex<double> b = std::sin(q) / q;
        std::complex<double> c = -q * std::sin(q);
        std::complex<double> d = std::cos(q);

        // Middle region
        double q2 = k * std::sqrt(std::abs(1.0 - 2.0 * V0));

        std::complex<double> e, f, g, h;

        if (2.0 * V0 < 1.0) {
            e = std::cos(2.0 * q2);
            f = std::sin(2.0 * q2) / q2;
            g = -q2 * std::sin(2.0 * q2);
            h = std::cos(2.0 * q2);
        }
        else {
            e = std::cosh(2.0 * q2);
            f = std::sinh(2.0 * q2) / q2;
            g = q2 * std::sinh(2.0 * q2);
            h = std::cosh(2.0 * q2);
        }

        // M = M3 * M2 * M1, where M3 = M1
        std::complex<double> x = e*a + f*c;
        std::complex<double> y = e*b + f*d;
        std::complex<double> z = g*a + h*c;
        std::complex<double> w = g*b + h*d;

        std::complex<double> M11 = a*x + b*z;
        std::complex<double> M12 = a*y + b*w;
        std::complex<double> M21 = c*x + d*z;
        std::complex<double> M22 = c*y + d*w;

        std::complex<double> I(0.0, 1.0);

        std::complex<double> A = M11 + I*k*M12;
        std::complex<double> B = M21 + I*k*M22;

        std::complex<double> r =
            -(B - I*k*A) / (B + I*k*A);

        std::complex<double> t =
            2.0*I*k / (B + I*k*A);

        double R = std::norm(r);
        double T = std::norm(t);

        std::cout << V0 << " "
                  << k << " "
                  << R << " "
                  << T << " "
                  << R + T << "\n";
    }

    return 0;
}