// Use the Newton-Raphson method to find the zeros of the Legendre Polynomials, from l = 1 to 10 (command line).
// Plot the Legendre polynomials, and mark the zeros on the plot. Print out the zeroes to std::cout.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/LegendrePolynomial.h"
#include "QatPlotting/PlotPoint.h"

#include <QApplication>
#include <QMainWindow>
#include <QBrush>
#include <QImage>
#include <QPainter>
#include <QString>

#include <iostream>
#include <cmath>

double newton_raphson(const Genfun::LegendrePolynomial &P, double x)
{
    double dx = 1e-6;

    for (int i = 0; i < 100; i++) {
        double f = P(x);
        double fp = (P(x + dx) - P(x - dx)) / (2.0 * dx);

        double xnew = x - f / fp;

        if (std::abs(xnew - x) < 1e-10) {
            return xnew;
        }

        x = xnew;
    }

    return x;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <l>" << std::endl;
        return 1;
    }

    int l = std::stoi(argv[1]);

    Genfun::LegendrePolynomial P(l);

    // Find the l zeros.
    for (int i = 0; i < l; i++) {

        // Initial guesses spread across [-1,1].
        double x = (1.0 - 1.0/(8.0*l*l) + 1.0/(8.0*l*l*l)) * std::cos(M_PI * (4*i - 1) / (4*l + 2));
        double zero = newton_raphson(P, x);

        std::cout << "zero " << i + 1 << " = " << zero << std::endl;
    }

    PRectF rect;
    rect.setXmin(-1.0);
    rect.setXmax(1.0);
    rect.setYmin(-1.1);
    rect.setYmax(1.1);

    PlotView *plotView = new PlotView(rect);

    PlotFunction1D *plot = new PlotFunction1D(P);

    plotView->add(plot);
    plotView->setBackgroundBrush(QBrush(Qt::white));

    for (int i = 0; i < l; i++) {
        double x = std::cos(M_PI * (i + 0.5) / l);
        double zero = newton_raphson(P, x);

        PlotPoint *point = new PlotPoint(zero, 0.0);
        plotView->add(point);
    }

    QMainWindow window;
    window.setCentralWidget(plotView);
    window.resize(800, 600);
    window.show();

    // save
    QString filename = QString("root_l%1.jpg").arg(l);

    QImage image(plotView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    plotView->render(&painter);
    painter.end();

    image.save(filename, "JPG");

    return app.exec();
}