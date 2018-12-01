#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    Eigen::Vector3d e = ray.origin;
    Eigen::Vector3d d = ray.direction;
    
    // ray and plane are parallel
    if (d.dot(normal) == 0) {
        return false;
    }
    
    // ray and plane intersect at a single point
    double ret = (point-e).dot(normal)/d.dot(normal);
    if (ret >= min_t) {
        t = ret;
        n = normal;
        return true;
    }
    return false;
}

