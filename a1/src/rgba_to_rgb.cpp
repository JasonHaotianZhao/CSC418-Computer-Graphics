#include "rgba_to_rgb.h"

void rgba_to_rgb(
  const std::vector<unsigned char> & rgba,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
    rgb.resize(height*width*3);
    
    // extract rgb information from rgba input, get rid of the aplha channel
    int write = 0;
    int read = 0;
    for (int i=0; i<width*height; i++) {
        for (int j=0; j<3; j++) {
            rgb[write++] = rgba[read++];
        }
        read++;
    }
}
