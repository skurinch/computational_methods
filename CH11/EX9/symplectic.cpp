#ifndef SYMPLECTICINTEGRATOR_H
#define SYMPLECTICINTEGRATOR_H

#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Argument.h"
#include "QatGenericFunctions/Variable.h"
#include <vector>
#include <stdexcept>
#include <cstddef>

class SymplecticIntegrator {

public:

    struct State {
        double t;
        std::vector<double> q;
        std::vector<double> p;
    };

    SymplecticIntegrator(
        const Genfun::AbsFunction& T,
        const Genfun::AbsFunction& V,
        unsigned int n,
        double h
    )
        : _T(T),
          _V(V),
          _n(n),
          _h(h)
    {
        if (n == 0 || h == 0.0)
            throw std::invalid_argument("Invalid integrator parameters");

        // Construct automatic partial derivatives.
        for (unsigned int i = 0; i < n; ++i) {

            Genfun::Variable v(i);

            _dT.push_back(_T.partial(v));
            _dV.push_back(_V.partial(v));
        }
    }

    // evaluate all derivatives of T at p.
    std::vector<double> velocity(const std::vector<double>& p) const {

        if (p.size() != _n)
            throw std::invalid_argument("Incorrect momentum dimension");

        Genfun::Argument a(_n);

        for (unsigned int i = 0; i < _n; ++i)
            a[i] = p[i];

        std::vector<double> result(_n);

        for (unsigned int i = 0; i < _n; ++i)
            result[i] = _dT[i](a);

        return result;
    }

    // evaluate all derivatives of V at q.
    std::vector<double> force(const std::vector<double>& q) const {

        if (q.size() != _n)
            throw std::invalid_argument("Incorrect position dimension");

        Genfun::Argument a(_n);

        for (unsigned int i = 0; i < _n; ++i)
            a[i] = q[i];

        std::vector<double> result(_n);

        for (unsigned int i = 0; i < _n; ++i)
            result[i] = -_dV[i](a);

        return result;
    }

    // one step.
    void step(State& s) const {

        if (s.q.size() != _n || s.p.size() != _n)
            throw std::invalid_argument("Incorrect phase-space dimension");

        std::vector<double> F = force(s.q);

        for (unsigned int i = 0; i < _n; ++i)
            s.p[i] += 0.5 * _h * F[i];

        std::vector<double> v = velocity(s.p);

        for (unsigned int i = 0; i < _n; ++i)
            s.q[i] += _h * v[i];

        F = force(s.q);

        for (unsigned int i = 0; i < _n; ++i)
            s.p[i] += 0.5 * _h * F[i];

        s.t += _h;
    }

    // integrate and return trajectory.
    std::vector<State> integrate(
        State initial,
        unsigned int numberOfSteps
    ) const {

        std::vector<State> trajectory;

        trajectory.reserve(numberOfSteps + 1);

        trajectory.push_back(initial);

        for (unsigned int i = 0; i < numberOfSteps; ++i) {
            step(initial);
            trajectory.push_back(initial);
        }

        return trajectory;
    }

    double energy(const State& s) const {

        Genfun::Argument aq(_n), ap(_n);

        for (unsigned int i = 0; i < _n; ++i) {
            aq[i] = s.q[i];
            ap[i] = s.p[i];
        }

        return _T(ap) + _V(aq);
    }

private:

    const Genfun::AbsFunction& _T;
    const Genfun::AbsFunction& _V;

    unsigned int _n;
    double _h;

    std::vector<Genfun::Derivative> _dT;
    std::vector<Genfun::Derivative> _dV;
};

#endif