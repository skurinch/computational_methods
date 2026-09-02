#include "SphBesselLib.h"
#include <iostream>
#include <cmath> 
#include <algorithm>
void plotSphBessel(std::vector<std::string>& canvas, int l) {
  int height=canvas.size(), width=canvas[0].size();
  // Do the mathematical part, evaluating the function:
  std::vector<double> values(width);

  double xmin = 0.0; 
  double xmax = 20.0;

  for (int col = 0; col < width; ++col) {
    double x = xmin + (xmax - xmin) * col / (width - 1);
    double y = std::sph_bessel(l, x);
    values[col] = y;
  }

  // Find the range of y values so that the 
  // curve is scaled to fit the canvas. 
  double ymin = *std::min_element(values.begin(), values.end()); 
  double ymax = *std::max_element(values.begin(), values.end());

  // Plot curve
  for (int col = 0; col < width; ++col) {
    int row = (int) std::round( (ymax - values[col]) / (ymax - ymin) * (height - 1) ); 
    if (row >= 0 && row < height) { 
      canvas[row][col] = '*'; }
  }
   
  std::cout << "\nSpherical Bessel Function j_" << l << "(x)\n" << std::endl;
  for (const auto& line : canvas)  std::cout << line << '\n';
}

