#include <iostream>
#include <cmath>

int main() {

    // constant argument
    // double x = 2.0;

    // variable argument
    double x;

    std::cout << "Enter x: ";
    std::cin >> x;

    // math libary function exp
    double result = std::exp(x);

    std::cout << result << std::endl;

    return 0;
}