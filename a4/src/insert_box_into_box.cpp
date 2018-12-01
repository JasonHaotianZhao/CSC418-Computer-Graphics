#include "insert_box_into_box.h"
#include <iostream>

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
    // find out global minimum/maximum x, y, z coordinate values
    for (int i=0; i<3; i++) {
        B.min_corner(i) = fmin(B.min_corner(i), A.min_corner(i));
        B.max_corner(i) = fmax(B.max_corner(i), A.max_corner(i));
    }
}

