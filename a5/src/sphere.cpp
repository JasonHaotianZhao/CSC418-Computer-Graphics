#include "sphere.h"
#include <iostream>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
    V.resize((num_faces_u+1)*(num_faces_v+1), 3);
    UV.resize((num_faces_u+1)*(num_faces_v+1), 2);
    NV.resize((num_faces_u+1)*(num_faces_v+1), 3);
    F.resize(num_faces_u*num_faces_v, 4);
    UF.resize(num_faces_u*num_faces_v, 4);
    NF.resize(num_faces_u*num_faces_v, 4);
    
    // https://stackoverflow.com/questions/41661369/smoothly-mapping-a-2d-uv-point-onto-a-3d-xyz-sphere
    double x, y, z;
    const auto uv2xyz = [&x, &y, &z](const double u, const double v) {
        const double lat = v*M_PI-M_PI/2.0;
        const double lon = u*2.0*M_PI-M_PI;
        x = cos(lat)*cos(lon);
        y = cos(lat)*sin(lon);
        z = sin(lat);
    };
    
    const double uLen = 1.0/num_faces_u;
    const double vLen = 1.0/num_faces_v;
    
    // loop through all the points in the texture coordinates
    int index = 0;
    for (int i=0; i<=num_faces_v; i++) {
        for (int j=0; j<=num_faces_u; j++) {
            const double u = j*uLen;
            const double v = i*vLen;
            uv2xyz(u, v);
            V.row(index) = Eigen::RowVector3d(x, y, z);
            UV.row(index) = Eigen::RowVector2d(u, v);
            NV.row(index) = Eigen::RowVector3d(x, y, z);
            index++;
        }
    }
    
    // loop through all the faces and find the 3d location, uv coordinate and normal for each vertices belong to the face
    index = 0;
    for (int i=0; i<num_faces_v; i++) {
        for (int j=0; j<num_faces_u; j++) {
            const int idx1 = i*(num_faces_u+1)+j;
            const int idx2 = idx1+1;
            const int idx3 = (i+1)*(num_faces_u+1)+(j+1);
            const int idx4 = idx3-1;
            F.row(index) = Eigen::RowVector4i(idx1, idx2, idx3, idx4);
            UF.row(index) = Eigen::RowVector4i(idx1, idx2, idx3, idx4);
            NF.row(index) = Eigen::RowVector4i(idx1, idx2, idx3, idx4);
            index++;
        }
    }
}
