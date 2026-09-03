#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex>

int main() {

    // Physical constants
    const double hbar = 1.054571817e-34;   // J s
    const double m    = 9.1093837e-31;     // electron mass (kg)
    const double eV   = 1.602176634e-19;   // joules

    // Particle energy and potential
    double E_eV, V_eV;

    std::cout << "Enter particle energy E (eV): ";
    std::cin >> E_eV;

    std::cout << "Enter potential V (eV): ";
    std::cin >> V_eV;

    double E = E_eV * eV;
    double V = V_eV * eV;

    // Complex number type
    using Complex = std::complex<double>;

    // Imaginary unit
    const Complex I(0.0, 1.0);

    // ------------------------------------------------
    // Wave numbers
    // ------------------------------------------------

    // k1 is real because V = 0 on the left
    Complex k1 = std::sqrt(Complex(2.0 * m * E)) / hbar;

    // k2 is complex and works for both E > V and E < V
    Complex k2 = std::sqrt(Complex(2.0 * m * (E - V))) / hbar;

    // ------------------------------------------------
    // Reflection and transmission amplitudes
    // ------------------------------------------------

    Complex r = (k1 - k2) / (k1 + k2);
    Complex t = (2.0 * k1) / (k1 + k2);

    // Reflection coefficient
    double R = std::norm(r);

    // Transmission coefficient
    double T = (k2.real() / k1.real()) * std::norm(t);

    // ------------------------------------------------
    // Print results
    // ------------------------------------------------

    std::cout << std::setprecision(10);

    std::cout << "k1 = " << k1 << " 1/m\n";

    std::cout << "k2 = " << k2 << " 1/m\n";

    std::cout << "\nr = "<< r  << "\n";

    std::cout << "t = "  << t  << "\n";

    std::cout << "\nR = " << R << "\n";

    std::cout << "T = " << T << "\n";

    // ------------------------------------------------
    // Calculate wavefunction
    // ------------------------------------------------

    std::cout << "\n--- Wavefunction ---\n";

    std::cout << "x (nm)" << std::setw(18) << "Re(psi)"
                          << std::setw(18) << "Im(psi)"
                          << std::setw(18) << "|psi|^2"
              << "\n";

    for (int n = -100; n <= 100; n++) {

        double x = n * 0.1e-9;  // meters

        Complex psi;

        if (x < 0.0) {
            // Incident + reflected wave
            psi = std::exp(I * k1 * x)
                + r * std::exp(-I * k1 * x);

        } else {
            // Transmitted wave
            psi = t * std::exp(I * k2 * x);
        }

        std::cout << std::fixed << std::setprecision(3) << x * 1e9

                  << std::scientific << std::setw(18) << psi.real()

                  << std::setw(18) << psi.imag()

                  << std::setw(18) << std::norm(psi)

                  << "\n";
    }

    return 0;
}

