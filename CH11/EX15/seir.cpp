#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/RKIntegrator.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/CubicSplinePolynomial.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/PlotHist1DDialog.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QKeySequence>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace Genfun;

// state: S, E, I, R.
using State = std::array<double, 4>;

static constexpr double beta  = 0.2;
static constexpr double gamma = 0.1;
static constexpr double b     = 1.0e-4;
static constexpr double d     = 1.0e-4;
static constexpr double alpha = 0.1;
static constexpr double nu    = 1.0e-3;

static constexpr double N0 = 1.0;

State derivatives(const State& y) {
    const double S = y[0];
    const double E = y[1];
    const double I = y[2];
    const double R = y[3];

    // Frequency-dependent transmission.
    const double infection = beta * S * I / N0;

    return {{
        b * N0 - infection + nu * R - d * S,
        infection - alpha * E - d * E,
        alpha * E - gamma * I - d * I,
        gamma * I - nu * R - d * R
    }};
}

State scale(const State& y, const State& k, double scale) {
    State result{};
    for (int i = 0; i < 4; ++i) {
        result[i] = y[i] + scale * k[i];
    }
    return result;
}

State tstep(const State& y, double dt) {
    const State k1 = derivatives(y);
    const State k2 = derivatives(scale(y, k1, 0.5 * dt));
    const State k3 = derivatives(scale(y, k2, 0.5 * dt));
    const State k4 = derivatives(scale(y, k3, dt));

    State next{};
    for (int i = 0; i < 4; ++i) {
        next[i] = y[i] + (dt / 6.0) *
            (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    }
    return next;
}

int main(int argc, char** argv) {

    // start plotting program
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("SEIR Epidemic Model");

    PlotView view({0.0, 5000.0, 0.0, 1.0});
    window.setCentralWidget(&view);

    // parameters
    const double tEnd = 3000.0;
    const double dt = 1;
    const int steps = static_cast<int>(tEnd / dt);

    // initial population: S, E, I, R
    State y{{N0 - 1e-6, 0.0, 1e-6, 0.0}};

    // create interpolation functions
    CubicSplinePolynomial SInterp;
    CubicSplinePolynomial EInterp;
    CubicSplinePolynomial IInterp;
    CubicSplinePolynomial RInterp;

    double t = 0.0;

    // store initial conditions
    SInterp.addPoint(t, y[0]);
    EInterp.addPoint(t, y[1]);
    IInterp.addPoint(t, y[2]);
    RInterp.addPoint(t, y[3]);

    // run the SEIR simulation using RK4
    for (int i = 0; i < steps; ++i) {

        y = tstep(y, dt);
        t += dt;

        SInterp.addPoint(t, y[0]);
        EInterp.addPoint(t, y[1]);
        IInterp.addPoint(t, y[2]);
        RInterp.addPoint(t, y[3]);
    }


    PlotFunction1D SPlot{SInterp};
    PlotFunction1D EPlot{EInterp};
    PlotFunction1D IPlot{IInterp};
    PlotFunction1D RPlot{RInterp};

    PlotFunction1D::Properties prop;

    prop.pen.setColor("Blue");
    SPlot.setProperties(prop);
    view.add(&SPlot);

    prop.pen.setColor("Red");
    EPlot.setProperties(prop);
    view.add(&EPlot);

    prop.pen.setColor("Green");
    IPlot.setProperties(prop);
    view.add(&IPlot);

    prop.pen.setColor("black");
    RPlot.setProperties(prop);
    view.add(&RPlot);

    PlotStream xLabelStream(view.xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                 << PlotStream::Center()
                 << "time [days]"
                 << PlotStream::EndP();

    PlotStream yLabelStream(view.yLabelTextEdit());
    yLabelStream << PlotStream::Clear()
                 << PlotStream::Center()
                 << "population fraction"
                 << PlotStream::EndP();

    window.show();
    return app.exec();
}