#include "per_vertex_normals.h"
#include "triangle_area_normal.h"
#include <unordered_map>
#include <vector>
#include <Eigen/Geometry>

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
    N = Eigen::MatrixXd::Zero(V.rows(), 3);
    
    // the face which connects to the given vertex
    std::unordered_map<int, std::vector<int>> vertex2face;
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            vertex2face[F(i, j)].push_back(i);
        }
    }
    
    const auto area = [&V, &F](const int f) {
        Eigen::RowVector3d A = V.row(F(f, 0));
        Eigen::RowVector3d B = V.row(F(f, 1));
        Eigen::RowVector3d C = V.row(F(f, 2));
        return ((B-A).cross(C-A)).norm()/2.0;
    };
    
    // the normal at the vertex is defined by the weighted sum of the face normal it connects to
    for (int i=0; i<V.rows(); i++) {
        double totalArea = 0;
        for (const auto f : vertex2face[i]) {
            totalArea += area(f);
            N.row(i) += triangle_area_normal(V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)));
        }
        N.row(i) = (N.row(i)/totalArea).normalized();
    }
}
