
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

#include <Eigen/SVD>  // for VectorXd, MatrixXd, MatrixXd::jacobiSvd()

#include "basis.hpp"

using namespace Eigen;
using namespace linreg;
using namespace std;

double unit(double)
{
   return 1.0;
}

double mycos(double x)
{
   return cos(x);
}

double mysin(double x)
{
   return sin(x);
}

int main(int, char **argv)
{
   auto const b = make_basis(unit, mycos, mysin);
   unsigned constexpr M = 100;  // Number of measurements.
   unsigned const N = b.size(); // Number of coefficients.
   MatrixXd B(M, N);
   VectorXd c(N), x(M), y(M);
   c[0] = 1.0;
   c[1] = 2.0;
   c[2] = 3.0;
   default_random_engine generator;
   normal_distribution<double> distribution;
   double constexpr X_LO = 0.0;
   double constexpr X_HI = 2.0 * M_PI;
   double constexpr DX = (X_HI - X_LO) / (M - 1);
   for (unsigned i = 0; i < M; ++i) {
      x(i) = X_LO + i * DX;
      y(i) = distribution(generator); // Start with noise.
      B.row(i) = b(x(i));             // Evaluate basis functions.
      y(i) += c.dot(B.row(i));        // Add in signal.
   }
   VectorXd const c_fit = B.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);
   cerr << argv[0] << ": coefs:";
   for (unsigned j = 0; j < N; ++j) {
      cerr << " " << c_fit[j];
   }
   cerr << endl;
   for (unsigned i = 0; i < M; ++i) {
      double const fit_y = c_fit.dot(b(x(i)));
      cout << x(i) << " " << y(i) << " " << fit_y << endl;
   }
}

