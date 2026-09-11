// Diagonalize a 3x3 matrix by solving its secular equation by solving for its eigenvalues and eigenvectors. 

#include <iostream>
#include <complex>
#include <cmath>

int main() {
    std::complex<double> A[3][3];
    std::cout << "Enter the 3x3 matrix:\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            double x;
            std::cin >> x;
            A[i][j] = x;
        }

    // Secular equation: lambda^3 - c2*lambda^2 + c1*lambda - c0 = 0
    std::complex<double> c2 = A[0][0] + A[1][1] + A[2][2];
    std::complex<double> c1 = A[0][0]*A[1][1] + A[0][0]*A[2][2]
                            + A[1][1]*A[2][2] - A[0][1]*A[1][0]
                            - A[0][2]*A[2][0] - A[1][2]*A[2][1];
    std::complex<double> c0 = A[0][0]*(A[1][1]*A[2][2]-A[1][2]*A[2][1])
                            - A[0][1]*(A[1][0]*A[2][2]-A[1][2]*A[2][0])
                            + A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);

    // Solve cubic equation
    std::complex<double> a = -c2, b = c1, c = -c0;
    std::complex<double> p = b - a*a/3.0;
    std::complex<double> q = 2.0*a*a*a/27.0 - a*b/3.0 + c;
    std::complex<double> d = std::sqrt(q*q/4.0 + p*p*p/27.0);
    std::complex<double> u = std::pow(-q/2.0 + d, 1.0/3.0);
    std::complex<double> v = std::pow(-q/2.0 - d, 1.0/3.0);
    std::complex<double> w(-0.5, std::sqrt(3.0)/2.0);

    std::complex<double> eigenvalues[3];
    eigenvalues[0] = u + v - a/3.0;
    eigenvalues[1] = w*u + std::conj(w)*v - a/3.0;
    eigenvalues[2] = std::conj(w)*u + w*v - a/3.0;

    // Matrix P: columns are normalized eigenvectors
    std::complex<double> P[3][3];
    for (int k = 0; k < 3; k++) {
        std::complex<double> lambda = eigenvalues[k];
        std::complex<double> a1 = A[0][0]-lambda, b1 = A[0][1], c1r = A[0][2];
        std::complex<double> a2 = A[1][0], b2 = A[1][1]-lambda, c2r = A[1][2];
        std::complex<double> x = b1*c2r - c1r*b2;
        std::complex<double> y = c1r*a2 - a1*c2r;
        std::complex<double> z = a1*b2 - b1*a2;
        double length = std::sqrt(std::norm(x)+std::norm(y)+std::norm(z));
        P[0][k] = x/length;
        P[1][k] = y/length;
        P[2][k] = z/length;
    }

    std::cout << "\nEigenvalues:\n[ ";
    for (int i = 0; i < 3; i++) {
        std::cout << eigenvalues[i];
        if (i < 2) std::cout << ", ";
    }
    std::cout << " ]\n";

    std::cout << "\nEigenvector matrix P:\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "[ ";
        for (int j = 0; j < 3; j++) {
            std::cout << P[i][j];
            if (j < 2) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    return 0;
}

