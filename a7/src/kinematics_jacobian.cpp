#include "kinematics_jacobian.h"
#include "transformed_tips.h"
#include <iostream>

void kinematics_jacobian(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  Eigen::MatrixXd & J)
{
  J = Eigen::MatrixXd::Zero(b.size()*3,skeleton.size()*3);

  Eigen::VectorXd oldPos = transformed_tips(skeleton, b);

  for (int i=0; i<skeleton.size(); i++) {
  	for (int j=0; j<3; j++) {
      Skeleton copy = skeleton;

      // apply finite differencing trick to get dX/da
      copy[i].xzx[j] += 10e-7;
      Eigen::VectorXd newPos = transformed_tips(copy, b);

  		for (int k=0; k<b.size()*3; k++) {
  			J(k, 3*i+j) = (newPos[k]-oldPos[k])/(10e-7);
  		}
  	}
  }
}
