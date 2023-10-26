#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "fstream"

#include "vec3.h"

using Color = Vec3;

void writeColor(std::ofstream &out, Color pixel_color, int spp) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide color by number of samples
    auto scale = 1.0 / spp;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component
    static const Interval intensity(0.0, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif //RAYTRACER_COLOR_H
