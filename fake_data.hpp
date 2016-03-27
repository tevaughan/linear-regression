
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  basis.hpp
///
/// \brief Definition of linreg::basis; declaration of linreg::polynom_basis,
///        linreg::fourier_basis.

#ifndef LINREG_FAKE_DATA_HPP
#define LINREG_FAKE_DATA_HPP

#include <random>     // for default_random_engine, normal_distribution<>
#include <Eigen/Core> // for MatrixX2d

namespace linreg
{
   /// Fake data set obtained by adding zero-mean Gaussian noise to a model.
   class fake_data
   {
      Eigen::MatrixX2d data_; ///< Storage for data.

   public:
      /// Construct a fake data set by adding zero-mean Gaussian noise to a
      /// model function. The points are distributed evenly along the abscissa
      /// between a lower bound and an upper bound.
      ///
      /// \tparam F  Type of model function, to which noise is added. An
      ///            instance of type F must be a function (or function object)
      ///            that takes a double and returns a double.
      ///
      /// \param m   Number of points.
      /// \param x1  Lower bound on abscissa.
      /// \param x2  Upper bound on abscissa.
      /// \param s   Standard deviation of Gaussian noise along ordinate.
      /// \param f   Model function.
      template <typename F>
      fake_data(int m, double x1, double x2, double s, F f)
         : data_(m, 2)
      {
         using namespace std;
         default_random_engine generator;
         normal_distribution<double> distribution(0.0, s);
         double const dx = (x2 - x1) / (m - 1);
         for (int i = 0; i < m; ++i) {
            data_(i, 0) = x1 + i * dx;
            data_(i, 1) = distribution(generator) + f(data_(i, 0));
         }
      }

      /// \return Data as an mx2 matrix.
      Eigen::MatrixX2d const &matrix() const
      {
         return data_;
      }

      /// \return Data as an mx2 matrix.
      operator Eigen::MatrixX2d const &() const
      {
         return data_;
      }
   };
}

#endif // ndef LINREG_FAKE_DATA_HPP

