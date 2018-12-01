#include "line_search.h"
#include <iostream>

double line_search(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const Eigen::VectorXd & z,
  const Eigen::VectorXd & dz,
  const double max_step)
{
  double sigma = max_step;
  double oldVal = f(z);
  Eigen::VectorXd A = z+sigma*dz;
  proj_z(A);

  // find a step such that it decreases the energy function
  while (f(A) > oldVal) {
    sigma /= 2.0;
    A = z+sigma*dz;
    proj_z(A);
  }

  return sigma;
}
