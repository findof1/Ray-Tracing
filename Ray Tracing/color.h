#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include <vector>

using color = vec3;

inline double linearToGamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

void writeColor(std::vector<unsigned char>& data, int index, const color& pixelColor) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));
    
    data[index] = rbyte;
    data[index + 1] = gbyte;
    data[index + 2] = bbyte;
}

#endif
