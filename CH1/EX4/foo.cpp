#include <iostream>                    // header for basic i/o
                                             //
int main (int argc, char ** argv) {    //
    for (int i=0; i<argc; i++) {       // loop over command line arguments
        std::cout << argv[i] << " ";  // print each argument to screen
    }                                      //
    std::cout << std::endl;            // end-of-line
    return 0;                            // program successful
} 