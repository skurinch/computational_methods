// There are four types of photoreceptors in the human retina, called rods, and three types of cones called red,
// green, and blue cones. The rods have a response peaking at 498 nm, the red, green, and blue cones have responses
// peaking at 564, 534, and 420 nm, respectively. Color perception is carries out by the rods.

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
#include <memory>
#include "QatGenericFunctions/InterpolatingFunction.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::black);
    app.setPalette(palette);

    // x
    std::vector<std::vector<double>> x = {
        {370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520},
        {400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550,
            560, 570, 580, 590},
        {400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550,
            560, 570, 580, 590, 600, 610, 620, 630, 640},
        {400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550,
            560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680}
    };

    // y
    std::vector<std::vector<double>> y = {
        {58.7, 67.3, 78.3, 88.9, 96.6, 98.9, 95.7, 86.2, 71, 52.1, 35.9, 26.8, 19.2, 14.3, 9, 5.1},
        {33.9, 35.2, 36.1, 40.5, 48.8, 59.6, 70.6, 81.4, 91.3, 96.8, 98.6, 96.5, 90.4, 82.6, 60.6,
            43.3, 29.3, 19.3, 13.2, 8.6},
        {36.1, 36.1, 34.1, 34.1, 35, 38.3, 42, 37.2, 50.4, 67, 77.6, 87.4, 95.9, 98.3, 97.4, 92.6,
            82.8, 71.8, 61.7, 52.8, 41.8, 30.8, 22.1, 14.3, 8.3},
        {37.4, 34.8, 34.1, 31.3, 30, 28.5, 31.1, 34.8, 46.9, 55.7, 64.4, 73.4, 83.3, 90.9, 99.3,
            99.8, 99.9, 98.4, 93.4, 86.5, 76.9, 65.4, 52.3, 38.7, 26.2, 16.7, 11, 8.8, 7.2}
    };

    // colors
    std::vector<QColor> colors = {
        Qt::black,
        Qt::red,
        Qt::blue,
        Qt::green
    };

    // plot range
    PRectF rect;
    rect.setXmin(400);
    rect.setXmax(700);
    rect.setYmin(-5);
    rect.setYmax(105);

    QMainWindow window;
    PlotView view(rect);
    view.setBackgroundBrush(Qt::white);
    view.xLabelTextEdit()->setText("wavelength (nm)");
    view.xLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.yLabelTextEdit()->setText("normalized absorbance");
    view.yLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.xLabelTextEdit()->setTextColor(Qt::black);
    view.yLabelTextEdit()->setTextColor(Qt::black);
    window.setCentralWidget(&view);

    std::vector<std::unique_ptr<Genfun::InterpolatingFunction>> interpolators;
    std::vector<std::unique_ptr<PlotProfile>> dataPoints;
    std::vector<std::unique_ptr<PlotFunction1D>> interpolations;
    for (size_t j = 0; j < x.size(); ++j)
    {
        // interpolate
        interpolators.push_back(
            std::make_unique<Genfun::InterpolatingFunction>(Genfun::CUBIC_SPLINE)
        );

        for (size_t i = 0; i < x[j].size(); ++i)
        {
            interpolators[j]->addPoint(x[j][i], y[j][i]);
        }

        // input data
        dataPoints.push_back(std::make_unique<PlotProfile>());

        PlotProfile::Properties dataProperties;
        dataProperties.pen.setColor(colors[j]);
        dataProperties.pen.setWidth(1);
        dataProperties.brush.setColor(colors[j]);
        dataProperties.drawSymbol = true;
        dataProperties.symbolSize = 6;

        dataPoints[j]->setProperties(dataProperties);

        for (size_t i = 0; i < x[j].size(); ++i)
        {
            dataPoints[j]->addPoint(x[j][i], y[j][i]);
        }

        view.add(dataPoints[j].get());

        // interpolation
        interpolations.push_back(
            std::make_unique<PlotFunction1D>(*interpolators[j], rect)
        );

        PlotFunction1D::Properties interpolationProperties;
        interpolationProperties.pen.setColor(colors[j]);
        interpolationProperties.pen.setWidth(2);

        interpolations[j]->setProperties(interpolationProperties);

        view.add(interpolations[j].get());
    }

    window.resize(800, 600);
    window.show();

    // save
    QImage image(view.size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    view.render(&painter);
    painter.end();

    image.save("photoreceptors.jpg", "JPG");


    return app.exec();
}