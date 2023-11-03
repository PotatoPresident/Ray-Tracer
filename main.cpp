#include "ray.h"
#include "mathutils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main(int argc, char* argv[]) {


    HittableList world;

    auto ground_Material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_Material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            Point3 center(a + 0.9*randomDouble(), 0.2 + 0.6*randomDouble(), b + 0.9*randomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> Sphere_Material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    Sphere_Material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    Sphere_Material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                } else {
                    // glass
                    Sphere_Material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color (0.9, 0.2, 0.2));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color (0.5, 0.6, 0.9), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
    Color::random(0.5, 1);

    Camera cam;

    if (argc > 1) {
        if (argc != 6) {
            std::cerr << "Usage: " << argv[0] << " <image_width> <image_height> <samples_per_pixel> <max_depth> <threads>\n";
            return 1;
        }
        std::size_t pos;
        cam.image_width = std::stoi(argv[1], &pos, 0);
        cam.image_height = std::stoi(argv[2], &pos, 0);
        cam.samples_per_pixel = std::stoi(argv[3], &pos, 0);
        cam.max_depth = std::stoi(argv[4], &pos, 0);
        cam.max_threads = std::stoi(argv[5], &pos, 0);
    }

    cam.vfov     = 20;
    cam.look_from = Point3(14,2,4);
    cam.look_at   = Point3(0,0,0);
    cam.vup      = Vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_distance    = 10.0;

    cam.render(world);
}
