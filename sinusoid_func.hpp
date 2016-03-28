
/// Copyright 2016  Thomas E. Vaughan
///
/// The present software is redistributable under the terms of the GNU LESSER
/// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
/// along with the software.
///
/// \file  sinusoid.cpp
/// \brief Generator for data in sinusoidal example.

#ifndef LINREG_SINUSOID_FUNC_HPP
#define LINREG_SINUSOID_FUNC_HPP

#include <cmath> // for sin(), atan()

namespace linreg
{
   /// Sinusoidal function with configurable amplitude, period, and phase.
   class sinusoid
   {
      double amplitude_; ///< Amplitude.
      double angfreq_;   ///< Angular frequency (radians per unit of domain).
      double phase_;     ///< Phase (radians, 0 indicates sine function).

   public:
      /// Construct sinusoidal model with given amplitude, period, and phase.
      /// Phase zero corresponds to the sine function.
      ///
      /// \tparam a   Amplitude.
      /// \tparam p   Period.
      /// \tparam ph  Phase.
      sinusoid(double a, double p, double ph)
         : amplitude_(a), angfreq_(8.0 * atan(1.0) / p), phase_(ph)
      {
      }

      double const &amplitude() const
      {
         return amplitude_;
      }

      double period() const
      {
         return 8.0 * atan(1.0) / angfreq_;
      }

      double const &phase() const
      {
         return phase_;
      }

      double operator()(double t) const
      {
         return amplitude_ * sin(angfreq_ * t + phase_);
      }
   };
}

#endif // ndef LINREG_SINUSOID_FUNC_HPP

