
// Copyright 2016  Thomas E. Vaughan
//
// The present software is redistributable under the terms of the GNU LESSER
// GENERAL PUBLIC LICENSE, which must be distributed in the file, 'LICENSE',
// along with the software.

#ifndef LINREG_GPLOT_HPP
#define LINREG_GPLOT_HPP

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <Eigen/Core> // for MatrixX2d

namespace linreg
{
   /// Description of one, complete argument to gnuplot's 'plot' command. The
   /// gplot class interfaces to gnuplot only through a data file for each
   /// plot, and so the plot class always refers to a data file. (That is,
   /// gnuplot's built-in ability to define and plot a function is not used,
   /// not even when gplot::add_func_plot() is called.)
   class plot
   {
      std::string datafile_; ///< Name of file containing two columns of data.
      std::string with_;     ///< Style string, such as "points" or "lines".

   public:
      std::string title; ///< Title string appearing in key.

      /// Construct a plot element.
      /// \param d  Name of file containing two columns of data.
      /// \param w  Style string, such as "points" or "lines".
      /// \param t  Title string appearing in key.
      plot(std::string const &d, std::string const &w, std::string const &t);

      /// \return Name of file containing two columns of data.
      std::string const &datafile() const
      {
         return datafile_;
      }

      /// \return Style string, usch as "points" or "lines".
      std::string const &with() const
      {
         return with_;
      }

      /// Set style string, to something like "points" or "lines".
      void with(std::string const &w);
   };

   /// Send instance of plot class to output stream in format expected by
   /// gnuplot on plot line.
   std::ostream &operator<<(std::ostream &os, plot const &p);

   /// Representation of a graph, which can be written out to a gnuplot input
   /// file and its associated data files. The gnuplot configuration generated
   /// by gplot::write_gpi() will, after gnuplot processes the generated
   /// inputs, produce a PDF output file via gnuplot's 'pdfcairo' engine.
   class gplot
   {
      /// String used to construct name of input file, name of each data file,
      /// and name of output graphics file.
      std::string basename_;

      /// List of key-value strings used to construct gnuplot 'set' commands.
      std::map<std::string, std::string> options_;

      /// List of elements to gnuplot's 'plot' command.
      std::vector<plot> plots_;

      std::string dfname() const; ///< \return Name of current data file.

   public:
      /// Construct new gplot instance with specified basename; by default,
      /// grid is turned on, and key is set below graph.
      gplot(std::string const &basename);

      void grid(bool f); ///< Turn on grid only if argument be true.

      /// Set key style to something like "below" or "outside".
      void key(std::string const &k);

      void xlabel(std::string const &xl);            ///< Set label for x axis.
      void xrange(double x1 = 0.0, double x2 = 0.0); ///< Set range for x axis.
      void ylabel(std::string const &yl);            ///< Set label for y axis.
      void yrange(double y1 = 0.0, double y2 = 0.0); ///< Set range for y axis.

      /// Add a function to plot. Because gplot interacts with gnuplot only
      /// through data files, the function is evaluated, and the resultant
      /// points are stored in a file.
      ///
      /// \tparam F   Type of function to evaluate.
      /// \param  x1  Lower bound of domain over which function is evaluated.
      /// \param  x2  Upper bound of domain over which function is evaluated.
      /// \param  n   Number of evenly spaced points to evaluate on domain.
      /// \param  f   Function to evaluate.
      /// \param  w   Style string, such as "lines" or "points".
      /// \param  t   Title string appearing in key.
      template <typename F>
      void add_func_plot(double x1, double x2, unsigned n, F f,
                         std::string const &w = "lines",
                         std::string const &t = "")
      {
         using namespace std;
         string const fname = dfname();
         ofstream ofs(fname);
         if (n < 2) {
            throw "At least two points required for function plot.";
         }
         double const dx = (x2 - x1) / (n - 1);
         for (unsigned i = 0; i < n; ++i) {
            double const x = x1 + i * dx;
            ofs << x << " " << f(x) << endl;
         }
         plots_.push_back(plot(fname, w, t));
      }

      /// Add a set of data to plot.
      /// \param d  Two-column set of data.
      /// \param w  Style string, such as "lines" or "points".
      /// \param t  Title string appearing in key.
      void add_data_plot(Eigen::MatrixX2d const &d,
                         std::string const &w = "points",
                         std::string const &t = "");

      void write_gpi() const; ///< Write inputs to gnuplot.
   };
}

#endif // ndef LINREG_GPLOT_HPP

