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

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override{
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = record.front_face ? (1.0 / ir) : ir;

        Vec3 unit_direction = unitVector(ray_in.direction());
        double cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble()) {
            direction = reflect(unit_direction, record.normal);
        } else {
            direction = refract(unit_direction, record.normal, refraction_ratio);
        }

        scattered = Ray(record.point, direction);
        return true;
    }

private:
    double ir; // Index of refraction

    static double reflectance(double cosine, double i_of_ref) {
        // Use Schlick's approximation for reflectance
        auto r0 = (1 - i_of_ref) / (1 + i_of_ref);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine), 5);
    }
};

#endif //RAYTRACER_MATERIAL_H
