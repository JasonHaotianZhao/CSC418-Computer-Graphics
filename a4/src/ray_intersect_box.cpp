#include "ray_intersect_box.h"
#include <iostream>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t,
  double & t)
{
    double a = 1/ray.direction(0);
    double tXMin, tXMax;
    if (a >= 0) {
        tXMin = a*(box.min_corner(0)-ray.origin(0));
        tXMax = a*(box.max_corner(0)-ray.origin(0));
    } else {
        tXMin = a*(box.max_corner(0)-ray.origin(0));
        tXMax = a*(box.min_corner(0)-ray.origin(0));
    }
    
    double b = 1/ray.direction(1);
    double tYMin, tYMax;
    if (b >= 0) {
        tYMin = b*(box.min_corner(1)-ray.origin(1));
        tYMax = b*(box.max_corner(1)-ray.origin(1));
    } else {
        tYMin = b*(box.max_corner(1)-ray.origin(1));
        tYMax = b*(box.min_corner(1)-ray.origin(1));
    }
    
    double c = 1/ray.direction(2);
    double tZMin, tZMax;
    if (c >= 0) {
        tZMin = c*(box.min_corner(2)-ray.origin(2));
        tZMax = c*(box.max_corner(2)-ray.origin(2));
    } else {
        tZMin = c*(box.max_corner(2)-ray.origin(2));
        tZMax = c*(box.min_corner(2)-ray.origin(2));
    }
    
    double tMin = fmax(fmax(tXMin, tYMin), tZMin);
    double tMax = fmin(fmin(tXMax, tYMax), tZMax);
    if (tMax < tMin) {
        return false;
    }
    
    double intersectionMin = fmax(min_t, tMin);
    double intersectionMax = fmin(max_t, tMax);
    if (intersectionMax < intersectionMin) {
        return false;
    }
    return true;
}
