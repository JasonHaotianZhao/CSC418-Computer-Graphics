#include "demosaic.h"

void demosaic(
  const std::vector<unsigned char> & bayer,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
    rgb.resize(width*height*3);
    
    std::vector<std::vector<int>> directions = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    
    std::vector<std::vector<char>> bayerPattern = {{'g', 'b'}, {'r', 'g'}};
    
    int write = 0;
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            double redSum = 0, greenSum = 0, blueSum = 0;
            int redCount = 0, greenCount = 0, blueCount = 0;
            
            // loop through all eight neighbors of the current pixel
            for (auto direction : directions) {
                int neighborX = i + direction[0];
                int neighborY = j + direction[1];
                // make sure the coordinate of the neighbor is valid
                if (neighborX >= 0 && neighborX < height && neighborY >= 0 && neighborY < width) {
                    // the bayer color of the neighbor
                    int index1 = neighborX % 2;
                    int index2 = neighborY % 2;
                    char color = bayerPattern[index1][index2];
                    if (color == 'r') {
                        redSum += bayer[neighborY+width*neighborX];
                        redCount++;
                    } else if (color == 'g') {
                        greenSum += bayer[neighborY+width*neighborX];
                        greenCount++;
                    } else {
                        blueSum += bayer[neighborY+width*neighborX];
                        blueCount++;
                    }
                }
            }
            
            int index1 = i % 2;
            int index2 = j % 2;
            char color = bayerPattern[index1][index2];
            
            // calculate the average rgb value from the neighbors
            unsigned char r = (unsigned char)((int)(redSum / redCount));
            unsigned char g = (unsigned char)((int)(greenSum / greenCount));
            unsigned char b = (unsigned char)((int)(blueSum / blueCount));
            
            // "use the exact color sample when it's available and average available neighbors (in all 8 directions) to fill in missing colors."
            if (color == 'r') {
                rgb[write++] = bayer[j+width*i];
                rgb[write++] = g;
                rgb[write++] = b;
            } else if (color == 'g') {
                rgb[write++] = r;
                rgb[write++] = bayer[j+width*i];
                rgb[write++] = b;
            } else {
                rgb[write++] = r;
                rgb[write++] = g;
                rgb[write++] = bayer[j+width*i];
            }
        }
    }
    
}
