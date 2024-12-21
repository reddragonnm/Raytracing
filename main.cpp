#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Material.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"


int main()
{
    // World
    HittableList world{};

    auto groundMaterial{ std::make_shared<Lambertian>(Color3d{0.5, 0.5, 0.5}) };
    world.add(std::make_shared<Sphere>(Vector3d{ 0, -1000, 0 }, 1000, groundMaterial));

    for (int a = -3; a < 3; a++)
    {
        for (int b = -3; b < 3; b++)
        {
            double chooseMat{ Utils::randomDouble() };
            Vector3d center{ a + 0.9 * Utils::randomDouble(), 0.2, b + 0.9 * Utils::randomDouble() };

            if (VectorAddons::length(center - Vector3d{ 4, 0.2, 0 }) > 0.9)
            {
                std::shared_ptr<Material> sphereMat;

                if (chooseMat < 0.8)
                {
                    Color3d albedo{ VectorAddons::randomVec() * VectorAddons::randomVec() };
                    sphereMat = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphereMat));
                }
                else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = VectorAddons::randomVec(0.5, 1);
                    auto fuzz = Utils::randomDouble(0, 0.5);
                    sphereMat = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphereMat));
                }
                else {
                    // glass
                    sphereMat = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphereMat));
                }
            }
        }

    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vector3d(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color3d(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Vector3d(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color3d(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Vector3d(4, 1, 0), 1.0, material3));

    // Camera
    Camera cam{};
    cam.imageWidth = 400;
    cam.samplesPerPixel = 20;
    cam.maxRayBounces = 10;

    cam.lookfrom = Vector3d{ 13, 2, 3 };
    cam.lookat = Vector3d{ 0, 0,0 };
    cam.vup = Vector3d{ 0, 1, 0 };
    cam.vfov = 20;

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.initialize();


    sf::RenderWindow window(sf::VideoMode(cam.imageWidth, cam.imageHeight),
        "Raytracing");

    sf::Texture tex;
    auto image = cam.render(world);
    tex.loadFromImage(image);

    if (image.saveToFile("final.png")) std::cout << "Image saved\n";
    else std::cout << "Unsuccessful\n";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }


        sf::Sprite bgSprite{ tex };

        window.draw(bgSprite);
        window.display();
    }

    return 0;
}
