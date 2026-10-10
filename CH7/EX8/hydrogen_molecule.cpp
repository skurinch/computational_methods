// Shalini Kurinchi-Vendhan

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatDataAnalysis/Hist1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QColor>
#include <QKeySequence>
#include <Eigen/Dense>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>

class HydrogenMoleculeMarkovChain {

public:

    HydrogenMoleculeMarkovChain(double d, int sign, double sigma = 1.0)
        : d(d),
          sign(sign),
          gauss(0.0, sigma),
          flat(0.0, 1.0)
    {
        r = Eigen::Vector3d(0.0, 0.0, d / 2.0);
        prob = probability(r);
    }

    double radius(const Eigen::Vector3d& x, int proton) const {
        double zproton = proton * d / 2.0;
        return (x - Eigen::Vector3d(0.0, 0.0, zproton)).norm();
    }

    double phi(const Eigen::Vector3d& x, int proton) const {
        double rr = radius(x, proton);
        return std::exp(-rr) / std::sqrt(M_PI);
    }

    double amplitude(const Eigen::Vector3d& x) const {
        return phi(x, +1) + sign * phi(x, -1);
    }

    double probability(const Eigen::Vector3d& x) const {
        double a = amplitude(x);
        return a * a;
    }

    // one MCMC step
    double move() {
        Eigen::Vector3d proposal = r;

        for (int i = 0; i < 3; ++i) {
            proposal[i] += gauss(engine);
        }

        double newProb = probability(proposal);

        if (prob > 0.0 && flat(engine) < std::min(1.0, newProb / prob)) {
            r = proposal;
            prob = newProb;
            ++accepted;
        }

        ++steps;
        return prob;
    }

    // electron energy
    double localEnergy() const {
        double r1 = radius(r, +1);
        double r2 = radius(r, -1);
        double p1 = phi(r, +1);
        double p2 = phi(r, -1);
        double denom = p1 + sign * p2;

        if (r1 < 1.e-12 || r2 < 1.e-12 || std::abs(denom) < 1.e-12) {
            return 0.0;
        }

        return -0.5 - (p1 / r2 + sign * p2 / r1) / denom;
    }

    // sample from phi_1^2
    double overlapRatio() const {
        double r1 = radius(r, +1);
        double r2 = radius(r, -1);

        return std::exp(r1 - r2);
    }

    double acceptance() const {
        return steps > 0
            ? static_cast<double>(accepted) / steps : 0.0;
    }

private:
    double d;
    int sign;
    Eigen::Vector3d r;
    double prob;

    std::mt19937 engine{std::random_device{}()};
    std::normal_distribution<double> gauss;
    std::uniform_real_distribution<double> flat;

    unsigned long steps = 0;
    unsigned long accepted = 0;
};


// estimate expectation
double calculateOverlap(double d, int nSamples, double sigma) {

    HydrogenMoleculeMarkovChain chain(d, +1, sigma);

    const int burn = nSamples / 5;

    for (int i = 0; i < burn; ++i) {
        chain.move();
    }

    double sum = 0.0;

    for (int i = 0; i < nSamples; ++i) {
        chain.move();
        sum += chain.overlapRatio();
    }

    return sum / nSamples;
}


// estimate total energy
double calculateEnergy(double d, int sign, int nSamples, double sigma) {

    HydrogenMoleculeMarkovChain chain(d, sign, sigma);

    const int burn = nSamples / 5;

    for (int i = 0; i < burn; ++i) {
        chain.move();
    }

    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < nSamples; ++i) {
        chain.move();

        double e = chain.localEnergy();

        if (std::isfinite(e)) {
            sum += e;
            ++count;
        }
    }

    // Aad proton-proton electrostatic repulsion.
    return sum / count + 1.0 / d;
}


