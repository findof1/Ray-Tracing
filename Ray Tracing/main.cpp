#include <hittableList.h>
#include <sphere.h>
#include <common.h>
#include <camera.h>
#include "material.h"


int main() {
    hittableList world;

    auto groundMaterial = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomDouble();
            auto chooseMove = randomDouble();
            point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphereMaterial;

                if (chooseMat < 0.8) {

                    auto albedo = color::random() * color::random();
                    sphereMaterial = std::make_shared<lambertian>(albedo);
                    if (chooseMove > 0.5) {
                        world.add(std::make_shared<sphere>(center, 0.2, sphereMaterial));
                    }
                    else {
                        auto center2 = center + vec3(0, randomDouble(0, .5), 0);
                        world.add(std::make_shared<sphere>(center, center2, 0.2, sphereMaterial));
                    }
                }
                else if (chooseMat < 0.95) {
             
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<metal>(albedo, fuzz);
                    if (chooseMove > 0.5) {
                        world.add(std::make_shared<sphere>(center, 0.2, sphereMaterial));
                    }
                    else {
                        auto center2 = center + vec3(0, randomDouble(0, .5), 0);
                        world.add(std::make_shared<sphere>(center, center2, 0.2, sphereMaterial));
                    }
                }
                else {
                    sphereMaterial = std::make_shared<dielectric>(1.5);
                    if (chooseMove > 0.5) {
                        world.add(std::make_shared<sphere>(center, 0.2, sphereMaterial));
                    }
                    else {
                        auto center2 = center + vec3(0, randomDouble(0, .5), 0);
                        world.add(std::make_shared<sphere>(center, center2, 0.2, sphereMaterial));
                    }
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(2, 1, -3), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, -1), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(6, 1, 3), 1.0, material3));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.render(world);

    return 0;
}