#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {

    int x = std::atoi(argv[1]);
    int y = std::atoi(argv[2]);

    std::cout << x << "+" << y << "=" << x + y << std::endl;

    return 0;
}