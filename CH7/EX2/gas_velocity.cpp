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
#include <numbers>
#include <cmath>

int main(int argc, char **argv) {

  // Random number generator
  std::random_device dev;
  std::mt19937 engine(dev());

  std::string usage = std::string("usage: ") + argv[0];

  if (argc != 1) {
    std::cout << usage << std::endl;
    exit(0);
  }

  // parameteres
  constexpr unsigned int N{10000};
  constexpr double E{3 * 100000};
  constexpr double m{1.0};
  constexpr double kT{2.0 / 3.0 * E / N};

  // start the plotting application
  QApplication app(argc, argv);
  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
  PlotView view({0, 20, 0, 400.0});
  window.setCentralWidget(&view);

  // histogram of speeds
  Hist1D mbVelocityHist{"Velocities", 100, 0, 20.0};

  // sample the gamma distribution
  std::gamma_distribution<double> gamma(1.5, 1.0);

  for (unsigned int i = 0; i < N; ++i) {

    // dimensionless variable
    double x = gamma(engine);

    // convert to physical speed
    double v = sqrt(2.0 * kT / m * x);

    mbVelocityHist.accumulate(v);
  }

  // plot the distribution
  using namespace Genfun;

  Power p3by2(1.5);
  Exp exp;
  Variable V;

  GENFUNCTION MBDist = p3by2(m / 2.0 / M_PI / kT) * 4.0 * M_PI * V * V * exp(-m * V * V / 2.0 / kT)
                              * mbVelocityHist.binWidth() * N;

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
      << "Gamma Distribution"
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