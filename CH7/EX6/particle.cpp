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

// generate the angular distribution using the MCMC
class AngularMarkovChain {
public:
  AngularMarkovChain(double sigma = 0.2)
    : gauss(0.0, sigma) {}

  double probability(double mu) {
    if (mu < -1.0 || mu > 1.0) return 0.0;

    double P1 = mu;
    double P3 = 0.5 * (5.0 * pow(mu, 3) - 3.0 * mu);
    double P5 = 0.125 * (63.0 * pow(mu, 5) - 70.0 * pow(mu, 3) + 15.0 * mu);
    double P7 = (1.0 / 16.0) * (429.0 * pow(mu, 7) - 693.0 * pow(mu, 5) + 315.0 * pow(mu, 3) - 35.0 * mu);

    double amplitude = sqrt(3.0 / 8.0)  * P1 - sqrt(7.0 / 6.0)  * P3 + sqrt(11.0 / 24.0) * P5
                           - sqrt(15.0 / 6.0)  * P7;

    return amplitude * amplitude;
  }

  double move(double &mu, double prob = 0.0) {
    double muProp = mu + gauss(engine);

    if (muProp < -1.0 || muProp > 1.0)
      return prob;

    double newProb = probability(muProp);

    if (newProb > prob) {
      mu = muProp;
      return newProb;
    }

    if (flat(engine) < newProb / prob) {
      mu = muProp;
      return newProb;
    }

    return prob;
  }

private:
  std::mt19937 engine;
  std::uniform_real_distribution<double> flat{0.0, 1.0};
  std::normal_distribution<double> gauss;
};


int main(int argc, char **argv) {

  std::random_device dev;
  std::mt19937 engine(dev());

  // parameters
  constexpr unsigned int NPOINTS{10000};

  // start plotting application
  QApplication app(argc, argv);
  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
  PlotView view({-1.0, 1.0, 0, 400.0});
  window.setCentralWidget(&view);

  // plot histogram
  Hist1D angularHist{"cos(x)", 100, -1.0, 1.0};;

  // generate distribution using MCMC
  AngularMarkovChain chain(0.2);

  double mu = 0.0;
  double prob = chain.probability(mu);

  for (unsigned int i = 0; i < NPOINTS; ++i) {
    prob = chain.move(mu, prob);
    angularHist.accumulate(mu);
  }


  // theoretical distribution
  using namespace Genfun;
  Variable X;

  GENFUNCTION P1 = X;
  GENFUNCTION P3 = 0.5 * (5.0 * X * X * X - 3.0 * X);
  GENFUNCTION P5 = 0.125 * (63.0 * X * X * X * X * X - 70.0 * X * X * X + 15.0 * X);
  GENFUNCTION P7 = (1.0 / 16.0) * (429.0 * X * X * X * X * X * X * X - 693.0 * X * X * X * X * X
                                + 315.0 * X * X * X - 35.0 * X);

  GENFUNCTION amplitude = sqrt(3.0 / 8.0)  * P1 - sqrt(7.0 / 6.0)  * P3 + sqrt(11.0 / 24.0) * P5
                                - sqrt(15.0 / 6.0)  * P7;

  GENFUNCTION angularDist = amplitude * amplitude * angularHist.binWidth() * NPOINTS;

  PlotHist1D angularPlot{angularHist};
  PlotFunction1D angularFunction{angularDist};
  view.add(&angularPlot);
  view.add(&angularFunction); 

  // label plot
  PlotStream titleStream(view.titleTextEdit());
  titleStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "Angular Distribution from MCMC"
      << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "cos(x)"
      << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream
      << PlotStream::Clear()
      << PlotStream::Center()
      << PlotStream::Family("Times")
      << PlotStream::Size(16)
      << "# particles"
      << PlotStream::EndP();

  // save
  window.show();
  app.exec();
  return 0;
}