#pragma once

#include <SFML/Graphics.hpp>

#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include "VectorAddons.h"
#include "Utils.h"


class Camera
{
public:
    double aspectRatio{ 16.0 / 9 }; // width/height
    int imageWidth{ 400 }; // rendered image width in pixel count
    int imageHeight;

    int samplesPerPixel{ 20 }; // number of random samples for each pixel
    int maxRayBounces{ 10 };

    double vfov{ 90 }; // vertical view angle (field of view)
    Vector3d lookfrom{ 0, 0, 0 };
    Vector3d lookat{ 0, 0, -1 };
    Vector3d vup{ 0, 1, 0 };

    double defocusAngle{ 0 };
    double focusDist{ 10 }; // distance from lookfrom to center of plane of perfect focus

    sf::Image& render(const Hittable& world)
    {

        for (int i{ 0 }; i < imageWidth; i++)
        {
            for (int j{ 0 }; j < imageHeight; j++)
            {
                Color3d pixelColor{};
                for (int k{ 0 }; k < samplesPerPixel; k++)
                {
                    pixelColor += getRayColor(getSamplingRay(i, j), world, maxRayBounces);
                }


                screen.setPixel(i, j, VectorAddons::color3dToSfColor(pixelColor / static_cast<double>(samplesPerPixel)));
            }
        }

        return screen;
    }

    void initialize()
    {
        imageHeight = std::max(1, static_cast<int>(imageWidth / aspectRatio));


        double theta{ Utils::degreesToRadians(vfov) };
        double h{ std::tan(theta / 2) };
        viewportHeight = 2.0 * h * focusDist;

        viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
        center = lookfrom;

        w = VectorAddons::unitVector(lookfrom - lookat);
        u = VectorAddons::unitVector(VectorAddons::crossProduct(vup, w));
        v = VectorAddons::crossProduct(w, u);

        viewportU = viewportWidth * u;
        viewportV = viewportHeight * -v;

        pixelDeltaU = Vector3d{ viewportU / static_cast<double>(imageWidth) };
        pixelDeltaV = Vector3d{ viewportV / static_cast<double>(imageHeight) };

        double defocusRadius{ focusDist * std::tan(Utils::degreesToRadians(defocusAngle / 2)) };
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;

        viewportUpperLeft = Vector3d{ center - (focusDist * w) - viewportU / 2.0 - viewportV / 2.0 };
        pixel00Loc = Vector3d{ viewportUpperLeft + (pixelDeltaU + pixelDeltaV) / 2.0 };

        screen.create(imageWidth, imageHeight);
    }

private:
    sf::Image screen;

    double viewportHeight;
    double viewportWidth; // multiplied by actual aspect ratio
    Vector3d center;

    Vector3d viewportU;
    Vector3d viewportV; // y increasing downwards

    Vector3d pixelDeltaU;
    Vector3d pixelDeltaV;

    Vector3d viewportUpperLeft;
    Vector3d pixel00Loc;

    Vector3d u, v, w; // camera basis vectors
    Vector3d defocusDiskU;
    Vector3d defocusDiskV;

    Ray getSamplingRay(int i, int j) const
    {
        Vector3d offset{ sampleSquare() };
        Vector3d pixelSample{ pixel00Loc + ((i + offset.x) * pixelDeltaU) + ((j + offset.y) * pixelDeltaV) };

        Vector3d rayOrigin{ defocusAngle <= 0 ? center : defocusDiskSample() };
        Vector3d rayDirection{ pixelSample - rayOrigin };

        return Ray{ rayOrigin, rayDirection };
    }

    Vector3d defocusDiskSample() const
    {
        Vector3d p{ VectorAddons::randomInUnitDisk() };
        return center + (p.x * defocusDiskU) + (p.y * defocusDiskV);
    }

    Vector3d sampleSquare() const
    {
        return Vector3d{ Utils::randomDouble(-0.5, 0.5), Utils::randomDouble(-0.5, 0.5) , 0.0 };
    }

    Color3d getRayColor(const Ray& r, const Hittable& world, int depth) const
    {
        if (depth <= 0) return Color3d{};

        HitInfo info;
        // min=0.01 to get rid of the shadow acne problem
        // Floating point errors for origin for the next ray so that it may instantiate just below or above the surface
        if (world.hit(r, Interval{ 0.001, Utils::infinity }, info))
        {
            Ray scattered{};
            Color3d attenuation{};

            if (info.mat->scatter(r, info, attenuation, scattered))
                return attenuation * getRayColor(scattered, world, depth - 1);

            return Color3d{};
        }

        // Gradient background
        Vector3d unitDirection{ VectorAddons::unitVector(r.direction) };
        double a{ (unitDirection.y + 1.0) / 2.0 }; // unitDirection.y varies from -1 to 1 => a varies from 0 to 1

        // Lerp blend between colors
        return (1.0 - a) * Color3d { 1.0, 1.0, 1.0 } + a * Color3d(0.5, 0.7, 1.0);
    }
};

