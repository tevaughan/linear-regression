
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  sinusoid.cpp
/// \brief Generator for data in sinusoidal example.

#include <iostream> // for cout, endl
#include <memory>   // for make_shared<>()

#include "fake_data.hpp" // for fake_data
#include "fit.hpp"   // for abstract_basis, fourier_basis, polynom_basis, fit
#include "gplot.hpp" // for gplot
#include "sinusoid_func.hpp" // for sinusoid

using namespace linreg;
using namespace std;

double constexpr AMP = 1.3;                // Amplitude of sinusoidal model.
double constexpr PER = 1.0;                // Period of sinusoidal model.
double constexpr PHS = 3.1415926536 / 4.0; // Phase of sinusoidal model.

unsigned constexpr M = 100;      // Number of fake measurements.
double constexpr X1 = 0.1 * PER; // Lower-bound x of measurements
double constexpr X2 = 0.9 * PER; // Upper-bound x of measurements.
double constexpr SIG = 0.1;      // Std dev on y of measurements.

int main()
{
   sinusoid const s(AMP, PER, PHS);      // Model underlying fake measurements.
   fake_data const d(M, X1, X2, SIG, s); // Fake measurements.
   auto fb = make_shared<fourier_basis>(1, PER);
   auto pb = make_shared<polynom_basis>(3);
   unsigned const fN = fb->size(); // Number of coefficients for fourier basis.
   unsigned const pN = pb->size(); // Number of coefficients for polynom basis.
   fit const fourier_fit(fb, d);
   fit const polynom_fit(pb, d);
   gplot gp("sinusoid");
   ostringstream ossf, ossp;
   ossf << "fourier coefs:";
   ossp << "polynom coefs:";
   for (unsigned j = 0; j < fN; ++j) {
      ossf << " " << fourier_fit.coefs()[j];
   }
   for (unsigned j = 0; j < pN; ++j) {
      ossp << " " << polynom_fit.coefs()[j];
   }
   gp.add_func_plot(0.0, PER, 100, fourier_fit, "lines", ossf.str());
   gp.add_func_plot(0.0, PER, 100, polynom_fit, "lines", ossp.str());
   gp.add_data_plot(d.matrix());
   gp.write_gpi();
}

