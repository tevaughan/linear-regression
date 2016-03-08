
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
#include <memory>   // for make_shared<>()
#include <random>   // for default_random_engine, normal_distribution<>

#include <Eigen/SVD> // for VectorXd, MatrixXd, MatrixXd::jacobiSvd()

#include "fit.hpp" // for abstract_basis, fourier_basis, polynom_basis, fit

using namespace Eigen;
using namespace linreg;
using namespace std;

shared_ptr<MatrixX2d> generate_data(unsigned const M,
                                    shared_ptr<abstract_basis> b)
{
   unsigned const N = b->size();
   auto data = make_shared<MatrixX2d>(M, 2);
   VectorXd c(N);
   for (unsigned i = 0; i < N; ++i) {
      c[i] = i + 1.0;
   }
   default_random_engine generator;
   normal_distribution<double> distribution;
   double constexpr X_LO = 0.0;
   double constexpr X_HI = 2.0 * M_PI;
   double const DX = (X_HI - X_LO) / (M - 1);
   for (unsigned i = 0; i < M; ++i) {
      double &x = (*data)(i, 0);
      double &y = (*data)(i, 1);
      x = X_LO + i * DX;
      y = distribution(generator) + c.dot((*b)(x));
   }
   return data;
}

int main(int, char **argv)
{
   auto fb = make_shared<fourier_basis>(1);
   auto pb = make_shared<polynom_basis>(3);
   unsigned const fN = fb->size(); // Number of coefficients for fourier.
   unsigned const pN = pb->size(); // Number of coefficients for polynom.
   unsigned constexpr M = 100;     // Number of measurements.
   auto data = generate_data(M, fb);
   fit const fourier_fit(fb, data);
   fit const polynom_fit(pb, data);
   cerr << argv[0] << ": fourier coefs:";
   for (unsigned j = 0; j < fN; ++j) {
      cerr << " " << fourier_fit.coefs()[j];
   }
   cerr << argv[0] << "  polynomial coefs:";
   for (unsigned j = 0; j < pN; ++j) {
      cerr << " " << polynom_fit.coefs()[j];
   }
   cerr << endl;
   for (unsigned i = 0; i < M; ++i) {
      double &x = (*data)(i, 0);
      double const &y = (*data)(i, 1);
      double const ffit_y = fourier_fit.coefs().dot((*fb)(x));
      double const pfit_y = polynom_fit.coefs().dot((*pb)(x));
      cout << x << " " << y << " " << ffit_y << " " << pfit_y << endl;
   }
}

