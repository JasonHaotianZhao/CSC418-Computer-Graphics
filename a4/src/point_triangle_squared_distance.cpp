#include "point_triangle_squared_distance.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

double point_triangle_squared_distance(
  const Eigen::RowVector3d & query,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  Eigen::RowVector3d & bary)
{
    const Eigen::Vector3d A_(A);
    const Eigen::Vector3d B_(B);
    const Eigen::Vector3d C_(C);
    const Eigen::Vector3d query_(query);
    double alpha, beta, gamma;
    
    const auto baryFinder = [&alpha, &beta, &gamma](const Eigen::Vector3d A_, const Eigen::Vector3d B_, const Eigen::Vector3d C_, const Eigen::Vector3d point) {
        double a = A_(0), b = A_(1), c = A_(2);
        double d = B_(0), e = B_(1), f = B_(1);
        double g = C_(0), h = C_(1), i = C_(2);
        double j = point(0), k = point(1), l = point(2);
        double M = a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g);
        alpha = (j*(e*i-h*f)+k*(g*f-d*i)+l*(d*h-e*g))/M;
        beta = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;
        gamma = (f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/(-M);
    };
    
    const auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
    
    // use the normal to calculate the point's projection onto the plane
    Eigen::Vector3d normal = ((B_-A_).cross(C_-A_)).normalized();
    Eigen::Vector3d v = query_-A_;
    double dist = v.dot(normal);
    Eigen::Vector3d projection = query_-dist*normal;
    baryFinder(A_, B_, C_, projection);
    
    // the projection point lies within the triangle
    if (alpha>=0 && alpha<=1 && beta>=0 && beta<=1 && gamma>=0 && gamma <=1) {
        bary = {alpha, beta, gamma};
        return (query_-projection).squaredNorm();
    }
    
    // project point onto line segment AB and clamp the value if necessary
    double t1 = (B_-A_).dot(projection-A_);
    t1 = clamp(t1);
    Eigen::Vector3d closest1 = A_+t1*(B_-A_);
    
    // project point onto line segment AC and clamp the value if necessary
    double t2 = (C_-A_).dot(projection-A_);
    t2 = clamp(t2);
    Eigen::Vector3d closest2 = A_+t2*(C_-A_);
    
    // project point onto line segment BC and clamp the value if necessary
    double t3 = (C_-B_).dot(projection-B_);
    t3 = clamp(t3);
    Eigen::Vector3d closest3 = B_+t3*(C_-B_);
    
    Eigen::Vector3d distances;
    distances(0) = (query_-closest1).squaredNorm();
    distances(1) = (query_-closest2).squaredNorm();
    distances(2) = (query_-closest3).squaredNorm();
    
    // find out the global minimum closest point
    int result;
    distances.minCoeff(&result);
    
    Eigen::MatrixXd candidates;
    candidates.resize(3, 3);
    candidates.row(0) = Eigen::RowVector3d(closest1);
    candidates.row(1) = Eigen::RowVector3d(closest2);
    candidates.row(2) = Eigen::RowVector3d(closest3);
    
    baryFinder(A_, B_, C_, Eigen::Vector3d(candidates.row(result)));
    bary = {alpha, beta, gamma};
    return (query_-Eigen::Vector3d(candidates.row(result))).squaredNorm();
    
}
