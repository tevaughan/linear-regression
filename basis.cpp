
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  basis.cpp
/// \brief Definition of linreg::polynomial_basis, linreg::fourier_basis.

#include <cmath> // for cos(), sin()
#include "basis.hpp"

using namespace Eigen;
using namespace linreg;

VectorXd polynom_basis::operator()(double const x) const
{
   unsigned const sz = size();
   VectorXd result(sz);
   result(0) = 1.0;
   for (unsigned i = 1; i < sz; ++i) {
      result(i) = result(i - 1) * x;
   }
   return result;
}

VectorXd fourier_basis::operator()(double const x) const
{
   unsigned const sz = size();
   VectorXd result(sz);
   result(0) = 1.0;
   for (unsigned i = 1; i < sz; i += 2) {
      unsigned const j = (i + 1) / 2;
      double const k = j * angfreq_;
      result(i + 0) = cos(k * x);
      result(i + 1) = sin(k * x);
   }
   return result;
}

