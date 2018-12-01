#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include <iostream>

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
    if (num_recursive_calls > 5) return false;
    
    Eigen::Vector3d ret(0, 0, 0);
    int hit_id; double t; Eigen::Vector3d n;
    
    if (first_hit(ray, min_t, objects, hit_id, t, n)) {
        Eigen::Vector3d p = ray.origin+t*ray.direction;
        
        // Specular Reflection
        Ray reflectRay;
        reflectRay.origin = p;
        reflectRay.direction = reflect(ray.direction.normalized(), n);;
        Eigen::Vector3d reflectComponent;
        
        if (raycolor(reflectRay, 0.00001, objects, lights, num_recursive_calls+1, reflectComponent)) {
            const Eigen::Vector3d& km = objects[hit_id]->material->km;
            ret = ret+(km.array()*reflectComponent.array()).matrix();
        }
        
        // Blinn-Phong Shading
        Eigen::Vector3d blinnPhoneComponent = blinn_phong_shading(ray, hit_id, t, n, objects, lights);
        ret = ret+blinnPhoneComponent;
        
        rgb = ret;
        return true;
    }
    rgb = ret;
    return false;
}