int main(int argc, char** argv) {
    
    // start plotting application
    QApplication app(argc, argv);

    // paramters
    const int NPOINTS = 40;
    const int NSAMPLES = 50000;
    const double SIGMA = 1.0;
    const double DMIN = 0.2;
    const double DMAX = 8.2;
    const double DD = (DMAX - DMIN) / NPOINTS;

    // histograms
    Hist1D NsHist("N_s", NPOINTS, DMIN, DMAX);
    Hist1D NaHist("N_a", NPOINTS, DMIN, DMAX);
    Hist1D EsHist("E_s", NPOINTS, DMIN, DMAX);
    Hist1D EaHist("E_a", NPOINTS, DMIN, DMAX);

    for (int i = 0; i < NPOINTS; ++i) {

        double d = DMIN + (i + 0.5) * DD;
        double S = calculateOverlap(d, NSAMPLES, SIGMA);

        // normalization constants.
        double Ns = 1.0 / std::sqrt(2.0 * (1.0 + S));

        // need to calculate 1 - S more stably otherwise I get nans for N_a
        double oneMinusS;
        if (d < 1e-3) {
            double d2 = d * d;
            oneMinusS = d2 / 6.0 - d2 * d2 / 24.0
                        + d2 * d2 * d / 45.0;
        } else {
            double S = std::exp(-d) * (1.0 + d + d * d / 3.0);
            oneMinusS = 1.0 - S;
        }
        double Na = 1.0 / std::sqrt(2.0 * oneMinusS);

        // total energies
        double Es = calculateEnergy(d, +1, NSAMPLES, SIGMA);
        double Ea = calculateEnergy(d, -1, NSAMPLES, SIGMA);

        NsHist.accumulate(d, Ns);
        NaHist.accumulate(d, Na);
        EsHist.accumulate(d, Es);
        EaHist.accumulate(d, Ea);

        std::cout
            << "d = " << d
            << "  S = " << S
            << "  Ns = " << Ns
            << "  Na = " << Na
            << "  Es = " << Es
            << "  Ea = " << Ea
            << "\n";
    }

    // plot normalizations
    QMainWindow normWindow;
    QToolBar* normToolbar = normWindow.addToolBar("Tools");
    QAction* normQuit = normToolbar->addAction("Quit");
    normQuit->setShortcut(QKeySequence("q"));
    QObject::connect(normQuit, SIGNAL(triggered()), &app, SLOT(quit()));
    PlotView normView({DMIN, DMAX, 0.0, 2.0});
    normWindow.setCentralWidget(&normView);

    PlotHist1D NsPlot(NsHist);
    PlotHist1D NaPlot(NaHist);
    normView.add(&NsPlot);
    normView.add(&NaPlot);

    PlotStream normTitle(normView.titleTextEdit());
    normTitle << PlotStream::Clear()
              << PlotStream::Center()
              << PlotStream::Family("Times")
              << PlotStream::Size(16)
              << "Normalization constants"
              << PlotStream::EndP();

    PlotStream normX(normView.xLabelTextEdit());
    normX << PlotStream::Clear()
          << PlotStream::Center()
          << PlotStream::Family("Times")
          << PlotStream::Size(16)
          << "d [Bohr radii]"
          << PlotStream::EndP();

    PlotStream normY(normView.yLabelTextEdit());
    normY << PlotStream::Clear()
          << PlotStream::Center()
          << PlotStream::Family("Times")
          << PlotStream::Size(16)
          << "N"
          << PlotStream::EndP();

    // plot energies
    QMainWindow energyWindow;
    QToolBar* energyToolbar = energyWindow.addToolBar("Tools");
    QAction* energyQuit = energyToolbar->addAction("Quit");
    energyQuit->setShortcut(QKeySequence("q"));
    QObject::connect(energyQuit, SIGNAL(triggered()), &app, SLOT(quit()));
    PlotView energyView({DMIN, DMAX, -1.0, 1.0});
    energyWindow.setCentralWidget(&energyView);

    PlotHist1D EsPlot(EsHist);
    PlotHist1D EaPlot(EaHist);

    energyView.add(&EsPlot);
    energyView.add(&EaPlot);

    PlotStream energyTitle(energyView.titleTextEdit());
    energyTitle << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times")
                << PlotStream::Size(16)
                << "Total Energies"
                << PlotStream::EndP();

    PlotStream energyX(energyView.xLabelTextEdit());
    energyX << PlotStream::Clear()
            << PlotStream::Center()
            << PlotStream::Family("Times")
            << PlotStream::Size(16)
            << "d [Bohr radii]"
            << PlotStream::EndP();

    PlotStream energyY(energyView.yLabelTextEdit());
    energyY << PlotStream::Clear()
            << PlotStream::Center()
            << PlotStream::Family("Times")
            << PlotStream::Size(16)
            << "E [Hartrees]"
            << PlotStream::EndP();

    normWindow.show();
    energyWindow.show();

    return app.exec();
}