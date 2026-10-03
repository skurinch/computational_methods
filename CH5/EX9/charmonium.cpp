// charmonium 
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/GaussIntegrator.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotProfile.h"
#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QTextEdit>
#include <QLabel>
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace Genfun;

// constants
const double hbarc = 197.3269804;
const double mc    = 1370.0;
const double alpha = 0.38;
const double a     = 2.43e-3; 


// potential
double V(double r)
{
    return -4.0 * alpha * hbarc / (3.0 * r) + r / (hbarc * a * a);
}


// momentum function p^2(r) = 2 m (E - V(r)) - L^2 / r^2
double p2(double r, double E, int l)
{
    double L = l + 0.5;

    return 2.0 * mc * (E - V(r)) - hbarc * hbarc * L * L / (r * r);
}


// find the limits of the integration by finding the turning points
std::vector<double> turning_points(double E, int l)
{
    std::vector<double> roots;

    const int N = 10000;
    const double rmin = 1.0e-6;
    const double rmax = 100.0;

    double r_old = rmin;
    double f_old = p2(r_old, E, l);

    for (int i = 1; i <= N; ++i)
    {
        double r = rmin
                 + (rmax - rmin) * i / N;

        double f = p2(r, E, l);

        if (f_old * f < 0.0)
        {
            double left = r_old;
            double right = r;

            for (int j = 0; j < 50; ++j)
            {
                double middle = 0.5 * (left + right);

                if (p2(left, E, l) * p2(middle, E, l) < 0.0)
                    right = middle;
                else
                    left = middle;
            }

            roots.push_back(0.5 * (left + right));

            if (roots.size() == 2)
                break;
        }

        r_old = r;
        f_old = f;
    }

    return roots;
}

// momentum function for the Gauss integrator
class Momentum : public AbsFunction
{
public:
    Momentum(double E, int l, double rminus, double rplus):
        E_(E),
        l_(l),
        rminus_(rminus),
        rplus_(rplus)
    {
    }

    double operator()(double x) const
    {
        double rmid = 0.5 * (rplus_ + rminus_);
        double dr   = 0.5 * (rplus_ - rminus_);

        double r = rmid + dr * x;

        return sqrt(p2(r, E_, l_)) * dr;
    }

    double operator()(const Argument& x) const
    {
        return (*this)(x[0]);
    }

    Momentum* clone() const
    {
        return new Momentum(*this);
    }

private:
    double E_;
    int l_;
    double rminus_;
    double rplus_;
};


// integral p_r dr
double integrate(double E, int l)
{
    std::vector<double> roots = turning_points(E, l);

    if (roots.size() != 2)
    {
        std::cerr << "Could not find two turning points.\n";
        return -1.0;
    }

    double rminus = roots[0];
    double rplus  = roots[1];

    GaussLegendreRule rule(100);

    GaussIntegrator integrator(rule, GaussIntegrator::INTEGRATE_DX);

    Momentum f(E, l, rminus, rplus);

    return integrator(f);
}

// I(E) = (n_r + 1/2) pi hbar c
double find_energy(int nr, int l)
{
    double target = (nr + 0.5) * M_PI * hbarc;

    double Elow   = 0.0;
    double Ehigh  = 3000.0;

    for (int i = 0; i < 100; ++i)
    {
        double E = 0.5 * (Elow + Ehigh);

        double I = integrate(E, l);

        if (I < target)
            Elow = E;
        else
            Ehigh = E;
    }


    return 0.5 * (Elow + Ehigh);
}

// ==================================================================================================================

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    std::cout << std::fixed
              << std::setprecision(3);

    // Store the six energy levels
    std::vector<double> energies;
    std::vector<std::string> labels;

    // S states
    std::cout << "S states (l = 0)\n";

    for (int nr = 0; nr <= 2; ++nr)
    {
        double E = find_energy(nr, 0);
        double total_energy = 2.0 * mc + E;

        energies.push_back(total_energy);
        labels.push_back("S" + std::to_string(nr));

        std::cout
            << "n_r = " << nr << "  E = " << E << " MeV  "  << "Total = " << total_energy << " MeV\n";
    }

    std::cout << "\n";

    // P states
    std::cout << "P states (l = 1)\n";

    for (int nr = 0; nr <= 2; ++nr)
    {
        double E = find_energy(nr, 1);
        double total_energy = 2.0 * mc + E;

        energies.push_back(total_energy);
        labels.push_back("P" + std::to_string(nr));

        std::cout
            << "n_r = " << nr << "  E = " << E << " MeV  "  << "Total = " << total_energy << " MeV\n";
    }

    // set up plot
    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::black);
    app.setPalette(palette);

    PRectF rect;
    rect.setXmin(0.0);
    rect.setXmax(6.0);
    rect.setYmin(2700);
    rect.setYmax(4000);

    QMainWindow window;
    PlotView view(rect);
    view.setBackgroundBrush(Qt::white);
    view.xLabelTextEdit()->setText("State");
    view.xLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.xLabelTextEdit()->setTextColor(Qt::black);
    view.yLabelTextEdit()->setText("Energy (MeV)");
    view.yLabelTextEdit()->setAlignment(Qt::AlignHCenter);
    view.yLabelTextEdit()->setTextColor(Qt::black);
    window.setCentralWidget(&view);

    for (size_t i = 0; i < energies.size(); ++i)
    {
        PlotProfile *profile = new PlotProfile();

        PlotProfile::Properties properties;

        properties.pen.setColor(Qt::black);
        properties.pen.setWidth(3);

        profile->setProperties(properties);

        double x1 = i + 0.2;
        double x2 = i + 0.8;

        profile->addPoint(x1, energies[i]);
        profile->addPoint(x2, energies[i]);

        view.add(profile);
    }

    // save
    window.resize(800, 600);
    window.show();
    QImage image(view.size(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    view.render(&painter);
    painter.end();
    image.save("energy_levels.jpg", "JPG");
    return app.exec();
}