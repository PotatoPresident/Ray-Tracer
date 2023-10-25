#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "fstream"

#include "vec3.h"

using color = vec3;

void write_color(std::ofstream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif //RAYTRACER_COLOR_H
