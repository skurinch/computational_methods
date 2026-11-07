#include "QatGenericFunctions/RungeKuttaClassicalSolver.h"
#include "QatGenericFunctions/PhaseSpace.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Power.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "plot.h"

using namespace Genfun;

int main(int argc, char **argv) {

    // define parameters
    const double I1 = 1.0;       // transverse moment of inertia
    const double I3 = 0.5;       // axial moment of inertia
    const double m  = 1.0;       // mass
    const double g  = 9.81;      // gravitational acceleration
    const double l  = 1.0;       // distance to center of mass

    // initial conditions
    const double theta0     = M_PI / 3.0;
    const double u0         = cos(theta0);
    const double psi0       = 0.0;
    const double phi0       = 0.0;
    const double thetaDot0  = 0.4;
    const double psiDot0    = 20.0;
    const double phiDot0    = 1.0;

    // momenta
    const double pu0   = -I1 * thetaDot0 / sin(theta0);
    const double ppsi0 = I3 * (psiDot0 + u0 * phiDot0);
    const double pphi0 = I1 * (1.0 - u0*u0) * phiDot0 + u0 * ppsi0;

    // define phase space
    Classical::PhaseSpace phaseSpace(3);

    const Classical::PhaseSpace::Component 
        & q = phaseSpace.coordinates(),
        & p = phaseSpace.momenta();

    // hamiltonian
    GENFUNCTION a = 1.0 - q[0]*q[0];

    GENFUNCTION H = a * p[0]*p[0] / (2.0*I1)
                    + (p[2] - q[0]*p[1]) *(p[2] - q[0]*p[1]) / (2.0*I1*a)
                    + p[1]*p[1] / (2.0*I3)
                    + m*g*l*q[0];

    
    phaseSpace.start(q[0], u0);
    phaseSpace.start(q[1], psi0);
    phaseSpace.start(q[2], phi0);
    phaseSpace.start(p[0], pu0);
    phaseSpace.start(p[1], ppsi0);
    phaseSpace.start(p[2], pphi0);

    // solve Hamiltonian
    Classical::RungeKuttaSolver solver(H, phaseSpace);

    // start the plotting application
    QApplication app(argc, argv);

    plot(app, solver.equationOf(q[0]), "", "t", "u");

    plot(app, solver.equationOf(q[1]), "", "t", "psi");

    plot(app, solver.equationOf(q[2]), "", "t", "phi");

    return app.exec();
}