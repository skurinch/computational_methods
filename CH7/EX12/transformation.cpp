
#include "QatDataAnalysis/Hist1D.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/MultipleViewWindow.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QKeySequence>
#include <cmath>
#include <iostream>
#include <random>
#include <string>

double analytic(double x) {
  const double xmin = 1.0;
  const double xmax = 10.0;

  if (x < xmin || x > xmax) return 0.0;

  const double N = 100000.0;
  const double binWidth = (xmax - xmin) / 50.0;

  return N * binWidth / (x * std::log(xmax / xmin));
}

int main(int argc, char **argv) {
    // parameters
    const double xmin = 1.0;
    const double xmax = 10.0;
    const int N = 100000;
    const int NBINS = 50;

    std::mt19937 engine;
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    // start plotting program
    QApplication app(argc, argv);
    MultipleViewWindow window;
    QToolBar *toolBar = window.addToolBar("Tools");
    QAction *quitAction = toolBar->addAction("Quit");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));

    // histogram of transformed uniform random variate
    Hist1D h("X", NBINS, xmin, xmax);

    for (int i = 0; i < N; i++) {
        double u = uniform(engine);
        double x = xmin * std::pow(xmax / xmin, u);
        h.accumulate(x);
    }

    // analytic form
    Genfun::GENFUNCTION f = Genfun::F1D(analytic);

    PlotHist1D histPlot = h;
    PlotFunction1D analyticPlot(f);
    PlotView view(histPlot.rectHint());
    window.add(&view, "Transformation Method");

    view.add(&histPlot);
    view.add(&analyticPlot);

    PlotStream titleStream(view.titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times")
                << PlotStream::Size(16)
                << "Transformation Method"
                << PlotStream::EndP();

    PlotStream xLabelStream(view.xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times")
                << PlotStream::Size(16)
                << "x"
                << PlotStream::EndP();

    PlotStream yLabelStream(view.yLabelTextEdit());
    yLabelStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Times")
                << PlotStream::Size(16)
                << "P(x)"
                << PlotStream::EndP();

    window.show();
    return app.exec();
}
