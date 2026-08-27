#include <iostream>

int  parse(int argc, char* argv[], int& x, int& y);
int  add(int x, int y);
void print(int x, int y, int result);

int main(int argc, char* argv[]) {
    int x, y;

    if (parse(argc, argv, x, y) != 0) {
        return 1;
    }

    int result = add(x, y);

    print(x, y, result);

    return 0;
}