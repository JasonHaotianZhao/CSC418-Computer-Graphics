#include "insert_triangle_into_box.h"
#include <iostream>

void insert_triangle_into_box(
  const Eigen::RowVector3d & a,
  const Eigen::RowVector3d & b,
  const Eigen::RowVector3d & c,
  BoundingBox & B)
{
    // find out global maximum/minimum x, y, z coordinate values
    for (int i=0; i<3; i++) {
        B.min_corner(i) = fmin(fmin(a(i), b(i)), fmin(c(i), B.min_corner(i)));
        B.max_corner(i) = fmax(fmax(a(i), b(i)), fmax(c(i), B.max_corner(i)));
    }
}


