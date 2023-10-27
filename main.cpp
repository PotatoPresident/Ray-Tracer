#include "ray.h"
#include "mathutils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main() {
    // World
    HittableList world;

    // Materials
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_matte = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_glass = make_shared<Dielectric>(1.5);
    auto material_metal = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point3(0.0,-100.5,-1.0), 100, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0,0.0,-1.0), 0.5, material_matte));
    world.add(make_shared<Sphere>(Point3(-1.0,0.0,-1.0), -0.5, material_glass));
    world.add(make_shared<Sphere>(Point3(1.0,0.0,-1.0), 0.5, material_metal));


    // Camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}
