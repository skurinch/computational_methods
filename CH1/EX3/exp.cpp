#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {

    double x = std::atof(argv[1]);

    double result = 1.0;
    double term = 1.0;

    // Taylor series expansion. Need to use a high value of n.
    for (int n = 1; n <= 100; n++) {
        term = term * x / n;
        result = result + term;
    }

    std::cout << result << std::endl;

    return 0;
}