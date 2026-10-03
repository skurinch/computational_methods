#include "MUELLERCALC/StokesVector.h"
#include "MUELLERCALC/MuellerMatrix.h"
#include <iostream>
#include <string>

int main (int argc, char ** argv) {

  StokesVector v0(StokesVector::Horizontal);

  MuellerMatrix PH(MuellerMatrix::Horizontal);
  MuellerMatrix DG(MuellerMatrix::Diagonal);
  MuellerMatrix PV(MuellerMatrix::Vertical);

  // make a sequence of polarizers:
  MuellerMatrix CP = PH*DG*PV;

  StokesVector v1 = CP*v0;

  std::cout << "Initial= " << v0 << std::endl;

  std::cout << "Filter= " << std::endl;

  std::cout << CP << std::endl;

  std::cout << "Final= " << v1 << std::endl;

  std::cout << "Final intensity= " << v1(0) << std::endl;

  return 1;
}