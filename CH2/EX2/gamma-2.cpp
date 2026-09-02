#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(10);

    for (int n = 1; n <= 10; ++n) {
        // epsilon = 0.1, 0.01, 0.001, ...
        double eps = std::pow(10.0, -n);

        double gamma = 1.0 / std::sqrt((2.0 - eps) * eps);

        std::cout << "epsilon = " << eps
                  << "    gamma = " << gamma << '\n';
    }

    return 0;
}