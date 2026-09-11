// solve the Schrodinger equation for E > 0, impinging upon a potential barrier which has a piecewise continuous form.

#include <iostream>
#include <cmath>
#include <complex>
#include <iomanip>

struct Matrix2 {
    std::complex<double> a11, a12, a21, a22;
};

Matrix2 multiply(const Matrix2& A, const Matrix2& B)
{
    return {
        A.a11*B.a11 + A.a12*B.a21,
        A.a11*B.a12 + A.a12*B.a22,
        A.a21*B.a11 + A.a22*B.a21,
        A.a21*B.a12 + A.a22*B.a22
    };
}

Matrix2 propagation(double k, double V, double L)
{
    if (std::abs(V-1.0) < 1e-12)
        return {1.0, L, 0.0, 1.0};

    if (V < 1.0) {
        double q = k*std::sqrt(1.0-V);
        return {
            std::cos(q*L),
            std::sin(q*L)/q,
            -q*std::sin(q*L),
            std::cos(q*L)
        };
    }

    double gamma = k*std::sqrt(V-1.0);
    return {
        std::cosh(gamma*L),
        std::sinh(gamma*L)/gamma,
        gamma*std::sinh(gamma*L),
        std::cosh(gamma*L)
    };
}

void calculate(double V0, double k, double& R, double& T)
{
    Matrix2 M1 = propagation(k, V0, 1.0);
    Matrix2 M2 = propagation(k, 2.0*V0, 2.0);
    Matrix2 M3 = propagation(k, V0, 1.0);

    Matrix2 M = multiply(M3, multiply(M2, M1));

    std::complex<double> I(0.0, 1.0);
    std::complex<double> A = M.a11 + I*k*M.a12;
    std::complex<double> B = M.a21 + I*k*M.a22;

    std::complex<double> r = -(B-I*k*A)/(B+I*k*A);
    std::complex<double> t = 2.0*I*k/(B+I*k*A);

    R = std::norm(r);
    T = std::norm(t);
}

int main()
{
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "# V0/E    ka    R    T    R+T\n";

    for (double V0 = 0.1; V0 <= 1.0; V0 += 0.1) {
        double k = 2.0;
        double R, T;

        calculate(V0, k, R, T);

        std::cout << V0 << " "
                  << k << " "
                  << R << " "
                  << T << " "
                  << R+T << "\n";
    }

    return 0;
}