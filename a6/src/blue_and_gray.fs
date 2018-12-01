// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool is_moon;
// Outputs:
out vec3 color;
void main()
{
    // set moon to gray color
    if (is_moon) {
        color = vec3(0.5, 0.5, 0.5);
        return;
    }
    // set planet to blue color
    color = vec3(0, 0, 1);
}
