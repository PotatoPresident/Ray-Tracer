#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "mathutils.h"
#include "hittable_list.h"
#include "color.h"
#include "hittable.h"

class HitRecord;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Color& _albedo) : albedo(_albedo) {}

    bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        auto scatter_dir = record.normal + randomUnitVector();

        if (scatter_dir.nearZero()) {
            scatter_dir = record.normal;
        }

        scattered = Ray(record.point, scatter_dir);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material {
public:
    Metal(const Color& _albedo, double fuzziness) : albedo(_albedo), fuzz(fuzziness < 1 ? fuzziness : 1) {}

    bool scatter(const Ray &ray_in, const HitRecord &record, Color &attenuation, Ray &scattered) const override {
        Vec3 reflected = reflect(unitVector(ray_in.direction()), record.normal);
        scattered = Ray(record.point, reflected + fuzz*randomInUnitSphere());
        attenuation = albedo;

        return (dot(scattered.direction(), record.normal) > 0);
    }


private:
    Color albedo;
    double fuzz;
};

#endif //RAYTRACER_MATERIAL_H
