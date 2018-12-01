// Generate a pseudorandom unit 3D vector
// 
// Inputs:
//   seed  3D seed
// Returns psuedorandom, unit 3D vector drawn from uniform distribution over
// the unit sphere (assuming random2 is uniform over [0,1]²).
//
// expects: random2.glsl, PI.glsl

vec3 random_direction( vec3 seed)
{
    // generate a uniform distribution 2d vector as uv coordinate
    vec2 uv = random2(seed);
    
    // map uv coordinate to spherical coordinate
    float theta = -2*M_PI*uv.x;
    float phi = acos(2*uv.y-1);
    
    // map spherical coordinate to cartesian coordinate
    float x = 1.0*sin(theta)*cos(phi);
    float y = 1.0*sin(theta)*sin(phi);
    float z = 1.0*cos(theta);
    
    // return unit vector
    return normalize(vec3(x, y, z));
}
