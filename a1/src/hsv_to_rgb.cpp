#include "hsv_to_rgb.h"
#include <cmath>

void hsv_to_rgb(
  const double h,
  const double s,
  const double v,
  double & r,
  double & g,
  double & b)
{
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
    double c = v * s;
    double hPrime = fmod(h / 60.0, 6.0);
    double x = c * (1.0 - fabs(fmod(hPrime, 2.0) - 1.0));
    
    double r1, g1, b1;
    if (0.0 <= hPrime && hPrime < 1.0) {
        r1 = c;
        g1 = x;
        b1 = 0.0;
    } else if (1.0 <= hPrime && hPrime < 2.0) {
        r1 = x;
        g1 = c;
        b1 = 0.0;
    } else if (2.0 <= hPrime && hPrime < 3.0) {
        r1 = 0.0;
        g1 = c;
        b1 = x;
    } else if (3.0 <= hPrime && hPrime < 4.0) {
        r1 = 0.0;
        g1 = x;
        b1 = c;
    } else if (4.0 <= hPrime && hPrime < 5.0) {
        r1 = x;
        g1 = 0.0;
        b1 = c;
    } else if (5.0 <= hPrime && hPrime < 6.0) {
        r1 = c;
        g1 = 0.0;
        b1 = x;
    } else {
        r1 = 0.0;
        g1 = 0.0;
        b1 = 0.0;
    }
    
    double m = v - c;
    r = r1 + m;
    g = g1 + m;
    b = b1 + m;
}
