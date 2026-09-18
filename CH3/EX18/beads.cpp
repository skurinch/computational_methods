// Consider a system of 100 beads on a string connected by springs. Each bead has the same mass m and each spring
// has the same spring constant k. The first and last beads are attached to fixed points by means of another spring
// of constant k. Determine the eigenmodes of this system. For the first five eigenmodes, plot the ith element of
// the eigenmode as a function of the index i.

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"

#include <QApplication>
#include <QMainWindow>
#include <QBrush>
#include <QImage>
#include <QPainter>
#include <QString>

#include <Eigen/Dense>
#include <iostream>
#include <cmath>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    int N    = 100;
    double m = 1.0;
    double k = 1.0;

    // Construct the matrix K/m.
    Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(N, N);

    for (int i = 0; i < N; i++) {
        matrix(i, i) = 2.0 * k / m;

        if (i > 0) {
            matrix(i, i - 1) = -k / m;
        }

        if (i < N - 1) {
            matrix(i, i + 1) = -k / m;
        }
    }

    // Find the eigenvalues and eigenvectors.
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(matrix);
    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();

    // Print the first five eigenvalues.
    for (int mode = 0; mode < 5; mode++) {
        std::cout << "Mode " << mode + 1
                  << ": omega^2 = "
                  << eigenvalues(mode)
                  << std::endl;
    }

    // Plot the first five eigenmodes.
    for (int mode = 0; mode < 5; mode++) {

        PRectF rect;
        rect.setXmin(1.0);
        rect.setXmax(100.0);
        rect.setYmin(-0.2);
        rect.setYmax(0.2);

        PlotView *plotView = new PlotView(rect);

        PlotProfile *profile = new PlotProfile();

        for (int i = 0; i < N; i++) {
            profile->addPoint(i + 1, eigenvectors(i, mode));
        }

        plotView->add(profile);
        plotView->setBackgroundBrush(QBrush(Qt::white));

        QMainWindow *window = new QMainWindow();
        window->setCentralWidget(plotView);
        window->resize(800, 600);
        window->show();

        // save
        QString filename = QString("mode_%1.jpg").arg(mode + 1);

        QImage image(plotView->size(), QImage::Format_RGB32);
        image.fill(Qt::white);

        QPainter painter(&image);
        plotView->render(&painter);
        painter.end();

        image.save(filename, "JPG");
    }

    return app.exec();
}