#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    // take command-line argument
    double x = std::atof(argv[1]);

    // math library function exp
    double result = std::exp(x);

    std::cout << result << std::endl;

    return 0;
}