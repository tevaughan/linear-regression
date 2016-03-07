
# Linear Regression, the Best-Fit Curve, and C++

**Linear regression can be used to find the best-fit curve through a set of
points in the plane.**

The linear-regression project aims to produce a PDF document (the most recent
version of which can be obtained on the [release page]), a C++ library, and a
command-line tool (whose source is C++).

The document describes
 - the mathematics of linear regression,
 - how to set up a linear regression in [Eigen 3]
 - how to use the linear-regression library, and
 - how to use the linear-regression command-line tool.

At present, the document shows all of the mathematics for the straight-forward,
computationally fastest approach. Also, the document gives a brief mathematical
introduction that leads to the slower, but more numerically robust SVD-based
approach. While the document does not go into the details of the mathematics
for the SVD-based approach, the document does show how to code up the SVD-based
linear regression in Eigen 3.

There is an example application (which generates a figure for the document).
This application is a fully working implementation of the SVD-based approach to
linear regression.

The library and executable are not written yet.

 - [Build]

[release page]: https://github.com/tevaughan/linear-regression/releases
[Eigen 3]: http://eigen.tuxfamily.org/index.php?title=Main_Page
[Build]: ./BUILD.md

