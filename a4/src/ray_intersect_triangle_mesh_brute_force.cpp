#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
    int ret_f; double ret_t = std::numeric_limits<double>::infinity(); bool found = false;
    
    // loop through all triangle faces
    for (int i=0; i<F.rows(); i++) {
        double curr_t;
        // find the closest hit
        if (ray_intersect_triangle(ray, V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2)), min_t, max_t, curr_t) && curr_t < ret_t) {
            found = true;
            ret_t = curr_t;
            ret_f = i;
        }
    }
    
    if (found) {
        hit_f = ret_f;
        hit_t = ret_t;
        return true;
    }
    return false;
    
}
