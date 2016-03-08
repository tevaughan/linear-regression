
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  fit.hpp
/// \brief Definition of class linreg::fit.

#ifndef LINREG_FIT_HPP
#define LINREG_FIT_HPP

#include <memory>    // for shared_ptr<>
#include <Eigen/SVD> // for MatrixXd::jacobiSvd()
#include "basis.hpp"

namespace linreg
{
   enum fit_solution {
      FIT_SIMPLE,
      FIT_SVD
   };

   class fit
   {
      typedef std::shared_ptr<abstract_basis const> basis_ptr;
      typedef std::shared_ptr<Eigen::MatrixX2d const> data_ptr;
      basis_ptr basis_;
      data_ptr data_;
      Eigen::VectorXd coefs_;

   public:
      fit(basis_ptr b, data_ptr d, fit_solution s = FIT_SVD)
         : basis_(b), data_(d)
      {
         using namespace Eigen;
         if (s == FIT_SVD) {
            unsigned const M = d->rows();
            MatrixXd B(M, b->size());
            for (unsigned i = 0; i < M; ++i) {
               B.row(i) = (*b)((*d)(i, 0));
            }
            coefs_ = B.jacobiSvd(ComputeThinU | ComputeThinV).solve(d->col(1));
         } else {
            throw "simple solution not yet implemented";
         }
      }
   };
}

#endif // ndef LINREG_FIT_HPP

