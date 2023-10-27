#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(Point3 _center, double _radius, shared_ptr<Material> _material) : center(_center), radius(_radius), material(std::move(_material)) {}

    bool hit(const Ray &ray, Interval ray_t, HitRecord& rec) const override {
        Vec3 oc = ray.origin() - center; // Origin to center
        auto a = ray.direction().lengthSquared();
        auto half_b = dot(oc, ray.direction());
        auto c = oc.lengthSquared() - radius * radius;
        auto discriminant = half_b*half_b - a*c;

        if (discriminant < 0) return false;
        auto sqrt_disc = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (-half_b - sqrt_disc) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrt_disc) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.point = ray.at(rec.t);
        Vec3 outward_normal = (rec.point - center) / radius;
        rec.setFaceNormal(ray, outward_normal);
        rec.material = material;

        return true;
    }

private:
    Point3 center;
    double radius;
    shared_ptr<Material> material;
};

#endif //RAYTRACER_SPHERE_H
