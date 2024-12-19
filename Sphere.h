#pragma once

#include <cmath>
#include <memory>

#include "Material.h"
#include "VectorAddons.h"
#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Sphere(const Vector3d& center, double radius, std::shared_ptr<Material> mat)
    {
        this->center = center;
        this->radius = std::fmax(0.0, radius);
        this->mat = mat;
    }

    bool hit(const Ray& r, Interval rayT, HitInfo& info) const override
    {
        Vector3d qc{ r.origin - center };
        Vector3d d{ r.direction };

        double a{ VectorAddons::lengthSquared(d) };
        double h{ -VectorAddons::dotProduct(qc, d) }; // h = -b/2 for simplification?
        double c{ VectorAddons::lengthSquared(qc) - radius * radius };

        double discriminant{ h * h - a * c };

        if (discriminant < 0.0)
            return false;

        double sqrtD{ std::sqrt(discriminant) };


        // get the smallest root (value of t) if possible
        double root{ (h - sqrtD) / a }; // smaller than below value as sqrtD and a are positive
        if (!rayT.surrounds(root))
        {
            root = (h + sqrtD) / a;
            if (!rayT.surrounds(root)) return false;
        }

        info.point = r.at(root);
        info.t = root;
        info.setFaceNormal(r, (info.point - center) / radius);
        info.mat = mat;

        return true;
    }

private:
    Vector3d center;
    double radius;
    std::shared_ptr<Material> mat;
};
