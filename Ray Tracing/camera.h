#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "color.h"
#include "hittable.h"
#include "interval.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "material.h"

class camera {
public:
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int channels = 3;
    int samplesPerPixel = 10;
    int maxDepth = 10;
    int fov = 70;
    point3 lookfrom = point3(0, 0, 0);   
    point3 lookat = point3(0, 0, -1); 
    vec3   vup = vec3(0, 1, 0);

    double defocusAngle = 0;
    double focusDist = 10;

    void render(const hittable& world) {
        initialize();

        std::vector<unsigned char> imageData(imageWidth * imageHeight * channels);

        for (int j = 0; j < imageHeight; ++j) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

            for (int i = 0; i < imageWidth; ++i) {
                color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    ray r = getRay(i, j);
                    pixelColor += rayColor(r, maxDepth ,world);
                }

                int index = (j * imageWidth + i) * channels;



                writeColor(imageData, index, pixelSamplesScale * pixelColor);
            }
        }

        if (!stbi_write_png("image.png", imageWidth, imageHeight, channels, imageData.data(), imageWidth * channels)) {
            std::cout << "Failed to write PNG file." << std::endl;
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int imageHeight;
    double pixelSamplesScale;
    point3 center;       
    point3 pixel00Loc;   
    vec3 pixelDeltaU; 
    vec3 pixelDeltaV;
    vec3 u, v, w;

    vec3   defocusDiskU;
    vec3   defocusDiskV;

    void initialize() {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = lookfrom;

        double theta = degreesToRadians(fov);
        double h = tan(theta / 2);

        double viewportHeight = 2 * h * focusDist;
        
        double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

        w = unitVector(lookfrom - lookat);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        vec3 viewportU = viewportWidth * u;
        vec3 viewportV = viewportHeight * -v;

        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        vec3 viewportUpperLeft = center - (focusDist * w) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        auto defocusRadius = focusDist * tan(degreesToRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    ray getRay(int i, int j) const {

        auto offset = sampleSquare();
        auto pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);


        point3 rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
        vec3 rayDirection = pixelSample - rayOrigin;
        double tm = randomDouble();

        return ray(rayOrigin, rayDirection, tm);
    }

    vec3 sampleSquare() const {
        return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
    }

    point3 defocusDiskSample() const {
        auto p = randomInUnitDisk();
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    color rayColor(const ray& r,int depth, const hittable& world) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }
        hitRecord rec;

        if (world.hit(r, interval(0.001, infinityVal), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * rayColor(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unitDirection = unitVector(r.direction());
        auto a = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif
