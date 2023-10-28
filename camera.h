#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "hittable.h"
#include "color.h"
#include "material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int CHANNEL_NUM = 3;

class Camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10; // Number of random samples per pixel
    int max_depth = 10; // Maximum number of ray bounces
    double vfov = 90; // Degrees
    Point3 look_from = Point3(0, 0, -1); // Point camera is looking from
    Point3 look_at = Point3(0, 0, 0); // Point camera is looking at
    Vec3 vup = Vec3(0, 1, 0); // Camera's relative up vector
    std::string imageName = "image.png";

    void render(const Hittable &world) {
        initialize();

        std::vector<unsigned char> pixels(image_height*image_width*CHANNEL_NUM);
        uint64_t index = 0;
        for (int j = 0; j < image_height; ++j) {
            std::cout << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray r = getRay(i, j);
                    pixel_color += rayColor(r, max_depth, world);
                }

                writeColor(pixels, index, pixel_color, samples_per_pixel);
            }
        }
        stbi_write_png(imageName.c_str(), image_width, image_height, CHANNEL_NUM, pixels.data(), image_width * CHANNEL_NUM);

        std::cout << "\rDone.                    \n";
    }

private:
    int image_height;
    Point3 center;
    Point3 pixel00_loc; // Location of the upper left pixel (0, 0)
    Vec3 pixel_delta_u; // Offset to next horizontal pixel
    Vec3 pixel_delta_v; // Offset to next vertical pixel
    Vec3 u, v, w; // Camera basis vectors

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = look_from;

        // Viewport dimensions
        auto focal_length = (look_from - look_at).length();
        auto theta = degreesToRadians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h * focal_length;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the camera basis vectors
        w = unitVector(look_from - look_at); // Unit vector pointing opposite the view direction
        u = unitVector(cross(vup, w)); // Unit vector pointing to camera right
        v = cross(w, u); // Unit vector pointing to camera up

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = viewport_width * u; // Horizontal viewport vector
        auto viewport_v = viewport_height * -v; // Vertical viewport vector

        // Calculate the horizontal and vertical delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate upper left pixel location
        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
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
            Ray scattered;
            Color attenuation;
            if (record.material->scatter(ray, record, attenuation, scattered)) {
                return attenuation * rayColor(scattered, depth-1, world);
            }
            return Color(0, 0, 0);
        }

        Vec3 unit_direction = unitVector(ray.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    }
};

#endif //RAYTRACER_CAMERA_H
