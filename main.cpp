#include <iostream>
#include <fstream>
#include "color.h"


int main() {
    // Image
    int image_width = 256;
    int image_height = 256;
    std::ofstream outfile;
    outfile.open("image.ppm");

    // Render

    outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; ++j) {
        std::cout << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0.5);
            write_color(outfile, pixel_color);

        }
    }

    std::cout << "\rDone.                    \n";

    outfile.close();
}
