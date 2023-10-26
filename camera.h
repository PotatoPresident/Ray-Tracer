#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "hittable.h"
#include "color.h"

class Camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    std::ofstream outfile;

    void render(const Hittable &world) {
        initialize();

        outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; ++j) {
            std::cout << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray r = getRay(i, j);
                    pixel_color += rayColor(r, max_depth, world);
                }

                writeColor(outfile, pixel_color, samples_per_pixel);
            }
        }

        std::cout << "\rDone.                    \n";

        outfile.close();
    }

private:
    int image_height;
    Point3 center;
    Point3 pixel00_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;

    void initialize() {
        outfile = std::ofstream("image.ppm");

        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = Point3(0, 0, 0);

        // Viewport dimensions
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = Vec3(viewport_width, 0, 0);
        auto viewport_v = Vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate upper left pixel location
        auto viewport_upper_left = center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray getRay(int i, int j) const {
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixelSampleSquare();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    // Returns a random point in the square surrounding the pixel center
    Vec3 pixelSampleSquare() const {
        auto px = -0.5 + randomDouble();
        auto py = -0.5 + randomDouble();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    static Color rayColor(const Ray& ray, int depth, const Hittable& world) {
        HitRecord record;

        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        if (world.hit(ray, Interval(0.001, infinity), record)) {
            Vec3 direction = randomOnHemisphere(record.normal);
            return 0.5 * rayColor(Ray(record.point, direction), depth - 1, world);
        }

        Vec3 unit_direction = unitVector(ray.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    }
};

#endif //RAYTRACER_CAMERA_H
