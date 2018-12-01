#include "copy_skeleton_at.h"
#include <iostream>
Skeleton copy_skeleton_at(
  const Skeleton & skeleton, 
  const Eigen::VectorXd & A)
{
  // create a copy of the skeleton with specified euler angles
  Skeleton copy = skeleton;
  for (int i=0; i<skeleton.size(); i++) {
  	copy[i].xzx = Eigen::Vector3d(A[3*i], A[3*i+1], A[3*i+2]);
  }
  return copy;
}
