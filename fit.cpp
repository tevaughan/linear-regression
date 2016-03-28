
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
/// along with the software.
///
/// \file  fit.cpp
/// \brief Definition of linreg::fit.

#include "fit.hpp"

using namespace Eigen;
using namespace linreg;

fit::fit(basis_ptr b, Eigen::MatrixX2d const& d, fit_solution s) : basis_(b)
{
   unsigned const M = d.rows();
   unsigned const N = b->size();
   MatrixXd const x = d.col(0);
   MatrixXd const y = d.col(1);
   if (s == FIT_SVD) {
      MatrixXd B(M, N);
      for (unsigned i = 0; i < M; ++i) {
         B.row(i) = (*b)(x(i));
      }
      coefs_ = B.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);
   } else {
      throw "simple solution not yet implemented";
   }
}
