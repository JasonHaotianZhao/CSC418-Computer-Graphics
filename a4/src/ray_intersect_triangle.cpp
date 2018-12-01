#include "ray_intersect_triangle.h"

bool ray_intersect_triangle(
  const Ray & ray,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  const double min_t,
  const double max_t,
  double & t)
{
    // use Cramer's rule to solve for t, gamma, beta
    double a, b, c, d, e, f, g, h, i, j, k, l;
    a = A(0)-B(0);
    d = A(0)-C(0);
    g = ray.direction(0);
    b = A(1)-B(1);
    e = A(1)-C(1);
    h = ray.direction(1);
    c = A(2)-B(2);
    f = A(2)-C(2);
    i = ray.direction(2);
    j = A(0)-ray.origin(0);
    k = A(1)-ray.origin(1);
    l = A(2)-ray.origin(2);

    double t_, gamma, beta, M;
    M = a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g);
    t_ = (f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/(-M);
    if (t_ < min_t || t_ >= max_t) {
        return false;
    }

    gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
    if (gamma < 0 || gamma > 1) {
        return false;
    }

    beta = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
    if (beta < 0 || beta > 1-gamma) {
        return false;
    }

    t = t_;
    return true;
}

