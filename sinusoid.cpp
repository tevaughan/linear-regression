
/// Copyright 2016  Thomas E. Vaughan
///
/// \file  sinusoid.cpp
/// \brief Generator for data in sinusoidal example.

#include <cmath>    // for sin(), cos()
#include <iostream> // for cout, endl
#include <random>   // for default_random_engine, normal_distribution<>

#include <Eigen/SVD>  // for VectorXd, MatrixXd, MatrixXd::jacobiSvd()

using namespace Eigen;
using namespace std;

typedef double (*basis_func)(double);
typedef vector<basis_func> func_basis;

int main(int, char **argv)
{
   func_basis const basis{cos, sin};
   unsigned constexpr M = 100;      // Number of measurements.
   unsigned const N = basis.size(); // Number of coefficients.
   MatrixXd B(M, N);
   VectorXd c(N), x(M), y(M);
   c[0] = 2.1;
   c[1] = 2.4;
   default_random_engine generator;
   normal_distribution<double> distribution;
   double constexpr X_LO = 0.0;
   double constexpr X_HI = 2.0 * M_PI;
   double constexpr DX = (X_HI - X_LO) / (M - 1);
   for (unsigned i = 0; i < M; ++i) {
      x(i) = X_LO + i * DX;
      y(i) = distribution(generator); // Start with noise.
      for (unsigned j = 0; j < N; ++j) {
         B(i, j) = basis[j](x(i));
         y(i) += c[j] * B(i, j); // Add in signal.
      }
   }
   MatrixXd const c_fit = B.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);
   cerr << argv[0] << ": coefs:";
   for (unsigned j = 0; j < N; ++j) {
      cerr << " " << c[j];
   }
   cerr << endl;
   for (unsigned i = 0; i < M; ++i) {
      double fit_y = 0.0;
      for (unsigned j = 0; j < N; ++j) {
         fit_y += c[j] * basis[j](x(i));
      }
      cout << x(i) << " " << y(i) << " " << fit_y << endl;
   }
}

