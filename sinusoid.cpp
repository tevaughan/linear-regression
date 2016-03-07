
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  sinusoid.cpp
/// \brief Generator for data in sinusoidal example.

#include <cmath>    // for sin(), cos()
#include <iostream> // for cout, endl
#include <random>   // for default_random_engine, normal_distribution<>

#include <Eigen/SVD> // for VectorXd, MatrixXd, MatrixXd::jacobiSvd()

#include "basis.hpp" // for fourier_basis

using namespace Eigen;
using namespace linreg;
using namespace std;

int main(int, char **argv)
{
   fourier_basis const fb(1);
   polynomial_basis const pb(3);
   unsigned constexpr M = 100;    // Number of measurements.
   unsigned const fN = fb.size(); // Number of coefficients for fourier.
   unsigned const pN = pb.size(); // Number of coefficients for polynomial.
   MatrixXd fB(M, fN), pB(M, pN);
   VectorXd fc(fN), x(M), y(M);
   for (unsigned i = 0; i < fN; ++i) {
      fc[i] = i + 1.0;
   }
   default_random_engine generator;
   normal_distribution<double> distribution;
   double constexpr X_LO = 0.0;
   double constexpr X_HI = 2.0 * M_PI;
   double constexpr DX = (X_HI - X_LO) / (M - 1);
   for (unsigned i = 0; i < M; ++i) {
      x(i) = X_LO + i * DX;
      y(i) = distribution(generator); // Start with noise in measured data.
      fB.row(i) = fb(x(i));           // Evaluate basis for fourier.
      pB.row(i) = pb(x(i));           // Evaluate basis for polynomial.
      y(i) += fc.dot(fB.row(i));      // Add (fourier) signal to measured data.
   }
   VectorXd const fc_fit = fB.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);
   VectorXd const pc_fit = pB.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);
   cerr << argv[0] << ": fourier coefs:";
   for (unsigned j = 0; j < fN; ++j) {
      cerr << " " << fc_fit[j];
   }
   cerr << argv[0] << "  polynomial coefs:";
   for (unsigned j = 0; j < fN; ++j) {
      cerr << " " << pc_fit[j];
   }
   cerr << endl;
   for (unsigned i = 0; i < M; ++i) {
      double const ffit_y = fc_fit.dot(fb(x(i)));
      double const pfit_y = pc_fit.dot(pb(x(i)));
      cout << x(i) << " " << y(i) << " " << ffit_y << " " << pfit_y << endl;
   }
}

