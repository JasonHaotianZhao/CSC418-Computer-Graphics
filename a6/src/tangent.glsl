// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit tangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{    
    float theta = acos(N.z);
    float phi = atan(N.y/N.x);

    // tangent vector at a point on the unit sphere
    T = normalize(vec3(cos(theta)*cos(phi)-sin(phi)*sin(theta),
                       sin(theta)*cos(phi)+sin(phi)*cos(theta),
                       -sin(phi)));

    // bi-tangent vector at the point
    B = normalize(cross(T, N));
}
