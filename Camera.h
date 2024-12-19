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

    Camera()
    {
        initialize();
    }

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

private:
    sf::Image screen;

    double focalLength; // distance between camera center and viewport center
    double viewportHeight;
    double viewportWidth; // multiplied by actual aspect ratio
    Vector3d center;

    Vector3d viewportU;
    Vector3d viewportV; // y increasing downwards

    Vector3d pixelDeltaU;
    Vector3d pixelDeltaV;

    Vector3d viewportUpperLeft;
    Vector3d pixel00Loc;

    void initialize()
    {
        imageHeight = std::max(1, static_cast<int>(imageWidth / aspectRatio));

        focalLength = 1.0;
        viewportHeight = 2.0;
        viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
        center = Vector3d{ 0, 0, 0 };

        viewportU = Vector3d{ viewportWidth, 0, 0 };
        viewportV = Vector3d{ 0, -viewportHeight, 0 };

        pixelDeltaU = Vector3d{ viewportU / static_cast<double>(imageWidth) };
        pixelDeltaV = Vector3d{ viewportV / static_cast<double>(imageHeight) };

        viewportUpperLeft = Vector3d{ center + Vector3d{0, 0, -focalLength} - viewportU / 2.0 - viewportV / 2.0 };
        pixel00Loc = Vector3d{ viewportUpperLeft + (pixelDeltaU + pixelDeltaV) / 2.0 };

        screen.create(imageWidth, imageHeight);
    }

    Ray getSamplingRay(int i, int j) const
    {
        Vector3d offset{ sampleSquare() };
        Vector3d pixelSample{ pixel00Loc + ((i + offset.x) * pixelDeltaU) + ((j + offset.y) * pixelDeltaV) };

        Vector3d rayOrigin{ center };
        Vector3d rayDirection{ pixelSample - rayOrigin };

        return Ray{ rayOrigin, rayDirection };
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

