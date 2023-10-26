#ifndef RAYTRACER_INTERVAL_H
#define RAYTRACER_INTERVAL_H

#include <functional>
#include "mathutils.h"

class Interval {
public:
    double min, max;

    Interval(double _min, double _max) : min(_min), max(_max) {}

    Interval() : min(+infinity), max(-infinity) {}

    [[nodiscard]] bool contains(double x) const {
        return min <= x && x <= max;
    }

    [[nodiscard]] bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        return std::clamp(x, min, max);
    }

    static const Interval empty, universe;
};

const static Interval empty (+infinity, -infinity);
const static Interval universe (-infinity, +infinity);

#endif //RAYTRACER_INTERVAL_H
