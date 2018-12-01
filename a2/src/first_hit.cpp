#include "first_hit.h"
#include <limits>

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
    bool found = false;
    double ret_t = std::numeric_limits<double>::max();
    double ret_id;
    Eigen::Vector3d ret_normal;
    
    // loop through all objects in the scene
    for (int i=0; i<objects.size(); i++) {
        double curr_t;
        Eigen::Vector3d curr_normal;
        // choose the closest intersection
        if (objects[i]->intersect(ray, min_t, curr_t, curr_normal) && curr_t < ret_t) {
            found = true;
            ret_t = curr_t;
            ret_normal = curr_normal;
            ret_id = i;
        }
    }
    
    if (found) {
        t = ret_t;
        n = ret_normal;
        hit_id = ret_id;
        return true;
    }
    return false;
}

