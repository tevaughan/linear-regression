
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
#include <Eigen/SVD> // for MatrixXd::jacobiSvd()
#include "basis.hpp"

namespace linreg
{
   enum fit_solution { FIT_SIMPLE, FIT_SVD };

   class fit
   {
      typedef std::shared_ptr<abstract_basis const> basis_ptr;
      typedef std::shared_ptr<Eigen::MatrixX2d const> data_ptr;
      basis_ptr basis_;
      data_ptr data_;
      Eigen::VectorXd coefs_;

   public:
      fit(basis_ptr b, data_ptr d, fit_solution s = FIT_SVD);

      basis_ptr basis() const
      {
         return basis_;
      }

      data_ptr data() const
      {
         return data_;
      }

      Eigen::VectorXd const &coefs() const
      {
         return coefs_;
      }
   };
}

#endif // ndef LINREG_FIT_HPP

