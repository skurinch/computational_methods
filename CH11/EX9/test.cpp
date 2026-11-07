
#include "symplectic.cpp"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Square.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>

int main() {

  const double M = 1.0;
  const double K = 1.0;

  const double PI = std::acos(-1.0);

  Genfun::Variable q(0);
  Genfun::Variable p(0);

  Genfun::Square square;

  // Hamiltonian

  Genfun::GENFUNCTION T = (1.0/(2.0*M))*square(p);
  Genfun::GENFUNCTION V = (0.5*K)*square(q);

  // initial conditions
  SymplecticIntegrator::State initial;

  initial.t = 0.0;
  initial.q = {0.0};
  initial.p = {1.0};

  const double H0 = 1.0/(2.0*M);

  std::cout << std::setprecision(12);

  // first test
  std::cout << "\nTest solution after one oscillation...\n";

  const double h = 0.01;
  const unsigned int N = static_cast<unsigned int>(2.0*PI/h);

  SymplecticIntegrator integrator(T,V,1,h);

  auto trajectory = integrator.integrate(initial,N);

  const auto &final   = trajectory.back();
  const double omega  = std::sqrt(K/M);

  const double qExact = std::sin(omega*final.t)/(M*omega);
  const double pExact = std::cos(omega*final.t);
  const double qError = std::abs(final.q[0]-qExact);
  const double pError = std::abs(final.p[0]-pExact);

  std::cout << "Position error: " << qError << "\n";
  std::cout << "Momentum error: " << pError << "\n";

  // test 2
  std::cout << "\nTest energy conservation...\n";

  double maxEnergyError = 0.0;

  for (const auto &s : trajectory) {

    const double H = integrator.energy(s);

    const double error = std::abs(H-H0);

    maxEnergyError =
      std::max(maxEnergyError,error);
  }

  std::cout << "Maximum energy error: " << maxEnergyError << "\n";

  // test 3
  std::cout << "\nCheck for convergence...\n";

  const double tFinal = 1.0;

  auto calculateError =
    [&](double dt) {

      unsigned int steps =
        static_cast<unsigned int>(tFinal/dt);

      SymplecticIntegrator I(T,V,1,dt);

      auto result = I.integrate(initial,steps);

      const auto &s = result.back();

      const double qReference = std::sin(omega*s.t)/(M*omega);

      const double pReference =  std::cos(omega*s.t);

      return std::sqrt(std::pow(s.q[0]-qReference,2) + std::pow(s.p[0]-pReference,2));
    };

  const double e1 = calculateError(0.1);
  const double e2 = calculateError(0.05);
  const double e3 = calculateError(0.025);

  std::cout << "Error h=0.1:   " << e1 << "\n";
  std::cout << "Error h=0.05:  " << e2 << "\n";
  std::cout << "Error h=0.025: " << e3 << "\n";
  
}
