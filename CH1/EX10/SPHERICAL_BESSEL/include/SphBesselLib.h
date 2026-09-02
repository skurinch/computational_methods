#ifndef _SPH_BESSEL_H_
#define _SPH_BESSEL_H_
#include <string>
#include <vector>

// Parse the command line, return an integer representing the degree
// of the Spherical Bessel Function to be plotted.
int parseInput(int argc, char *argv[]);

// Manufacture a plotting canvas with x and y axes.  
std::vector<std::string> getCanvas(int width, int height);

// Plot the P_l(x) on the canvas. 
void plotSphBessel(std::vector<std::string>& canvas, int l);

#endif
