#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
    bayer.resize(width*height);
  
    // the bayer mosaic pattern used:
    // g b
    // r g
    std::vector<std::vector<char>> bayerPattern = {{'g', 'b'}, {'r', 'g'}};
    
    int write = 0;
    for (int i=0; i<height; i++) {
        int index1 = i % 2;
        for (int j=0; j<width; j++) {
            int index2 = j % 2;
            // what is the bayer color for the pixel I'm currently working on?
            char color = bayerPattern[index1][index2];
            
            // based on the bayer color, write corresponding color to the result
            if (color == 'r') {
                bayer[write++] = rgb[0+3*(j+width*i)];
            } else if (color == 'g') {
                bayer[write++] = rgb[1+3*(j+width*i)];
            } else {
                bayer[write++] = rgb[2+3*(j+width*i)];
            }
        }
    }
}
