// Given a 3d position as a seed, compute a smooth procedural noise
// value: "Perlin Noise", also known as "Gradient noise".
//
// Inputs:
//   st  3D seed
// Returns a smooth value between (-1,1)
//
// expects: random_direction, smooth_step

float perlin_noise( vec3 st) 
{
    // get the surrounding unit cube around the input point
    vec3 i = floor(st);
    vec3 f = fract(st);
    
    // get the 8 corners' coordinates of the unit cube
    vec3 xyz = i + vec3(0, 0, 0);
    vec3 xyz_ = i + vec3(0, 0, 1);
    vec3 xy_z = i + vec3(0, 1, 0);
    vec3 xy_z_ = i + vec3(0, 1, 1);
    vec3 x_yz =  i + vec3(1, 0, 0);
    vec3 x_yz_ = i + vec3(1, 0, 1);
    vec3 x_y_z = i + vec3(1, 1, 0);
    vec3 x_y_z_ = i + vec3(1, 1, 1);
    
    // get the gradient vector at each corner
    vec3 g1 = random_direction(xyz);
    vec3 g2 = random_direction(xyz_);
    vec3 g3 = random_direction(xy_z);
    vec3 g4 = random_direction(xy_z_);
    vec3 g5 = random_direction(x_yz);
    vec3 g6 = random_direction(x_yz_);
    vec3 g7 = random_direction(x_y_z);
    vec3 g8 = random_direction(x_y_z_);
    
    // smooth f value and later it will be use to interpolate the result
    vec3 f_ = smooth_step(f);
    
    // get the distance vector between the input point and the 8 corners
    vec3 d1 = f - vec3(0, 0, 0);
    vec3 d2 = f - vec3(0, 0, 1);
    vec3 d3 = f - vec3(0, 1, 0);
    vec3 d4 = f - vec3(0, 1, 1);
    vec3 d5 = f - vec3(1, 0, 0);
    vec3 d6 = f - vec3(1, 0, 1);
    vec3 d7 = f - vec3(1, 1, 0);
    vec3 d8 = f - vec3(1, 1, 1);
    
    // calculate the dot product between the gradient vector and the distance vector
    float dot1 = dot(g1, d1);
    float dot2 = dot(g2, d2);
    float dot3 = dot(g3, d3);
    float dot4 = dot(g4, d4);
    float dot5 = dot(g5, d5);
    float dot6 = dot(g6, d6);
    float dot7 = dot(g7, d7);
    float dot8 = dot(g8, d8);
    
    // trilinear interpolation
    float ret =  mix( mix( mix( dot( d1, g1 ),
                                dot( d2, g2 ), f_.x ),
                           mix( dot( d3, g3 ),
                                dot( d4, g4 ), f_.x), f_.y ),
                      mix( mix( dot( d5, g5 ),
                                dot( d6, g6 ), f_.x),
                           mix( dot( d7, g7 ),
                                dot( d8, g8 ), f_.x), f_.y), f_.z );
    
    // map the return value into range [-1.0, 1.0]
    return -1.0+2.0*ret;
}

