#include "reflect.h"

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
    reflected.resize(width*height*num_channels);
  
    // swap column i and column j to reflect the image
    int i = 0, j = width-1;
    while (i <= j) {
        for (int k=0; k<height; k++) {
            for (int offset=0; offset<num_channels; offset++) {
                reflected[offset+num_channels*(i+width*k)] = input[offset+num_channels*(j+width*k)];
                reflected[offset+num_channels*(j+width*k)] = input[offset+num_channels*(i+width*k)];
            }
        }
        i++;
        j--;
    }
}
