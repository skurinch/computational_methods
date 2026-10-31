// Shalini Kurinchi-Vendhan
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"
#include "Interpolator.h"
#include <QApplication>
#include <QMainWindow>
#include <QPalette>
#include <QColor>
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QTextEdit>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <mpi.h>
#include <cstdint>

// Define 3D vector of photons.
struct Vector {
    double x, y, z;

    Vector add(const Vector& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    Vector subtract(const Vector& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    Vector mulitply(double a) const {
        return {a*x, a*y, a*z};
    }

    double dot(const Vector& b) const {
        return x*b.x + y*b.y + z*b.z;
    }

    double norm() const {
        return std::sqrt(dot(*this));
    }

    Vector unit() const {
        return (*this)*(1.0/norm());
    }
};

struct Photon {
    double wavelength;
    Vector outgoing;
};

// Refraction
Vector refract(const Vector& I, const Vector& N, double eta) {
    double cosi = -I.dot(N);
    double k = 1.0 - eta*eta*(1.0 - cosi*cosi);

    if (k < 0.0) {
        return {0.0, 0.0, 0.0};
    }

    return (I*eta + N*(eta*cosi - std::sqrt(k))).unit();
}

// Reflection
Vector reflect(const Vector& I, const Vector& N) {
    return (I - N*(2.0*I.dot(N))).unit();
}

// Angle between vectors.
double angleDegrees(const Vector& a, const Vector& b) {
    double cosine = a.dot(b)/(a.norm()*b.norm());
    cosine = std::max(-1.0, std::min(1.0, cosine));
    return std::acos(cosine)*180.0/M_PI;
}

// Trace one photon through a spherical water droplet.
Vector traceRay(double wavelength, double y, double z,
              const Interpolator<double>& refractiveIndex) {

    const double R = 1.0;
    const double n = refractiveIndex(wavelength);

    // Incident beam travels along +x axis.
    const Vector incident = {1.0, 0.0, 0.0};

    const double x = -std::sqrt(R*R - y*y - z*z);
    Vector entry = {x, y, z};
    Vector normal = entry.unit();

    // From air to water.
    Vector inside = refract(incident, normal, 1.0/n);

    // Exits the water.
    double t = -2.0*entry.dot(inside);
    Vector exit = entry + inside*t;

    // Internal reflection.
    normal = exit.unit();
    Vector reflected = reflect(inside, normal);

    // Intersection with the surface after reflection.
    t = -2.0*exit.dot(reflected);
    Vector exitPoint = exit + reflected*t;

    // From water to air.
    normal = exitPoint.unit()*(-1.0);
    Vector outgoing = refract(reflected, normal, n);

    return outgoing;
}

// Generate photons using parallel processing.
const int NWAVELENGTH = 300;
const int NANGLE = 200;
const int NBINS = NWAVELENGTH * NANGLE;

class PhotonGenerator {

public:

    PhotonGenerator(int seed)
        : engine(seed),
          wavelengthDist(400.0, 700.0),
          positionDist(-1.0, 1.0),
          histogram(NBINS, 0)
    {
    }

    void compute(long long n,
                 const Interpolator<double>& refractiveIndex,
                 bool keepSample = false,
                 int maxSample = 100000)
    {
        for (long long i = 0; i < n; ++i) {

            double wavelength = wavelengthDist(engine);

            // Random impact position on the projected disk.
            double y, z;

            do {
                y = positionDist(engine);
                z = positionDist(engine);
            } while (y*y + z*z > 1.0);

            Vector outgoing = traceRay(
                wavelength, y, z, refractiveIndex
            );

            if (outgoing.norm() == 0.0) {
                continue;
            }

            double angle = angleDegrees(
                Vector{1.0, 0.0, 0.0}, outgoing
            );

            // Accumulate wavelength-angle histogram.
            if (angle >= 130.0 && angle <= 150.0) {

                int iw = static_cast<int>(
                    (wavelength - 400.0) / 300.0 * NWAVELENGTH
                );

                int ia = static_cast<int>(
                    (angle - 130.0) / 20.0 * NANGLE
                );

                iw = std::min(NWAVELENGTH - 1, iw);
                ia = std::min(NANGLE - 1, ia);

                histogram[iw * NANGLE + ia]++;
            }

            // Retain a small sample for visualization.
            if (keepSample &&
                static_cast<int>(sample.size()) < maxSample) {

                sample.push_back({wavelength, outgoing});
            }
        }
    }

    const std::vector<unsigned long long>& harvestHistogram() const {
        return histogram;
    }

    const std::vector<Photon>& harvestSample() const {
        return sample;
    }

private:

    std::mt19937 engine;

    std::uniform_real_distribution<double> wavelengthDist;
    std::uniform_real_distribution<double> positionDist;

    std::vector<unsigned long long> histogram;
    std::vector<Photon> sample;
};

// Convert wavelength to visible RGB color.
QColor wavelengthColor(double wavelength) {
    double r = 0.0, g = 0.0, b = 0.0;
    double w = wavelength;

    if (w < 440.0) {
        r = (440.0 - w)/60.0;
        b = 1.0;
    } else if (w < 490.0) {
        g = (w - 440.0)/50.0;
        b = 1.0;
    } else if (w < 510.0) {
        g = 1.0;
        b = (510.0 - w)/20.0;
    } else if (w < 580.0) {
        r = (w - 510.0)/70.0;
        g = 1.0;
    } else if (w < 645.0) {
        r = 1.0;
        g = (645.0 - w)/65.0;
    } else {
        r = 1.0;
    }

    double factor = 1.0;

    if (w < 420.0) {
        factor = 0.3 + 0.7*(w - 400.0)/20.0;
    } else if (w > 680.0) {
        factor = 0.3 + 0.7*(700.0 - w)/20.0;
    }

    return QColor(
        int(255.0*r*factor),
        int(255.0*g*factor),
        int(255.0*b*factor)
    );
}

// Make rainbow image.
class RainbowWidget : public QWidget {
public:
    RainbowWidget(const std::vector<Photon>& photons,
                  QWidget *parent = nullptr)
        : QWidget(parent), photons_(photons) {
        resize(500, 400);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);

        const double left = 85;
        const double right = width() - 30;
        const double top = 40;
        const double bottom = height() - 75;

        // Plot axes.
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawLine(QPointF(left, bottom), QPointF(right, bottom));
        painter.drawLine(QPointF(left, bottom), QPointF(left, top));

        auto xPixel = [&](double wavelength) {
            return left + (wavelength - 400.0) / 300.0 * (right - left);
        };

        auto yPixel = [&](double angle) {
            return bottom - (angle - 130.0) / 20.0* (bottom - top);
        };

        // x-axis ticks: wavelength in nm.
        painter.setPen(Qt::black);
        for (int wavelength = 400; wavelength <= 700;
             wavelength += 50) {
            double x = xPixel(wavelength);
            painter.drawLine(QPointF(x, bottom), QPointF(x, bottom + 5));
            painter.drawText(QRectF(x - 20, bottom + 10, 40, 20), Qt::AlignCenter, QString::number(wavelength));
        }

        // y-axis ticks: scattering angle.
        for (int angle = 130; angle <= 150; angle += 5) {
            double y = yPixel(angle);
            painter.drawLine(QPointF(left - 5, y), QPointF(left, y));
            painter.drawText(QRectF(25, y - 10, 50, 20), Qt::AlignRight | Qt::AlignVCenter, QString::number(angle));
        }

        painter.drawText(QRectF(left, height() - 40, right - left, 25), Qt::AlignCenter, "wavelength [nm]");

        painter.save();
        painter.translate(22, (top + bottom) / 2);
        painter.rotate(-90);
        painter.drawText(QRectF(-150, -10, 300, 25), Qt::AlignCenter, "scattering angle [°]");
        painter.restore();

        // plot each photon, colored by wavelength.
        for (const Photon& photon : photons_) {
            double angle = angleDegrees(
                Vector{1.0, 0.0, 0.0}, photon.outgoing
            );

            if (angle < 130.0 || angle > 150.0)
                continue;

            double x = xPixel(photon.wavelength);
            double y = yPixel(angle);

            painter.setPen(Qt::NoPen);
            painter.setBrush(wavelengthColor(photon.wavelength));
            painter.drawEllipse(QPointF(x, y), 2.0, 2.0);
        }
    }

private:
    std::vector<Photon> photons_;
};


// MAIN
int main(int argc, char** argv) {

    // Initialize MPI
    const int MASTER = 0;

    int TASKID, NUMTASKS;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &TASKID);
    MPI_Comm_size(MPI_COMM_WORLD, &NUMTASKS);

