// Determine the fraction of Bi-213, Tl-209, and Pb-209 at various times after the initial production of Bi-213.

#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

int main(){
    double lambda_b = std::log(2.0) / 45.6;
    double lambda_t = std::log(2.0) / 2.2;
    double lambda_p = std::log(2.0) / 195.0;

    Eigen::Matrix3d Lambda;

    Lambda << -lambda_b, 0, 0,
               lambda_b, -lambda_t, 0,
               0, lambda_t, -lambda_p;

    Eigen::Vector3d x0;
    x0 << 1, 0, 0;

    double times[] = {50, 150, 200, 500, 1000};

    for (double t : times) {
        Eigen::Vector3d x = (Lambda * t).exp() * x0;

        std::cout << "t = " << t << " min\n";
        std::cout << "Bi-213 = " << x(0) << "\n";
        std::cout << "Tl-209 = " << x(1) << "\n";
        std::cout << "Pb-209 = " << x(2) << "\n";
        std::cout << "-----------------------\n";
    }

    return 0;
}