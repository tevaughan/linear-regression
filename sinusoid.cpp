
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
/// along with the software.
///
/// \file  sinusoid.cpp
/// \brief Generator for data in sinusoidal example.

#include <iomanip>  // for setprecision()
#include <iostream> // for cout, endl
#include <memory>   // for make_shared<>()
#include <sstream>  // for ostringstream

#include "fake_data.hpp"     // for fake_data
#include "fit.hpp"           // for fourier_basis, polynom_basis, fit, VectorXd
#include "gplot.hpp"         // for gplot
#include "sinusoid_func.hpp" // for sinusoid

using namespace Eigen;
using namespace linreg;
using namespace std;

double constexpr AMP = 1.0; // Amplitude of sinusoidal model.
double constexpr PER = 1.0; // Period of sinusoidal model.
double constexpr PHS = 0.0; // Phase of sinusoidal model.

unsigned constexpr M = 100;      // Number of fake measurements.
double constexpr X1 = 0.0 * PER; // Lower-bound x of measurements
double constexpr X2 = 1.0 * PER; // Upper-bound x of measurements.
double constexpr SIG = 0.3;      // Std dev on y of measurements.

string coefs_str(VectorXd const &c)
{
   ostringstream oss;
   oss << setprecision(3);
   bool first = true;
   for (unsigned j = 0; j < c.size(); ++j) {
      if (first) {
         first = false;
      } else {
         oss << ",";
      }
      oss << " " << c[j];
   }
   return oss.str();
}

int main()
{
   sinusoid const s(AMP, PER, PHS);      // Model underlying fake measurements.
   fake_data const d(M, X1, X2, SIG, s); // Fake measurements.
   auto fb = make_shared<fourier_basis>(1, PER);
   auto pb = make_shared<polynom_basis>(3);
   fit const fourier_fit(fb, d);
   fit const polynom_fit(pb, d);
   gplot gp("sinusoid");
   ostringstream ossf, ossp;
   ossf << "fit on Fourier basis (coefs:" << coefs_str(fourier_fit.coefs())
        << ")";
   ossp << "fit on polynomal basis (coefs:" << coefs_str(polynom_fit.coefs())
        << ")";
   gp.add_func_plot(-0.05 * PER, 1.05 * PER, 100, fourier_fit, "lines",
                    ossf.str());
   gp.add_func_plot(-0.05 * PER, 1.05 * PER, 100, polynom_fit, "lines",
                    ossp.str());
   gp.add_data_plot(d.matrix(), "points", "simulated data");
   gp.xlabel("x");
   gp.ylabel("y");
   gp.write_gpi();
}

