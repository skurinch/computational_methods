// Solve the Schrodinger equation for V/E=1.5 and a given value of ka, and plot.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"

#include <QApplication>
#include <QMainWindow>
#include <QBrush>
#include <QImage>
#include <QPainter>
#include <QString>

#include <iostream>
#include <cmath>
#include <complex>
#include <iomanip>

// Define parameters.
double VoverE = 1.5;
double ka;


// Solve Schrodinger equation like in EX7.
double schrodinger(double x)
{
    double k = ka;
    std::complex<double> I(0.0, 1.0);

    std::complex<double> q = k * std::sqrt(std::complex<double>(1.0 - VoverE, 0.0));

    std::complex<double> a = std::cos(q);
    std::complex<double> b = std::sin(q) / q;
    std::complex<double> c = -q * std::sin(q);
    std::complex<double> d = std::cos(q);

    double q2 = k * std::sqrt(std::abs(1.0 - 2.0 * VoverE));

    std::complex<double> e, f, g, h;

    if (2.0 * VoverE < 1.0) {
        e = std::cos(2.0 * q2);
        f = std::sin(2.0 * q2) / q2;
        g = -q2 * std::sin(2.0 * q2);
        h = std::cos(2.0 * q2);
    }
    else {
        e = std::cosh(2.0 * q2);
        f = std::sinh(2.0 * q2) / q2;
        g = q2 * std::sinh(2.0 * q2);
        h = std::cosh(2.0 * q2);
    }

    std::complex<double> xx = e * a + f * c;
    std::complex<double> y  = e * b + f * d;
    std::complex<double> z  = g * a + h * c;
    std::complex<double> w  = g * b + h * d;

    std::complex<double> M11 = a * xx + b * z;
    std::complex<double> M12 = a * y  + b * w;
    std::complex<double> M21 = c * xx + d * z;
    std::complex<double> M22 = c * y  + d * w;

    std::complex<double> A = M11 + I * k * M12;
    std::complex<double> B = M21 + I * k * M22;

    std::complex<double> r =
        -(B - I * k * A) / (B + I * k * A);

    // Incoming + reflected wave
    std::complex<double> wave =
        std::exp(I * k * x) +
        r * std::exp(-I * k * x);

    return std::real(wave);
}

// Main function to plot.
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ka>" << std::endl;
        return 1;
    }

    ka = std::stod(argv[1]);

    std::cout << "ka = " << ka << std::endl;

    for (double x = -10.0; x <= 10.0; x += 1.0) {
        std::cout << x << "  " << schrodinger(x) << std::endl;
    }

    PRectF rect;

    rect.setXmin(-10.0);
    rect.setXmax(10.0);
    rect.setYmin(-2.0);
    rect.setYmax(2.0);

    PlotView *plotView = new PlotView(rect);

    Genfun::F1D psi(schrodinger);

    PlotFunction1D *plot = new PlotFunction1D(psi);

    plotView->add(plot);
    plotView->setBackgroundBrush(QBrush(Qt::white));

    QMainWindow window;
    window.setCentralWidget(plotView);
    window.resize(800, 600);
    window.show();

    // save
    QString filename = QString("psi_ka%1.jpg").arg(ka);

    QImage image(plotView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    plotView->render(&painter);
    painter.end();

    image.save(filename, "JPG");

    return app.exec();
}