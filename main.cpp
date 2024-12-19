#include <memory>

#include <SFML/Graphics.hpp>

#include "Material.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"


int main()
{
    auto materialGround{ std::make_shared<Lambertian>(Color3d{0.8, 0.8, 0.0}) };
    auto materialCenter{ std::make_shared<Lambertian>(Color3d{0.1, 0.2, 0.5}) };
    //auto materialLeft{ std::make_shared<Metal>(Color3d{0.8, 0.8, 0.8}, 0.3) };
    auto materialLeft{ std::make_shared<Dielectric>(1.5) };
    auto materialBubble{ std::make_shared<Dielectric>(1.0 / 1.5) };
    auto materialRight{ std::make_shared<Metal>(Color3d{1, 1, 1}, 1.0) };

    // World
    HittableList world{};
    world.add(std::make_shared<Sphere>(Vector3d{ 0, 0, -1 }, 0.5, materialCenter));
    world.add(std::make_shared<Sphere>(Vector3d{ 0, -100.5, -1 }, 100, materialGround));

    world.add(std::make_shared<Sphere>(Vector3d{ -1.0, 0.0, -1.0 }, 0.5, materialLeft));
    world.add(std::make_shared<Sphere>(Vector3d{ -1.0, 0.0, -1.0 }, 0.4, materialBubble));

    world.add(std::make_shared<Sphere>(Vector3d{ 1.0, 0.0, -1.0 }, 0.5, materialRight));


    // Camera
    Camera cam{};

    sf::RenderWindow window(sf::VideoMode(cam.imageWidth, cam.imageHeight),
        "Raytracing");

    sf::Texture tex;
    tex.loadFromImage(cam.render(world));

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
