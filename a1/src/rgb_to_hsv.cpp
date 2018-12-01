#include "rgb_to_hsv.h"
#include <algorithm>

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
    double maximum = std::max(std::max(r, g), b);
    double minimum = std::min(std::min(r, g), b);
    
    v = maximum;
    
    double delta = maximum - minimum;
    if (delta < 0.00001) {
        s = 0.0;
        h = 0.0;
        return;
    }
    
    if (maximum == minimum) {
        h = 0.0;
    } else if (maximum == r) {
        h = (0.0 + (g - b) / delta) * 60.0;
    } else if (maximum == g) {
        h = (2.0 + (b - r) / delta) * 60.0;
    } else {
        h = (4.0 + (r - g) / delta) * 60.0;
    }
    
    if (h < 0.0) {
        h += 360.0;
    }
    
    if (maximum == 0.0) {
        s = 0.0;
    } else {
        s = delta / maximum;
    }
}
