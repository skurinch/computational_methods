// Shalini Kurinchi-Vendhan

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include <cmath>

// generate the Maxwell-Boltzmann distribution using the MCMC
class MaxwellBoltzmannMarkovChain {

public:

  MaxwellBoltzmannMarkovChain(double sigma = 1.0)
    : gauss(0.0, sigma) {}

  double probability(double x) {

    if (x <= 0.0) {
      return 0.0;
    }

    return sqrt(x) * exp(-x);
  }


  double move(double &x, double prob = 0.0) {

    // propose a new value
    double xProp = x + gauss(engine);

    // reject negative values
    if (xProp <= 0.0) {
      return prob;
    }

    double newProb = probability(xProp);

    // automatically accept
    if (newProb > prob) {
      x = xProp;
      return newProb;
    }

    // accept with probability newProb / prob
    if (flat(engine) < newProb / prob) {
      x = xProp;
      return newProb;
    }

    // reject proposal
    return prob;
  }


private:
  std::mt19937 engine;
  std::uniform_real_distribution<double> flat;
  std::normal_distribution<double> gauss;
};


int main(int argc, char **argv) {

  std::random_device dev;
  std::mt19937 engine(dev());

  // parameters
  constexpr unsigned int N{10000};
  constexpr double E{3 * 100000};
  constexpr double m{1.0};
  constexpr double kT{2.0 / 3.0 * E / N};
  constexpr unsigned int NPOINTS{10000};
  constexpr double sigma{1.0};

  // start plotting application
  QApplication app(argc, argv);
  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
  PlotView view({0, 20, 0, 400.0});
  window.setCentralWidget(&view);

  // plot histogram
  Hist1D mbVelocityHist{"Velocities", 100, 0, 20.0};

  // generate distribution using MCMC
  MaxwellBoltzmannMarkovChain chain(sigma);

  double x = 1.0;
  double prob = chain.probability(x);

  for (unsigned int i = 0; i < NPOINTS; ++i) {

    // one MCMC step
    prob = chain.move(x, prob);

    // convert dimensionless x to physical velocity
    double v = sqrt(2.0 * kT / m * x);

    mbVelocityHist.accumulate(v);
  }


  // theoretical distribution
  using namespace Genfun;

  Power p3by2(1.5);
  Exp exp;
  Variable V;

  GENFUNCTION MBDist = p3by2(m / 2.0 / M_PI / kT) * 4.0 * M_PI * V * V * exp(-m * V * V / 2.0 / kT)
                            * mbVelocityHist.binWidth() * NPOINTS;

  PlotFunction1D pMBDist{MBDist};
  PlotHist1D mbVelocityPlot{mbVelocityHist};

  view.add(&mbVelocityPlot);
  view.add(&pMBDist);


  // label plot
  PlotStream titleStream(view.titleTextEdit());
  titleStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "Maxwell-Boltzmann distribution from MCMC"
      << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "v"
      << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "# molecules"
      << PlotStream::EndP();

  // save
  window.show();
  app.exec();
  return 0;
}