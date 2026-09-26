// A simple pendulum consisting of a point-like mass at the end of a massless string of length l has a period which
// can be expressed as T(theta_max) where theta_max is the amplitude of the motion. For small amplitudes
// T_0 = 2 PI * SQRT(l/g). For large amplitudes, T = T_0 f(theta_max). Integrate the equations of motion of the
// pendulum, in order to obtain f(theta_max) at the points theta_max = PI/12, PI/6, PI/3, 2PI/3, 5PI/6, 11PI/12...

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"
#include "QatGenericFunctions/QuadratureRule.h"
#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"
#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QTextEdit>
#include <QLabel>
#include <cmath>
#include <iostream>
#include <vector>

class PendulumFunction : public Genfun::AbsFunction
{
public:
    PendulumFunction(double thetaMax): thetaMax_(thetaMax) {}

    virtual double operator()(double phi) const
    {
        double k = std::sin(thetaMax_ / 2.0);

        return 1.0 / std::sqrt( 1.0 - k * k * std::sin(phi) * std::sin(phi));
    }

    virtual double operator()(const Genfun::Argument &phi) const
    {
        return (*this)(phi[0]);
    }

    virtual PendulumFunction *clone() const
    {
        return new PendulumFunction(*this);
    }

private:
    double thetaMax_;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    using namespace Genfun;

    // maximum amplitudes
    std::vector<double> thetaMax = {M_PI / 12.0, M_PI / 6.0, M_PI / 3.0, 2.0 * M_PI / 3.0,
        5.0 * M_PI / 6.0, 11.0 * M_PI / 12.0
    };

    // quadrature rule
    SimpsonsRule simpsonRule;

    // calculate f(theta_max)
    for (size_t i = 0; i < thetaMax.size(); ++i)
    {
        PendulumFunction f(thetaMax[i]);

        SimpleIntegrator simpson(0.0, M_PI / 2.0, simpsonRule,1024);

        double integral = simpson(f);
        double periodRatio = (2.0 / M_PI) * integral;

    }

    // set up the plot
    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::black);
    app.setPalette(palette);

    // plot range
    PRectF rect;
    rect.setXmin(0.0);
    rect.setXmax(M_PI / 2.0);
    rect.setYmin(0.9);
    rect.setYmax(5.0);

    QMainWindow window;
    PlotView view(rect);
    view.setBackgroundBrush(Qt::white);
    view.xLabelTextEdit()->setText("phi");
    view.xLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.xLabelTextEdit()->setTextColor(Qt::black);
    view.yLabelTextEdit()->setText("f");
    view.yLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.yLabelTextEdit()->setTextColor(Qt::black);
    window.setCentralWidget(&view);

    // colors
    std::vector<QColor> colors = {
        Qt::red,
        Qt::blue,
        Qt::green,
        Qt::magenta,
        Qt::cyan,
        Qt::black
    };

    // legend
    QLabel *legend = new QLabel(&view);

    legend->setText(
        "<span style='color:red;'> PI/12</span><br>"
        "<span style='color:blue;'>PI/6</span><br>"
        "<span style='color:green;'>PI/3</span><br>"
        "<span style='color:magenta;'>PI/2</span><br>"
        "<span style='color:cyan;'>2PI/3</span><br>"
        "<span style='color:black;'>5PI/6</span>"
    );

    legend->adjustSize();
    legend->move(110, 110);
    legend->show();

    // plot each theta_max
    for (size_t j = 0; j < thetaMax.size(); ++j)
    {
        PendulumFunction f(thetaMax[j]);

        PlotProfile *profile = new PlotProfile();
        PlotProfile::Properties properties;
        properties.pen.setColor(colors[j]);
        properties.pen.setWidth(2);
        properties.brush.setColor(colors[j]);
        properties.drawSymbol = true;
        properties.symbolSize = 5;

        profile->setProperties(properties);


        // sample the function
        for (int i = 0; i <= 200; ++i)
        {
            double phi = (M_PI / 2.0) * i / 200.0;

            profile->addPoint(phi, f(phi));
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
    image.save("pendulum.jpg", "JPG");

    return app.exec();
}