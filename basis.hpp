
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
/// along with the software.
///
/// \file  basis.hpp
///
/// \brief Definition of linreg::basis; declaration of linreg::polynom_basis,
///        linreg::fourier_basis.

#ifndef LINREG_BASIS_HPP
#define LINREG_BASIS_HPP

#include <deque>      // for deque<>
#include <Eigen/Core> // for VectorXd

namespace linreg
{
   template <typename PF>
   class basis;

   template <typename PF>
   basis<PF> make_basis(PF p);

   template <typename PF, typename... Targs>
   basis<PF> make_basis(PF p, Targs... Fargs);

   /// Abstract base class for every type of basis.
   ///
   /// Each basis type must provide an operator() that takes a double x and
   /// returns a vector of values, each corresponding to a different basis
   /// function evaluated at the same x.
   ///
   /// Also, each basis type must report the size of the vector that will be
   /// returned by operator().
   struct abstract_basis {
      /// \return Value of each basis function at given value of its argument.
      virtual Eigen::VectorXd operator()(double const x) const = 0;

      /// \return Number of elements in vector returned by operator().
      virtual unsigned size() const = 0;

      /// Make sure that descendant's destructor is called.
      virtual ~abstract_basis() = default;
   };

   /// General, custom collection of basis functions for a linear regression.
   /// An instance of basis can be used when a user-defined collection of basis
   /// functions is needed. For a standard basis, such as the polynomial basis
   /// or the Fourier basis, use polynom_basis or fourier_basis, respectively.
   ///
   /// \tparam PF  Type of pointer to function. Typically, an instance of PF
   ///             should be either a regular C-style pointer to a global
   ///             function, which takes a double and returns a double, or an
   ///             instance of std::shared_ptr<F>, where type F is a class that
   ///             overloads operator() to take double and return double.
   ///             In any event, an instance pf of PF should return a double
   ///             when called as (*pf)(2.0).
   template <typename PF>
   class basis : public abstract_basis
   {
      /// A deque is used instead of a vector for storage because make_basis()
      /// function, which calls itself recursively, needs to be able to push
      /// onto the front.
      std::deque<PF> d_;

   public:
      /// Make sure that descendant's destructor is called.
      virtual ~basis() = default;

      /// \return Value of each basis function at given value of its argument.
      Eigen::VectorXd operator()(double const x) const override;

      friend basis make_basis<PF>(PF p);

      template <typename TPF, typename... Targs>
      friend basis<TPF> make_basis(TPF p, Targs... Fargs);

      /// \return Number of elements in vector returned by operator().
      unsigned size() const override
      {
         return d_.size();
      }
   };

   template <typename PF>
   Eigen::VectorXd basis<PF>::operator()(double const x) const
   {
      unsigned const sz = this->size();
      Eigen::VectorXd result(sz);
      for (unsigned i = 0; i < sz; ++i) {
         result(i) = (*d_[i])(x);
      }
      return result;
   }

   /// Construct, initialize, and return a basis containing exactly one basis
   /// function.
   ///
   /// This non-variadic function template is called to terminate the recursive
   /// call in the variadic function template.
   ///
   /// Note that automatic template-type deduction will work when p is a
   /// function pointer only when the passed in function has no overloads. So,
   /// for example, the global cos() function delcared in <cmath> will not
   /// allow template-type deduction of PF because cos() has overloads.
   template <typename PF>
   basis<PF> make_basis(PF p)
   {
      basis<PF> r;
      r.d_.push_front(p);
      return r;
   }

   /// Construct, initialize, and return a basis containing the argument list
   /// of basis functions.
   ///
   /// This variadic function template calls itself until it terminates by
   /// calling the non-variadic function template.
   ///
   /// Note that each basis function must have the same type.
   ///
   /// Note that automatic template-type deduction will work when the arguments
   /// are function pointers, but only when no passed in function has an
   /// overload. For example, the global cos() function delcared in <cmath>
   /// will not allow template-type deduction because cos() has overloads.
   template <typename PF, typename... Targs>
   basis<PF> make_basis(PF p, Targs... Fargs)
   {
      basis<PF> r = make_basis(Fargs...); // Recursive call.
      r.d_.push_front(p);
      return r;
   }

   /// Base class for standard bases, such as polynom_basis and fourier_basis.
   struct standard_basis : public abstract_basis {
      /// Make sure that descendant's destructor is called.
      virtual ~standard_basis() = default;

   protected:
      /// Indicator of the number of basis functions. For a polynomial, the
      /// number of basis functions is one more than the degree. For a fourier
      /// basis, the number is one more than twice the degree.
      unsigned degree_;

      /// Initialize degree on construction.
      standard_basis(unsigned const d) : degree_(d)
      {
      }
   };

   /// Polynomial basis of finite degree.
   struct polynom_basis : public standard_basis {
      /// Construct from specified degree of polynomial. The number of basis
      /// functions in the basis will be equal to one more than the degree.
      polynom_basis(unsigned const d) : standard_basis(d)
      {
      }

      /// Make sure that descendant's destructor is called.
      virtual ~polynom_basis() = default;

      /// \return Value of each basis function at given value of its argument.
      ///         - Element 0 corresponds to the constant function f(x) = 1;
      ///         - Element 1 corresponds to f(x) = x;
      ///         - Element 2 corresponds to f(x) = x*x;
      ///         - etc.
      Eigen::VectorXd operator()(double const x) const override;

      /// \return One more than degree of polynomial. This is the number of
      ///         elements in the vector returned by operator().
      unsigned size() const override
      {
         return degree_ + 1;
      }
   };

   /// Fourier basis of finite degree.
   class fourier_basis : public standard_basis
   {
      double angfreq_; ///< Angular frequency corresponding to fundamental period.

   public:
      /// Construct from specified degree and fundamental period of fourier
      /// basis. The number of basis functions in the basis will be equal to
      /// one more than twice the degree.
      fourier_basis(unsigned const deg, double fper)
         : standard_basis(deg), angfreq_(8.0 * atan(1.0) / fper)
      {
      }

      /// Make sure that descendant's destructor is called.
      virtual ~fourier_basis() = default;

      /// \return Value of each basis function at given value of its argument.
      ///         - Element 0 corresponds to the constant function f(x) = 1;
      ///         - Element 1 corresponds to f(x) = cos(x);
      ///         - Element 2 corresponds to f(x) = sin(x);
      ///         - Element 3 corresponds to f(x) = cos(2*x);
      ///         - Element 4 corresponds to f(x) = sin(2*x);
      ///         - etc.
      Eigen::VectorXd operator()(double const x) const override;

      /// \return Number (2*degree + 1) of elements in the vector returned by
      ///         operator().
      unsigned size() const override
      {
         return 2 * degree_ + 1;
      }
   };
}

#endif // ndef LINREG_BASIS_HPP

