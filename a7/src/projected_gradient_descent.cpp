#include "projected_gradient_descent.h"
#include "line_search.h"
#include <iostream>

void projected_gradient_descent(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const int max_iters,
  Eigen::VectorXd & z)
{
  Eigen::VectorXd z_ = z;
  Eigen::VectorXd dz = grad_f(z_);
  // use exponential search to find appropriate step 
	double sigma = line_search(f, proj_z, z, dz, 10000);
	z = z+sigma*dz;
  proj_z(z);
}
