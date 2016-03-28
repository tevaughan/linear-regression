
// Copyright 2016  Thomas E. Vaughan
//
// The present software is redistributable under the terms of the GNU LESSER
// GENERAL PUBLIC LICENSE, which is must be distributed in the file, 'LICENSE',
// along with the software.

#include <sstream> // for ostringstream
#include "gplot.hpp"

using namespace Eigen;
using namespace linreg;
using namespace std;

plot::plot(string const &d, string const &w, string const &t)
   : datafile_(d), title(t)
{
   with(w);
}

void plot::with(string const &w)
{
   if (w == "points" || w == "lines") {
      with_ = w;
   } else {
      throw "unknown plot style '" + w + "'";
   }
}

ostream &linreg::operator<<(ostream &os, plot const &p)
{
   return os << '"' + p.datafile() + '"' << " with " << p.with() << " title "
             << '"' + p.title + '"';
}

gplot::gplot(string const &basename)
   : basename_(basename)
   , options_{{"grid", ""},
              {"key", "below"},
              {"output", '"' + basename + ".pdf" + '"'},
              {"term", "pdfcairo"}}
{
}

void gplot::grid(bool f)
{
   if (f) {
      options_["grid"] = "";
   } else {
      options_.erase("grid");
   }
}

void gplot::key(string const &k)
{
   if (k == "below" || k == "outside") {
      options_["key"] = k;
   } else {
      throw string("unknown key style '") + k + "'";
   }
}

void gplot::xlabel(string const &xl)
{
   if (xl == "") {
      options_.erase("xlabel");
   } else {
      options_["xlabel"] = '"' + xl + '"';
   }
}

void gplot::xrange(double x1, double x2)
{
   if (x1 == x2) {
      options_.erase("xrange");
   } else {
      ostringstream oss;
      oss << "[" << x1 << ":" << x2 << "]";
      options_["xrange"] = oss.str();
   }
}

void gplot::ylabel(string const &yl)
{
   if (yl == "") {
      options_.erase("ylabel");
   } else {
      options_["ylabel"] = '"' + yl + '"';
   }
}

void gplot::yrange(double y1, double y2)
{
   if (y1 == y2) {
      options_.erase("yrange");
   } else {
      ostringstream oss;
      oss << "[" << y1 << ":" << y2 << "]";
      options_["yrange"] = oss.str();
   }
}

string gplot::dfname() const
{
   ostringstream oss_fname;
   oss_fname << basename_ << plots_.size() << ".dat";
   return oss_fname.str();
}

void gplot::add_data_plot(MatrixX2d const &d, string const &w, string const &t)
{
   string const fname = dfname();
   ofstream ofs(fname);
   for (unsigned i = 0; i < d.rows(); ++i) {
      ofs << d(i, 0) << " " << d(i, 1) << endl;
   }
   plots_.push_back(plot(fname, w, t));
}

void gplot::write_gpi() const
{
   ofstream ofs(basename_ + ".gpi");
   ofs << "\n";
   for (auto &p : options_) {
      ofs << "set " << p.first << " " << p.second << "\n";
   }
   ofs << "\nplot";
   bool first = true;
   for (auto &p : plots_) {
      if (first) {
         // On first pass, we terminate line containing only plot keyword, and
         // so no comma is required to separate one plot element from the next.
         first = false;
      } else {
         ofs << ",";
      }
      ofs << "\\\n " << p;
   }
   ofs << "\n" << endl;
}

