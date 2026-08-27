#include <cstdlib>

int parse(int argc, char* argv[], int& x, int& y) {

    x = std::atoi(argv[1]);
    y = std::atoi(argv[2]);

    return 0;
}