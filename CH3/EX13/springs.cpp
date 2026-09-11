// Five metal balls are connected by springs and are displaced from their equlibrium positions.

#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <algorithm>   // for std::max
#include <unistd.h>    // for usleep

int main()
{
    const int N = 5;

    Eigen::VectorXd mass(N);
    mass << 1, 3, 2, 1, 2;

    Eigen::MatrixXd K = Eigen::MatrixXd::Zero(N, N);
    double k[4] = {1, 2, 2, 1};

    for (int i = 0; i < 4; ++i) {
        K(i, i) += k[i];
        K(i + 1, i + 1) += k[i];
        K(i, i + 1) -= k[i];
        K(i + 1, i) -= k[i];
    }

    Eigen::MatrixXd MinvSqrt = Eigen::MatrixXd::Zero(N, N);

    for (int i = 0; i < N; ++i)
        MinvSqrt(i, i) = 1.0 / std::sqrt(mass(i));

    Eigen::MatrixXd A = MinvSqrt * K * MinvSqrt;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(A);

    Eigen::VectorXd omega2 = solver.eigenvalues();
    Eigen::MatrixXd modes = solver.eigenvectors();

    Eigen::VectorXd x0(N);
    x0 << -2, 3, 0, -3, 2;

    Eigen::VectorXd q0 = mass.array().sqrt() * x0.array();

    Eigen::VectorXd c0 = modes.transpose() * q0;

    for (int n = 0; n <= 2000; ++n) {
    double t = 20.0 * n / 2000.0;

    Eigen::VectorXd c(N);

    for (int j = 0; j < N; ++j) {
        double omega = std::sqrt(std::max(0.0, omega2(j)));
        c(j) = c0(j) * std::cos(omega * t);
    }

    Eigen::VectorXd q = modes * c;
    Eigen::VectorXd x = MinvSqrt * q;

    // Clear terminal and move cursor to the top
    std::cout << "\033[2J\033[H";

    std::cout << "t = " << t << "\n\n";

    // Draw wire
    const int width = 80;
    const int center = width / 2;
    const double scale = 5.0;

    std::string wire(width, '-');

    for (int i = 0; i < N; ++i) {
        int position = center + static_cast<int>(scale * x(i));

        if (position >= 0 && position < width)
            wire[position] = '1' + i;
    }

    std::cout << wire << "\n";

    std::cout << "\n";
    std::cout << "x = " << x.transpose() << "\n";

    double xcm = mass.dot(x) / mass.sum();
    std::cout << "x_cm = " << xcm << "\n";

    std::cout.flush();

    // Control animation speed
    usleep(30000);
    }
}