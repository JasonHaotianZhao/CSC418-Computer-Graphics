#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
    Eigen::Vector3d ret(0, 0, 0);
    
    // ambient light shading
    const Eigen::Vector3d& ka = objects[hit_id]->material->ka;
    ret = ret+(ka.array()*Eigen::Vector3d(1, 1, 1).array()).matrix();
    
    Eigen::Vector3d p = ray.origin+t*ray.direction;
    
    for (const auto& light : lights) {
        double max_t;
        Eigen::Vector3d l;
        light->direction(p, l, max_t);
        
        Ray lightRay;
        lightRay.origin = p;
        lightRay.direction = l;
        
        int temp1; double shadowT; Eigen::Vector3d temp2;
        // check if the point is in shadow or not
        if (!first_hit(lightRay, 0.00001, objects, temp1, shadowT, temp2) || shadowT >= max_t) {
            l = l.normalized();
            const Eigen::Vector3d v = (-ray.direction).normalized();
            const Eigen::Vector3d h = (v+l).normalized();
            const double phong_exponent = objects[hit_id]->material->phong_exponent;
            const Eigen::Vector3d& I = light->I;
            const Eigen::Vector3d& kd = objects[hit_id]->material->kd;
            const Eigen::Vector3d& ks = objects[hit_id]->material->ks;
            // diffuse and specular contributions
            ret = ret+(kd.array()*I.array()*fmax(n.dot(l), 0)).matrix()+(ks.array()*I.array()*pow(fmax(n.dot(h), 0), phong_exponent)).matrix();
        }
    }
    return ret;
}
