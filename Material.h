#pragma once

#include "Ray.h"
#include "VectorAddons.h"
#include "Hittable.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& rIn, const HitInfo& info, Color3d& attenuation, Ray& scattered) const
    {
        return false;
    }
};

class Lambertian : public Material
{
    // follows lambertian (diffuse) reflectance
    // always scatters

public:
    Lambertian(const Color3d& albedo)
    {
        this->albedo = albedo;
    }

    bool scatter(const Ray& rIn, const HitInfo& info, Color3d& attenuation, Ray& scattered) const override
    {
        Vector3d scatterDirection{ info.normal + VectorAddons::randomUnitVec() };

        if (VectorAddons::isNearZero(scatterDirection)) scatterDirection = info.normal;

        scattered = Ray{ info.point, scatterDirection };
        attenuation = albedo;

        return true;
    }

private:
    Color3d albedo{};
};

class Metal : public Material
{
public:
    Metal(const Color3d& albedo, double fuzz = 0.0)
    {
        this->albedo = albedo;
        this->fuzz = std::fmin(1.0, fuzz);
    }

    bool scatter(const Ray& rIn, const HitInfo& info, Color3d& attenuation, Ray& scattered) const override
    {
        Vector3d reflected{ VectorAddons::reflect(rIn.direction, info.normal) };
        reflected = VectorAddons::unitVector(reflected) + (fuzz * VectorAddons::randomUnitVec()); // fuzz needs to be scaled wrt reflection

        scattered = Ray{ info.point, reflected };
        attenuation = albedo;

        return VectorAddons::dotProduct(scattered.direction, info.normal) > 0; // if below the surface then absorb the ray
    }

private:
    Color3d albedo{};
    double fuzz;
};
