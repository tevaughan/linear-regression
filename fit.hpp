
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  fit.hpp
/// \brief Declaration of linreg::fit.

#ifndef LINREG_FIT_HPP
#define LINREG_FIT_HPP

#include <memory>    // for shared_ptr<>
#include <Eigen/SVD> // for MatrixX2d, MatrixXd::jacobiSvd(), VectorXd

#include "basis.hpp"

namespace linreg
{
   enum fit_solution { FIT_SIMPLE, FIT_SVD };

   class fit
   {
      typedef std::shared_ptr<abstract_basis const> basis_ptr;
      basis_ptr basis_;
      Eigen::VectorXd coefs_;

   public:
      fit(basis_ptr b, Eigen::MatrixX2d const &d, fit_solution s = FIT_SVD);

      basis_ptr basis() const
      {
         return basis_;
      }

      Eigen::VectorXd const &coefs() const
      {
         return coefs_;
      }

      double operator()(double x) const
      {
         return coefs_.dot((*basis_)(x));
      }
   };
}

#endif // ndef LINREG_FIT_HPP

