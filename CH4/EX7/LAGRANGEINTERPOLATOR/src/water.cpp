// Data on the dispersion of light in water is given in Table 4.3. Interpolate a smooth function for these data.
// Plot the function together with the input data.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/AbsFunction.h"
#include <QApplication>
#include <QMainWindow>
#include <QPalette>
#include <Qt>
#include <QTextEdit>
#include <QImage>
#include <QPainter>
#include <iostream>
#include <vector>
#include "Interpolator.h"


class InterpolatedFunction : public Genfun::AbsFunction
{
public:

    InterpolatedFunction(const Interpolator<double> &interpolator)
        : interpolator_(interpolator)
    {
    }

    virtual double operator()(double x) const
    {
        return interpolator_(x);
    }

    virtual double operator()(const Genfun::Argument &argument) const
    {
        return interpolator_(argument[0]);
    }

    virtual InterpolatedFunction *clone() const
    {
        return new InterpolatedFunction(*this);
    }

private:
    const Interpolator<double> &interpolator_;
};


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::black);
    app.setPalette(palette);

    // wavelength (nm)
    std::vector<double> x = {
        200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000
    };

    // index of refraction
    std::vector<double> y = {
        1.396, 1.362, 1.349, 1.343, 1.339, 1.337, 1.335, 1.333, 1.332, 1.331, 1.331, 1.330,
        1.329, 1.329, 1.328, 1.327, 1.327
    };


    // interpolate the data
    Interpolator<double> interpolator;

    for (size_t i = 0; i < x.size(); ++i)
    {
        interpolator.addPoint(x[i], y[i]);
    }

    std::cout << "Interpolation done" << std::endl;


    // plot range
    PRectF rect;
    rect.setXmin(200);
    rect.setXmax(1000);
    rect.setYmin(1.3);
    rect.setYmax(1.4);

    QMainWindow window;
    PlotView view(rect);
    view.setBackgroundBrush(Qt::white);
    view.xLabelTextEdit()->setText("wavelength (nm)");
    view.xLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.yLabelTextEdit()->setText("index of refraction");
    view.yLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.xLabelTextEdit()->setTextColor(Qt::black);
    view.yLabelTextEdit()->setTextColor(Qt::black);
    window.setCentralWidget(&view);

    // Plot the input data
    PlotProfile dataPoints;

    PlotProfile::Properties dataProperties;
    dataProperties.pen.setColor(Qt::black);
    dataProperties.pen.setWidth(1);
    dataProperties.brush.setColor(Qt::black);
    dataProperties.drawSymbol = true;
    dataProperties.symbolSize = 6;

    dataPoints.setProperties(dataProperties);

    for (size_t i = 0; i < x.size(); ++i)
    {
        dataPoints.addPoint(x[i], y[i]);
    }

    view.add(&dataPoints);


    // Turn the interpolator into a QAT function
    InterpolatedFunction function(interpolator);

    PlotFunction1D interpolation(function, rect);

    PlotFunction1D::Properties interpolationProperties;
    interpolationProperties.pen.setColor(Qt::black);
    interpolationProperties.pen.setWidth(1);

    interpolation.setProperties(interpolationProperties);

    view.add(&interpolation);


    window.resize(800, 600);
    window.show();

    // save
    QImage image(view.size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    view.render(&painter);
    painter.end();

    image.save("../../water_dispersion.jpg", "JPG");


    return app.exec();
}