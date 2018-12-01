#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>
#include <Eigen/Geometry>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
    N = Eigen::MatrixXd::Zero(F.rows()*3,3);
    
    std::vector<std::vector<int>> VF;
    vertex_triangle_adjacency(F, V.rows(), VF);

    const auto area = [&V, &F](const int f) {
        Eigen::RowVector3d A = V.row(F(f, 0));
        Eigen::RowVector3d B = V.row(F(f, 1));
        Eigen::RowVector3d C = V.row(F(f, 2));
        return ((B-A).cross(C-A)).norm()/2.0;
    };
    
    const auto faceNormalFunc = [&V, &F](const int f) {
        Eigen::RowVector3d A = V.row(F(f, 0));
        Eigen::RowVector3d B = V.row(F(f, 1));
        Eigen::RowVector3d C = V.row(F(f, 2));
        return ((B-A).cross(C-A)).normalized();
    };
    
    // loop through all the corners
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            Eigen::RowVector3d faceNormal = faceNormalFunc(i);
            Eigen::RowVector3d vertexNormal(0, 0, 0);
            double totalArea = 0;
            for (const auto f : VF[F(i, j)]) {
                // ignore triangle's whose normal is too different from the corner's face's normal
                if (faceNormal.dot(faceNormalFunc(f)) > cos(corner_threshold*M_PI/180.0)) {
                    totalArea += area(f);
                    vertexNormal += triangle_area_normal(V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)));
                }
            }
            N.row(i*3+j) = (vertexNormal/totalArea).normalized();
        }
    }
}