    // Simulation parameters
    const long long N_TOTAL = 1000000000;

    // Divide the work between MPI processes.
    long long N_LOCAL = N_TOTAL / NUMTASKS;

    if (TASKID < N_TOTAL % NUMTASKS) {
        N_LOCAL++;
    }

    // Water dispersion data.
    std::vector<double> wavelengths = {
        200, 250, 300, 350, 400, 450, 500, 550, 600,
        650, 700, 750, 800, 850, 900, 950, 1000
    };

    std::vector<double> indices = {
        1.396, 1.362, 1.349, 1.343, 1.339, 1.337,
        1.335, 1.333, 1.332, 1.331, 1.331, 1.330,
        1.329, 1.329, 1.328, 1.327, 1.327
    };

    Interpolator<double> interpolator;

    for (size_t i = 0; i < wavelengths.size(); ++i) {
        interpolator.addPoint(wavelengths[i], indices[i]);
    }

    // Generate photons
    PhotonGenerator generator(42 + TASKID);

    generator.compute(
        N_LOCAL,
        interpolator,
        TASKID == MASTER,
        100000
    );

    std::cout << "Task " << TASKID
              << " traced " << N_LOCAL
              << " photons." << std::endl;

    // Combine across MPI processors
    std::vector<unsigned long long> globalHistogram(NBINS, 0);

    MPI_Reduce(
        generator.harvestHistogram().data(),
        globalHistogram.data(),
        NBINS,
        MPI_UNSIGNED_LONG_LONG,
        MPI_SUM,
        MASTER,
        MPI_COMM_WORLD
    );

    MPI_Finalize();

    // Plot
    if (TASKID == MASTER) {

        unsigned long long totalInRange = 0;

        for (int i = 0; i < NBINS; ++i) {
            totalInRange += globalHistogram[i];
        }

        QApplication app(argc, argv);

        QPalette palette = app.palette();
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::ButtonText, Qt::black);
        app.setPalette(palette);

        RainbowWidget rainbow(generator.harvestSample());
        rainbow.show();

        return app.exec();
    }

    return 0;
}