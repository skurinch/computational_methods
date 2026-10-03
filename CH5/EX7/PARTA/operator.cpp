// position operator

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


// compute H_i(x) * x * H_j(x)
class XProduct : public AbsFunction
{
public:

    XProduct(int i, int j) : i_(i), j_(j) {}

    double operator()(double x) const
    {
        return hermite(i_, x) * x * hermite(j_, x);
    }

    double operator()(const Argument& x) const
    {
        return hermite(i_, x[0]) * x[0] * hermite(j_, x[0]);
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