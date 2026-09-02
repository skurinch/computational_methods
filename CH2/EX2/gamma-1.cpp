#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(10);

    for (int n = 1; n <= 10; ++n) {
        // beta = 0.9, 0.99, 0.999, ...
        double beta = 1.0 - std::pow(10.0, -n);

        double gamma = 1.0 / std::sqrt(1.0 - beta * beta);

        std::cout << "beta = " << beta
                  << "    gamma = " << gamma << '\n';
    }

    return 0;
}