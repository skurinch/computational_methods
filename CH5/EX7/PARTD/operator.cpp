// derivative operator

#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/GaussIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"

#include <iostream>
#include <cmath>

using namespace Genfun;


// define the Hermite polynomial H_n(x)
double hermite(int n, double x)
{
    if (n == 0)
        return 1.0;

    if (n == 1)
        return 2.0 * x;

    double H0 = 1.0;
    double H1 = 2.0 * x;

    for (int k = 2; k <= n; ++k)
    {
        double H2 = 2.0 * x * H1 - 2.0 * (k - 1) * H0;
        H0 = H1;
        H1 = H2;
    }

    return H1;
}


// compute the Hamiltonian integrand: H_i(x) * [ -1/2 d^2/dx^2 + 1/2 x^2 ] H_j(x)
class XProduct : public AbsFunction
{
public:

    XProduct(int i, int j) : i_(i), j_(j) {}
    double operator()(double x) const
    {
        double Hj = hermite(j_, x);

        // first derivative of H_j
        double dHj = 0.0;

        if (j_ > 0)
            dHj = 2.0 * j_ * hermite(j_ - 1, x);

        // second derivative of H_j
        double d2Hj = 0.0;

        if (j_ > 1)
            d2Hj = 4.0 * j_ * (j_ - 1) * hermite(j_ - 2, x);

        // second derivative of H_j(x) exp(-x^2/2),
        double d2psi = d2Hj - 2.0 * x * dHj + (x * x - 1.0) * Hj;

        double Hpsi = -0.5 * d2psi + 0.5 * x * x * Hj;

        return hermite(i_, x) * Hpsi;
    }

    double operator()(const Argument& x) const
    {
        return (*this)(x[0]);
    }

    XProduct* clone() const
    {
        return new XProduct(*this);
    }

private:
    int i_, j_;
};

int main(int argc, char **argv)
{
    int N = 100;

    if (argc == 2)
    {
        N = std::stoi(argv[1]);
    }

    // Use Gauss-Hermite quadrature
    GaussHermiteRule rule(N);
    GaussIntegrator integrator(rule, GaussIntegrator::INTEGRATE_WX_DX);

    // Compute the 5x5 x-hat matrix for i,j = 0,...,4
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            XProduct f(i, j);

            double integral = integrator(f);

            // Normalization of the Hermite basis states
            double normalization = 1.0 / std::sqrt(std::pow(2.0, i + j) * std::tgamma(i + 1) 
                                                            * std::tgamma(j + 1) * M_PI);

            double Aij = normalization * integral;

            std::cout << Aij << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}