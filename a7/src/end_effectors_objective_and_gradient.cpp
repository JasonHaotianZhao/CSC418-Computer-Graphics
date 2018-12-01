#include "end_effectors_objective_and_gradient.h"
#include "transformed_tips.h"
#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include <iostream>

void end_effectors_objective_and_gradient(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  const Eigen::VectorXd & xb0,
  std::function<double(const Eigen::VectorXd &)> & f,
  std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  std::function<void(Eigen::VectorXd &)> & proj_z)
{
  // function to calculate the energy function based on a set of euler angles
  f = [&](const Eigen::VectorXd & A)->double
  {
    Skeleton s = copy_skeleton_at(skeleton, A);
    Eigen::VectorXd currPos = transformed_tips(s, b);
    double ret = 0.0;
    for (int i=0; i<b.size(); i++) {
      ret += (Eigen::Vector3d(currPos[3*i], currPos[3*i+1], currPos[3*i+2])-
              Eigen::Vector3d(xb0[3*i], xb0[3*i+1], xb0[3*i+2])).squaredNorm();
    }
    return ret;
  };

  // helper function to calculate the energy function based on a set of end effector positions
  const auto helper = [&](const Eigen::VectorXd & X)->double
  {
    double ret = 0.0;
    for (int i=0; i<X.size()/3; i++) {
      ret += (Eigen::Vector3d(X[3*i], X[3*i+1], X[3*i+2])-
              Eigen::Vector3d(xb0[3*i], xb0[3*i+1], xb0[3*i+2])).squaredNorm();
    }
    return ret;
  };

  // function to calculate the gradient of energy function at a certain point
  grad_f = [&](const Eigen::VectorXd & A)->Eigen::VectorXd
  {
    // get the jacobian matrix
    Eigen::MatrixXd J;
    Skeleton s = copy_skeleton_at(skeleton, A);
    kinematics_jacobian(s, b, J);
    
    // calculate the dE/dX column vector
    Eigen::VectorXd dEdX = Eigen::VectorXd::Zero(b.size()*3);
    Eigen::VectorXd X = transformed_tips(s, b);
    double oldVal = helper(X);

    for (int i=0; i<dEdX.size(); i++) {
      Eigen::VectorXd copy = X;
      copy[i] += 10e-7;
      dEdX[i] = (helper(copy)-oldVal)/(10e-7);
    }
    return (-1.0)*(J.transpose())*dEdX;
  };

  // project constraints onto the input euler angles 
  proj_z = [&](Eigen::VectorXd & A)
  {
    for (int i=0; i<skeleton.size(); i++) {
      A[3*i] = std::max(skeleton[i].xzx_min[0], std::min(skeleton[i].xzx_max[0], A[3*i]));
      A[3*i+1] = std::max(skeleton[i].xzx_min[1], std::min(skeleton[i].xzx_max[1], A[3*i+1]));
      A[3*i+2] = std::max(skeleton[i].xzx_min[2], std::min(skeleton[i].xzx_max[2], A[3*i+2]));
    }
  };
}
