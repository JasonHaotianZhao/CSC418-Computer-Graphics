#include "forward_kinematics.h"
#include "euler_angles_to_transform.h"
#include <functional> // std::function
#include <iostream>

void forward_kinematics(
  const Skeleton & skeleton,
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
  T.resize(skeleton.size(),Eigen::Affine3d::Identity());

  // helper function to calculate the fk for a certain bone
  std::function<Eigen::Affine3d(int)> func; 
  func = [&skeleton, &func](int index)->Eigen::Affine3d {
  	if (skeleton[index].parent_index == -1) {
  		return Eigen::Affine3d::Identity();
  	}

  	Eigen::Affine3d parent = func(skeleton[index].parent_index);
  	return parent*(skeleton[index].rest_T)*
  		     euler_angles_to_transform(skeleton[index].xzx)*
  		     ((skeleton[index].rest_T).inverse());
  };

  // calculate fk for each bone
  for (int i=0; i<skeleton.size(); i++) {
  	T[i] = func(i);
  }
}
