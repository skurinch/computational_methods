// Make a program to plot the following seven functions:
// 1. sin(x)           5. Legendre polynomial P_3^2(x)
// 2. sin(5x)          6. any other function
// 3. sin(x^2)         7. the derivative of that function
// 4. e^-x sin(x)
// The program should take a command-line argument from 1-7. Print and save all plots from the program.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"

#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/AssociatedLegendre.h"
#include "QatGenericFunctions/FunctionNumDeriv.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QPixmap>
#include <QPalette>
#include <QTimer>

#include <cstdlib>
#include <iostream>
#include <memory>

// my "other" function: x^2 e^{−x^2}
#include <cmath>
#include "QatGenericFunctions/F1D.h"
double myFunction(double x) {
    return x * x * std::exp(-x * x);
}


int main(int argc, char **argv) {

  int choice = std::atoi(argv[1]);

  if (choice < 1 || choice > 7) {
    std::cout << "Please enter a number from 1 to 7." << std::endl;
    return 1;
  }


  // start QAT application
  QApplication app(argc, argv);
  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));

  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);


  // set the scale for the plot
  PRectF rect;
  if (choice == 5) {          // legendre polynomial needs a separate range
    rect.setXmin(-1.0);
    rect.setXmax(1.0);
    rect.setYmin(-10.0);
    rect.setYmax(10.0);
  }
  else {
      rect.setXmin(-5.0);
      rect.setXmax(5.0);
      rect.setYmin(-2.0);
      rect.setYmax(2.0);
  }


  // create the plot
  PlotView view(rect);
  window.setCentralWidget(&view);

  QPalette palette = view.palette();

  palette.setColor(QPalette::Window, Qt::white);
  palette.setColor(QPalette::Base, Qt::white);
  palette.setColor(QPalette::Text, Qt::black);
  palette.setColor(QPalette::WindowText, Qt::black);
  palette.setColor(QPalette::ButtonText, Qt::black);

  view.setPalette(palette);
  view.setAutoFillBackground(true);

  view.setStyleSheet(
      "QWidget {"
      "background-color: white;"
      "color: black;"
      "}"
  );

  // Define x
  Genfun::Variable x;

  // 1. sin(x)
  PlotFunction1D pSin = Genfun::Sin();
  // 2. sin(5x)
  PlotFunction1D pSin5 = Genfun::Sin()(5.0 * x);
  // 3. sin(x^2)
  PlotFunction1D pSinXSquared = Genfun::Sin()(Genfun::Power(2)(x));
  // 4. e^(-x) sin(x)
  PlotFunction1D pExpSin = Genfun::Exp()(-x) * Genfun::Sin();
  // 5. Associated Legendre polynomial P_3^2(x)
  PlotFunction1D pLegendre = Genfun::AssociatedLegendre(3, 2);
  // 6. other function
  Genfun::F1D f1D(myFunction);
  PlotFunction1D pOther = f1D;
  // 7. derivative of other function
  std::shared_ptr<const Genfun::AbsFunction> f = std::make_shared<Genfun::F1D>(f1D);
  Genfun::FunctionNumDeriv derivative(f);
  PlotFunction1D pDerivative = derivative;


  // choose function to plot
  if (choice == 1) {
    view.add(&pSin);
  }

  else if (choice == 2) {
    view.add(&pSin5);
  }

  else if (choice == 3) {
    view.add(&pSinXSquared);
  }

  else if (choice == 4) {
    view.add(&pExpSin);
  }

  else if (choice == 5) {
    view.add(&pLegendre);
  }

  else if (choice == 6) {
    view.add(&pOther);
  }

  else if (choice == 7) {
    view.add(&pDerivative);
  }


  // label plot
  PlotStream titleStream(view.titleTextEdit());

  titleStream << PlotStream::Clear()
              << PlotStream::Center()
              << PlotStream::Family("Sans Serif")
              << PlotStream::Size(16);

  if (choice == 1) {
    titleStream << "sin(x)";
  }

  else if (choice == 2) {
    titleStream << "sin(5x)";
  }

  else if (choice == 3) {
    titleStream << "sin(x^2)";
  }

  else if (choice == 4) {
    titleStream << "exp(-x) sin(x)";
  }

  else if (choice == 5) {
    titleStream << "Associated Legendre P_3^2(x)";
  }

  else if (choice == 6) {
    titleStream << "cos(x)";
  }

  else if (choice == 7) {
    titleStream << "Derivative of cos(x)";
  }

  titleStream << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());

  xLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Sans Serif")
               << PlotStream::Size(16)
               << "x"
               << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());

  yLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Sans Serif")
               << PlotStream::Size(16)
               << "y"
               << PlotStream::EndP();


  // show the window and save the plot
  window.show();

  QTimer::singleShot(500, [&view, choice]() 
  {
    QString filename = QString("plot%1.jpg").arg(choice);
    view.grab().save(filename, "JPG");
  });

  app.exec();

  return 0;
}