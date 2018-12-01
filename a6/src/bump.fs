// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
    // create a light source and get its coordinate in camera space
    float theta = (2.0*M_PI/8.0)*animation_seconds;
    vec4 light = view*vec4(3*cos(theta), 3, 3*sin(theta), 1);
    vec3 lightPoint = vec3(light.x/light.w, light.y/light.w, light.z/light.w);
    
    // calculate tangent and bi-tangent vector
    vec3 T, B;
    tangent(sphere_fs_in, T, B);
    
    mat4 model = model(is_moon, animation_seconds);
    
    // calculate new point position and normal
    vec3 newPos = bump_position(is_moon, sphere_fs_in);
    vec3 newNormal = normalize(cross((bump_position(is_moon, sphere_fs_in+0.0001*T)-newPos)/0.0001,
                                     (bump_position(is_moon, sphere_fs_in+0.0001*B)-newPos)/0.0001));
    
    // transform position and normal into camera space
    vec4 viewPoint_ = view*model*vec4(newPos, 1.0);
    vec3 viewPoint = vec3(viewPoint_.x/viewPoint_.w, viewPoint_.y/viewPoint_.w, viewPoint_.z/viewPoint_.w);
    vec3 viewNormal = (transpose(inverse(view))*transpose(inverse(model))*vec4(newNormal, 1.0)).xyz;
    
    // shade planet and moon accordingly
    if (!is_moon) {
        color = blinn_phong(vec3(0.02, 0.03, 0.08),
                            vec3(0.2, 0.3, 0.8),
                            vec3(0.1, 0.1, 0.1),
                            1000,
                            viewNormal,
                            normalize(-viewPoint),
                            normalize(lightPoint-viewPoint));

    } else {
        color = blinn_phong(vec3(0.02, 0.02, 0.02),
                            vec3(0.8, 0.8, 0.8),
                            vec3(0.1, 0.1, 0.1),
                            2000,
                            viewNormal,
                            normalize(-viewPoint),
                            normalize(lightPoint-viewPoint));
    }
}
