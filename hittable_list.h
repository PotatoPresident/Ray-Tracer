#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"

using std::shared_ptr;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord& rec) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(ray, Interval(ray_t.min, closest_so_far), temp_record)) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                rec = temp_record;
            }
        }

        return hit_anything;
    }
};

#endif //RAYTRACER_HITTABLE_LIST_H
