#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
    gray.resize(height*width);
  
    int read = 0;
    int write = 0;
    for (int i=0; i<height*width; i++) {
        //  i = 0.2126 r + 0.7152 g + 0.0722 b
        // convert rgb to gray scale value using the above formula
        double red = (double)rgb[read++];
        double green = (double)rgb[read++];
        double blue = (double)rgb[read++];
        double grayScale = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
        gray[write++] = (unsigned char)((int)grayScale);
    }
}


