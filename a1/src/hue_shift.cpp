#include "hue_shift.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void hue_shift(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double shift,
  std::vector<unsigned char> & shifted)
{
    shifted.resize(rgb.size());
    
    int read = 0;
    int write = 0;
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            double h, s, v;
            double r = (double)(rgb[read++]) / 255.0;
            double g = (double)(rgb[read++]) / 255.0;
            double b = (double)(rgb[read++]) / 255.0;
            // convert rgb to hsv
            rgb_to_hsv(r, g, b, h, s, v);
            // apply shift on hue value;
            h += shift;
            // convert back to rgb
            hsv_to_rgb(h, s, v, r, g, b);
            // write result back to output 1d vector
            shifted[write++] = (unsigned char)((int)(r * 255.0));
            shifted[write++] = (unsigned char)((int)(g * 255.0));
            shifted[write++] = (unsigned char)((int)(b * 255.0));
        }
    }
}
