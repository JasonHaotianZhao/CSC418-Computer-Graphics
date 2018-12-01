#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <string>
#include <vector>
#include <iostream>

// https://rosettacode.org/wiki/Catmull–Clark_subdivision_surface
void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
    if (num_iters == 0) {
        return;
    }
    
    SV.resize(0, 3);
    SF.resize(0, 4);
    
    // construct useful information map beforehand to speed up
    // the face point for each face
    std::unordered_map<int, Eigen::RowVector3d> facePoint;
    for (int i=0; i<F.rows(); i++) {
        Eigen::RowVector3d point(0, 0, 0);
        for (int j=0; j<F.cols(); j++) {
            point += V.row(F(i, j));
        }
        facePoint[i] = point/4.0;
    }
    
    // the face which connects to the given edge
    std::unordered_map<std::string, std::vector<int>> edge2Face;
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            std::string key1 = std::to_string(F(i, j))+"->"+std::to_string(F(i, (j+1)%F.cols()));
            std::string key2 = std::to_string(F(i, (j+1)%F.cols()))+"->"+std::to_string(F(i, j));
            edge2Face[key1].push_back(i);
            edge2Face[key2].push_back(i);
        }
    }
    
    // the face which connects to the given point
    std::unordered_map<int, std::vector<int>> point2Face;
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            point2Face[F(i, j)].push_back(i);
        }
    }
    
    // the neighbor point which connects to the given point
    std::unordered_map<int, std::vector<int>> pointNeighbors;
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            pointNeighbors[F(i, j)].push_back(F(i, (j+1)%F.cols()));
        }
    }
    
    // the edge point for a given edge
    const auto edgePoint = [&V, &F, &edge2Face, &facePoint](const int a, const int b) {
        Eigen::RowVector3d edgeCenter = (V.row(a)+V.row(b))/2.0;
        Eigen::RowVector3d facePointCenter(0, 0, 0);
        std::string key = std::to_string(a)+"->"+std::to_string(b);
        for (const int f : edge2Face[key]) {
            facePointCenter += facePoint[f];
        }
        facePointCenter /= 2.0;
        return (facePointCenter+edgeCenter)/2.0;
    };
    
    // find the new coordinate for point p
    const auto updatePoint = [&point2Face, &facePoint, &pointNeighbors, &V, &F](const int p) {
        Eigen::RowVector3d oldCoor = V.row(p);
        
        Eigen::RowVector3d avgFacePoint(0, 0, 0);
        for (const auto f : point2Face[p]) {
            avgFacePoint += facePoint[f];
        }
        avgFacePoint /= (double)point2Face[p].size();
        
        Eigen::RowVector3d avgMidEdge(0, 0, 0);
        for (const auto neighbor : pointNeighbors[p]) {
            avgMidEdge += (V.row(p)+V.row(neighbor))/2.0;
        }
        avgMidEdge /= (double)pointNeighbors[p].size();
        
        double n = point2Face[p].size();
        double m1 = (n-3)/n;
        double m2 = 1/n;
        double m3 = 2/n;
        return m1*oldCoor+m2*avgFacePoint+m3*avgMidEdge;
    };
    
    // add a new face into SV and SF
    const auto addNewFace = [&SV, &SF](const Eigen::RowVector3d first, const Eigen::RowVector3d second, const Eigen::RowVector3d third, const Eigen::RowVector3d fourth) {
        Eigen::RowVector4i idxs(-1, -1, -1, -1);
        Eigen::MatrixXd points;
        points.resize(4, 3);
        points << first,
                  second,
                  third,
                  fourth;
        for (int i=0; i<points.rows(); i++) {
            for (int j=0; j<SV.rows(); j++) {
                if (points.row(i).isApprox(SV.row(j))) {
                    idxs(i) = j;
                    break;
                }
            }
            if (idxs(i) == -1) {
                SV.conservativeResize(SV.rows()+1, SV.cols());
                SV.row(SV.rows()-1) = points.row(i);
                idxs(i) = SV.rows()-1;
            }
        }
        SF.conservativeResize(SF.rows()+1, SF.cols());
        SF.row(SF.rows()-1) = idxs;
    };
    
    // for each face, futher divides it into four sub faces
    for (int i=0; i<F.rows(); i++) {
        for (int j=0; j<F.cols(); j++) {
            Eigen::RowVector3d first = updatePoint(F(i, j));
            Eigen::RowVector3d second = edgePoint(F(i, j), F(i, (j+1)%F.cols()));
            Eigen::RowVector3d third = facePoint[i];
            Eigen::RowVector3d fourth = edgePoint(F(i, j), F(i, ((j-1)+F.cols())%F.cols()));
            addNewFace(first, second, third, fourth);
        }
    }
    
    catmull_clark(Eigen::MatrixXd(SV), Eigen::MatrixXi(SF), num_iters-1, SV, SF);
}

