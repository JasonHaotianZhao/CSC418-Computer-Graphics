#include "linear_blend_skinning.h"
#include <algorithm>
#include <iostream>

void linear_blend_skinning(
  const Eigen::MatrixXd & V,
  const Skeleton & skeleton,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T,
  const Eigen::MatrixXd & W,
  Eigen::MatrixXd & U)
{
  U.resize(V.size(), 3);
  
  // calculate contribution of each bone transformation to the a certain position
  for (int v=0; v<V.rows(); v++) {
  	Eigen::Vector4d pose = Eigen::Vector4d::Zero();
  	for (int i=0; i<skeleton.size(); i++) {
  		if (skeleton[i].weight_index == -1) {
  			continue;
  		}
  		pose += T[i]*Eigen::Vector4d(V(v, 0), V(v, 1), V(v, 2), 1)*W(v, skeleton[i].weight_index);
  	}
  	U(v, 0) = pose[0]/pose[3];
  	U(v, 1) = pose[1]/pose[3];
  	U(v, 2) = pose[2]/pose[3];
  }
}
