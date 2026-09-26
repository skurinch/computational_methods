// Compute the integral of tanh(x) from 0 to 1 using the rectangle rule, the trapezoidal rule, and Simpson's rule.
// For each, perform the calculation in two, four siz, eight, ... intervals and plot the difference between the
// approximate result and the true result on a logarithmic scale versus the mesh spacing 1/n.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"
#include "QatGenericFunctions/QuadratureRule.h"
#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/Tanh.h"
#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QTextEdit>
#include <QLabel>
#include <cmath>
#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    using namespace Genfun;

    // function
    GENFUNCTION f = Tanh();

    // exact solution: integral_0^1 tanh(x) dx = log(cosh(1))
    const double exact = std::log(std::cosh(1.0));

    // quadrature rules
    MidpointRule   midpointRule;
    TrapezoidRule  trapezoidRule;
    SimpsonsRule   simpsonRule;

    // data for the plot
    std::vector<double> x;
    std::vector<double> midpointError;
    std::vector<double> trapezoidError;
    std::vector<double> simpsonError;

    // n = 2, 4, 8, ..., 1024
    for (int n = 2; n <= 1024; n *= 2)
    {
        SimpleIntegrator midpoint(0, 1, midpointRule, n);
        SimpleIntegrator trapezoid(0, 1, trapezoidRule, n);
        SimpleIntegrator simpson(0, 1, simpsonRule, n);

        double errorMidpoint =
            std::abs(midpoint(f) - exact);

        double errorTrapezoid =
            std::abs(trapezoid(f) - exact);

        double errorSimpson =
            std::abs(simpson(f) - exact);

        // Log-log coordinates
        x.push_back(std::log10(1.0 / n));

        midpointError.push_back(std::log10(errorMidpoint));
        trapezoidError.push_back(std::log10(errorTrapezoid));
        simpsonError.push_back(std::log10(errorSimpson));

    }

    // Plot range
    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::black);
    app.setPalette(palette);

    PRectF rect;
    rect.setXmin(std::log10(1.0 / 1024.0));
    rect.setXmax(std::log10(1.0 / 2.0));
    rect.setYmin(-14);
    rect.setYmax(-1);

    QMainWindow window;
    PlotView view(rect);

    view.setBackgroundBrush(Qt::white);

    view.xLabelTextEdit()->setText("log 1/n");
    view.xLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.xLabelTextEdit()->setTextColor(Qt::black);

    view.yLabelTextEdit()->setText("log error");
    view.yLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.yLabelTextEdit()->setTextColor(Qt::black);

    window.setCentralWidget(&view);

    // legend
    QLabel *legend = new QLabel(&view);

    legend->setText(
        "<span style='color:red;'>  Midpoint   </span>"
        "<span style='color:blue;'> Trapezoid   </span>"
        "<span style='color:black;'>Simpson</span>"
    );

    legend->adjustSize();
    legend->move(110, 110);
    legend->show();

    // colors
    std::vector<QColor> colors = {
        Qt::red,
        Qt::blue,
        Qt::black
    };

    // errors
    std::vector<std::vector<double>> errors = {
        midpointError,
        trapezoidError,
        simpsonError
    };

    // plot each method
    for (size_t j = 0; j < errors.size(); ++j)
    {
        PlotProfile *profile = new PlotProfile();

        PlotProfile::Properties properties;
        properties.pen.setColor(colors[j]);
        properties.pen.setWidth(2);
        properties.brush.setColor(colors[j]);
        properties.drawSymbol = true;
        properties.symbolSize = 7;

        profile->setProperties(properties);

        for (size_t i = 0; i < x.size(); ++i)
        {
            profile->addPoint(x[i], errors[j][i]);
        }

        view.add(profile);
    }

    window.resize(800, 600);
    window.show();

    // Save plot
    QImage image(view.size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    view.render(&painter);
    painter.end();

    image.save("integral-1.jpg", "JPG");

    return app.exec();
}