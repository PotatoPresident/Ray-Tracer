#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"

class HitRecord {
public:
    Point3 point;
    Vec3 normal;
    double t;
    bool front_face;

    // Sets the hit record normal vector
    // NOTE: `outward_normal` should be unit length
    void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &r, Interval ray_t, HitRecord& rec) const = 0;
};

#endif //RAYTRACER_HITTABLE_H
