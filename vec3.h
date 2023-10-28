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

    bool nearZero() const {
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
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

inline Vec3 randomInUnitDisk() {
    while (true) {
        auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
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

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v, n)*n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif //RAYTRACER_VEC3_H
