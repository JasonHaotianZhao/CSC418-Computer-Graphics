// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;
// expects: PI, blinn_phong
void main()
{
    // create a light source and get its coordinate in camera space
    float theta = (2.0*M_PI/8.0)*animation_seconds;
    vec4 light = view*vec4(3*cos(theta), 3, 3*sin(theta), 1);
    vec3 lightPoint = vec3(light.x/light.w, light.y/light.w, light.z/light.w);
    
    // get the view point's coordinate in camera space
    vec3 viewPoint = vec3(view_pos_fs_in.x/view_pos_fs_in.w, view_pos_fs_in.y/view_pos_fs_in.w, view_pos_fs_in.z/view_pos_fs_in.w);
    
    // shade planet and moon accordingly
    if (!is_moon) {
        color = blinn_phong(vec3(0.02, 0.03, 0.08),
                            vec3(0.2, 0.3, 0.8),
                            vec3(0.1, 0.1, 0.1),
                            1000,
                            normalize(normal_fs_in),
                            normalize(-viewPoint),
                            normalize(lightPoint-viewPoint));
    } else {
        color = blinn_phong(vec3(0.02, 0.02, 0.02),
                            vec3(0.8, 0.8, 0.8),
                            vec3(0.1, 0.1, 0.1),
                            2000,
                            normalize(normal_fs_in),
                            normalize(-viewPoint),
                            normalize(lightPoint-viewPoint));
    }
    
    
    
}
