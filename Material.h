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

class Dielectric : public Material
{
public:
    Dielectric(double refractiveIndex)
    {
        this->refractiveIndex = refractiveIndex;
    }

    bool scatter(const Ray& rIn, const HitInfo& info, Color3d& attenuation, Ray& scattered) const override
    {
        attenuation = Color3d{ 1.0, 1.0, 1.0 };
        double ri{ info.frontFace ? (1.0 / refractiveIndex) : refractiveIndex };

        Vector3d unitDirection{ VectorAddons::unitVector(rIn.direction) };

        double cosTheta{ std::fmin(1.0, VectorAddons::dotProduct(-unitDirection, info.normal)) };
        double sinTheta{ std::sqrt(1.0 - cosTheta * cosTheta) };

        bool cannotRefract{ ri * sinTheta > 1.0 };
        Vector3d direction{ (cannotRefract || (reflectance(cosTheta, ri) > Utils::randomDouble())) ? VectorAddons::reflect(unitDirection, info.normal) : VectorAddons::refract(unitDirection, info.normal, ri) };


        scattered = Ray{ info.point, direction };
        return true;
    }

private:
    double refractiveIndex{ 0 }; // relative refractive index

    static double reflectance(double cosine, double refractionIndex)
    {
        // using Schlick's approx. for reflectance
        double r0{ (1 - refractionIndex) / (1 + refractionIndex) };
        r0 *= r0;
        return r0 + (1.0 - r0) * std::pow(1 - cosine, 5);
    }
};
