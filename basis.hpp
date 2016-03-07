
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which is must be distributed in the file,
/// 'LICENSE', along with the software.
///
/// \file  basis.hpp
/// \brief Definition of class linreg::basis.

#ifndef LINREG_BASIS_HPP
#define LINREG_BASIS_HPP

#include <deque> // for deque<>

#include <Eigen/Core> // for VectorXd

namespace linreg
{
   /// Collection of basis functions for a linear regression.
   ///
   /// A deque is used instead of a vector for the base class because the
   /// make_basis() function, which calls itself recursively, needs to be able
   /// to push onto the front.
   ///
   /// \tparam PF  Type of pointer to function. Typically, an instance of PF
   ///             should be either a regular C-style pointer to a global
   ///             function, which takes a double and returns a double, or an
   ///             instance of std::shared_ptr<F>, where type F is a class that
   ///             overloads operator() to take double and return double.
   ///             In any event, an instance pf of PF should return a double
   ///             when called as (*pf)(2.0).
   template <typename PF>
   struct basis : public std::deque<PF> {
      using std::deque<PF>::deque; ///< Inherit constructors.

      /// \return Value of each basis function at given value of its argument.
      Eigen::VectorXd operator()(double const x) const;
   };

   template <typename PF>
   Eigen::VectorXd basis<PF>::operator()(double const x) const
   {
      unsigned const sz = this->size();
      Eigen::VectorXd result(sz);
      for (unsigned i = 0; i < sz; ++i) {
         PF const &pf = (*this)[i];
         result(i) = (*pf)(x);
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
      r.push_front(p);
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
      r.push_front(p);
      return r;
   }

   /// Copy function pointers into new basis.
   template <template <class> class C, typename PF>
   basis<PF> make_basis(C<PF> c)
   {
      return c;
   }
}

#endif // ndef LINREG_BASIS_HPP

