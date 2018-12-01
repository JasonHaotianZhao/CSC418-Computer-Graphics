#include "catmull_rom_interpolation.h"
#include <Eigen/Dense>
#include <algorithm> // std::max
#include <iostream>

Eigen::Vector3d catmull_rom_interpolation(
  const std::vector<std::pair<double, Eigen::Vector3d> > & keyframes,
  double t)
{
  if (keyframes.size() == 0) {
  	return Eigen::Vector3d(0, 0, 0);
  }

  t = std::fmod(t, keyframes.back().first);

  int index = 0;
  while (std::max(keyframes[index].first, t) == t) {
  	index++;
  }

  // not enough information to apply catmull rom interpolation, use linear interpolation instead
  if (index-2 < 0 || index == keyframes.size()-1) {
    t = (keyframes[index].first-t)/(keyframes[index].first-keyframes[index-1].first);
  	return t*keyframes[index-1].second+(1-t)*keyframes[index].second;
  }

  // https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull–Rom_spline
  Eigen::Vector3d P0 = keyframes[index-2].second;
  Eigen::Vector3d P1 = keyframes[index-1].second;
  Eigen::Vector3d P2 = keyframes[index].second;
  Eigen::Vector3d P3 = keyframes[index+1].second;

  double t0, t1, t2, t3;
  t0 = keyframes[index-2].first;
  t1 = keyframes[index-1].first;
  t2 = keyframes[index].first;
  t3 = keyframes[index+1].first;

  t3 -= t0;
  t2 -= t0;
  t1 -= t0;
  t -= t0;
  t0 = 0.0;

  Eigen::Vector3d A1 = ((t1-t)/(t1-t0))*P0+((t-t0)/(t1-t0))*P1;
  Eigen::Vector3d A2 = ((t2-t)/(t2-t1))*P1+((t-t1)/(t2-t1))*P2;
  Eigen::Vector3d A3 = ((t3-t)/(t3-t2))*P2+((t-t2)/(t3-t2))*P3;

  Eigen::Vector3d B1 = ((t2-t)/(t2-t0))*A1+((t-t0)/(t2-t0))*A2;
  Eigen::Vector3d B2 = ((t3-t)/(t3-t1))*A2+((t-t1)/(t3-t1))*A3;

  Eigen::Vector3d C = ((t2-t)/(t2-t1))*B1+((t-t1)/(t2-t1))*B2;
  return C;
}
