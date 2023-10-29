#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "fstream"

#include "vec3.h"

using Color = Vec3;

inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);
}

void writeColor(std::vector<unsigned char>& pixels, int index, Color pixel_color, int spp) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide color by number of samples
    auto scale = 1.0 / spp;
    r *= scale;
    g *= scale;
    b *= scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component
    static const Interval intensity(0.0, 0.999);
    pixels[index] = static_cast<unsigned char>(256 * intensity.clamp(r));
    pixels[index+1] = static_cast<unsigned char>(256 * intensity.clamp(g));
    pixels[index+2] = static_cast<unsigned char>(256 * intensity.clamp(b));
}

#endif //RAYTRACER_COLOR_H
