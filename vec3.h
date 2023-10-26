#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>
#include "mathutils.h"

class Vec3 {
public:
    double e[3];

    Vec3() : e{0, 0, 0} {}
    Vec3(double x, double y, double z) : e{x, y, z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    Vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    Vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    [[nodiscard]] double lengthSquared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    [[nodiscard]] double length() const {
        return sqrt(lengthSquared());
    }

    static Vec3 random() {
        return {randomDouble(), randomDouble(), randomDouble()};
    }

    static Vec3 random(double min, double max) {
        return {randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
    }
};

using Point3 = Vec3; // 3D point

// Vector Util Functions
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3 &v, double t) {
    return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.e[0]*v.e[0]
         + u.e[1]*v.e[1]
         + u.e[2]*v.e[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2]*v.e[0] - u.e[0]*v.e[2],
                u.e[0]*v.e[1] - u.e[1]*v.e[0]);
}

inline Vec3 unitVector(Vec3 v) {
    return v / v.length();
}

inline Vec3 randomInUnitSphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

inline Vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
    Vec3 onUnitSphere = randomUnitVector();
    if (dot(onUnitSphere, normal) > 0.0) {
        return onUnitSphere; // In the same hemisphere as the normal
    } else {
        return -onUnitSphere;
    }
}

#endif //RAYTRACER_VEC3_H
