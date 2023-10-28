#ifndef RAYTRACER_MATHUTILS_H
#define RAYTRACER_MATHUTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Util Functions
inline double degreesToRadians(double degrees) {
    return degrees * (pi / 180.0);
}

// Returns a random real in [0, 1).
inline double randomDouble() {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random real in [min, max).
inline double randomDouble(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

#endif //RAYTRACER_MATHUTILS_H
