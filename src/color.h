#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"
#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    float r = pixel_color.x();
    float g = pixel_color.y();
    float b = pixel_color.z();

    // divide color by number samples, gamma-correct for gamma=2.0
    float scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // write translated 0-255 color value of each component
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif
