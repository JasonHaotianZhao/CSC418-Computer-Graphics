#include "over.h"

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
    C.resize(A.size());
    
    int read1 = 0, read2 = 0, write = 0;
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            double A_r = (double)A[read1++] / 255.0;
            double A_g = (double)A[read1++] / 255.0;
            double A_b = (double)A[read1++] / 255.0;
            double A_a = (double)A[read1++] / 255.0;
            
            double B_r = (double)B[read2++] / 255.0;
            double B_g = (double)B[read2++] / 255.0;
            double B_b = (double)B[read2++] / 255.0;
            double B_a = (double)B[read2++] / 255.0;
            
            // Porter-Duff Over operator formula
            // https://en.wikipedia.org/wiki/Alpha_compositing
            double a = 1.0 - (1.0 - A_a) * (1.0 - B_a);
            double r = ((A_a * A_r) + (1.0 - A_a) * B_a * B_r) / a;
            double g = ((A_a * A_g) + (1.0 - A_a) * B_a * B_g) / a;
            double b = ((A_a * A_b) + (1.0 - A_a) * B_a * B_b) / a;
            
            // formula found in textbook
            /*double r = A_a * A_r + (1 - A_a) * B_r;
            double g = A_a * A_g + (1 - A_a) * B_g;
            double b = A_a * A_b + (1 - A_a) * B_b;*/

            C[write++] = (unsigned char)((int)(r * 255.0));
            C[write++] = (unsigned char)((int)(g * 255.0));
            C[write++] = (unsigned char)((int)(b * 255.0));
            C[write++] = (unsigned char)((int)(a * 255.0));
        }
    }
}
