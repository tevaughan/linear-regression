
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
/// along with the software.
///
/// \file  fit.hpp
/// \brief Declaration of linreg::fit.

#ifndef LINREG_FIT_HPP
#define LINREG_FIT_HPP

#include <memory>    // for shared_ptr<>
#include <Eigen/SVD> // for MatrixX2d, VectorXd
#include "basis.hpp" // for abstract_basis

namespace linreg
{
   /// Method of finding coefficients of linear regression.
   enum fit_solution {
      /// Invert the square matrix obtained by differentiating the sum of
      /// squared deviations with respect to each coefficient, by setting each
      /// resultant equation to zero, and expressing the result as a matrix
      /// equation.
      FIT_SIMPLE,

      /// Find the best fitting coeffiecients by way of the singular-value
      /// decomposition of the rectangular matrix that is a factor of the
      /// square matrix described above.
      FIT_SVD
   };

   /// Best coefficients for fitting a set of basis functions to a collection
   /// of data points; an instance of class fit also serves as the
   /// corresponding function object.
   class fit
   {
      typedef std::shared_ptr<abstract_basis const> basis_ptr; ///< Short hand.
      basis_ptr basis_;       ///< Shared pointer to basis.
      Eigen::VectorXd coefs_; ///< Best-fit coefficients.

   public:
      /// Construct from basis, data, and (optionally) the method of fit.
      fit(basis_ptr b, Eigen::MatrixX2d const &d, fit_solution s = FIT_SVD);

      /// \return Shared pointer to basis.
      basis_ptr basis() const
      {
         return basis_;
      }

      /// \return Reference to best-fit coefficients.
      Eigen::VectorXd const &coefs() const
      {
         return coefs_;
      }

      /// \return Value of best-fit function at specified argument.
      double operator()(double x) const
      {
         return coefs_.dot((*basis_)(x));
      }
   };
}

#endif // ndef LINREG_FIT_HPP

