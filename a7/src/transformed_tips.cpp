#include "transformed_tips.h"
#include "forward_kinematics.h"
#include <iostream>

Eigen::VectorXd transformed_tips(
  const Skeleton & skeleton, 
  const Eigen::VectorXi & b)
{
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d>> T;
  forward_kinematics(skeleton, T);

  // based on the fk transformation for each bone, calculate the desired tip positions
  Eigen::VectorXd ret = Eigen::VectorXd(3*b.size());
  for (int i=0; i<b.rows(); i++) {
  	int index = b[i];
  	Eigen::Vector4d pos = T[index]*
  					  	          skeleton[index].rest_T*
  					  	          Eigen::Vector4d(skeleton[index].length, 0, 0, 1);

    ret[i*3] = pos[0]/pos[3],
    ret[i*3+1] = pos[1]/pos[3],
    ret[i*3+2] = pos[2]/pos[3];
  }

  return ret;
}
