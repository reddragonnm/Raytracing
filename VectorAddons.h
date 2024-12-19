#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Utils.h"
#include "Interval.h"

using Vector3d = sf::Vector3<double>;
using Color3d = Vector3d;

inline Vector3d operator*(const Vector3d& a, const Vector3d& b)
{
    return Vector3d{ a.x * b.x, a.y * b.y, a.z * b.z };
}

namespace VectorAddons
{
    inline double dotProduct(const Vector3d& v1, const Vector3d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline Vector3d crossProduct(const Vector3d& v1, const Vector3d& v2)
    {
        return Vector3d{
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        };
    }

    inline double lengthSquared(const Vector3d& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    inline double length(const Vector3d& v)
    {
        return std::sqrt(lengthSquared(v));
    }

    inline Vector3d unitVector(const Vector3d& v)
    {
        return v / length(v);
    }

    inline double linearToGamma(double linearComponent)
    {
        // image viewers expect that images are already gamma corrected but our image is linear space
        // if we use an image viewer we should convert this to gamma space
        // but currently this is viewed correctly in sfml

        // assume gamma=2 (approx.)
        // gammaComponent = linearComponent^(1/gamma) = sqrt(linearComponent)

        if (linearComponent > 0.0) return std::sqrt(linearComponent);
        return 0.0;
    }

    inline sf::Color color3dToSfColor(const Color3d& c)
    {
        static const Interval intensity{ 0.0, 1.0 };

        auto r{ static_cast<sf::Uint8>(intensity.clamp(c.x) * 255) };
        auto g{ static_cast<sf::Uint8>(intensity.clamp(c.y) * 255) };
        auto b{ static_cast<sf::Uint8>(intensity.clamp(c.z) * 255) };
        return sf::Color{ r, g, b };

    }

    inline Vector3d randomVec(double min = 0.0, double max = 0.0)
    {
        return Vector3d{
            Utils::randomDouble(min, max),
            Utils::randomDouble(min, max),
            Utils::randomDouble(min, max)
        };
    }

    inline Vector3d randomUnitVec()
    {
        while (true)
        {
            Vector3d p{ randomVec() };
            double lengthSq{ lengthSquared(p) };

            // prevent underflow to zero
            if (1e-160 < lengthSq && lengthSq <= 1) return p / std::sqrt(lengthSq);
        }
    }

    inline Vector3d randomOnHemisphere(const Vector3d& normal)
    {
        Vector3d onUnitSphere{ randomUnitVec() };
        return (dotProduct(onUnitSphere, normal) > 0.0) ? onUnitSphere : -onUnitSphere;
    }

    inline bool isNearZero(const Vector3d& v)
    {
        constexpr double s{ 1e-8 };
        return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
    }

    inline Vector3d reflect(const Vector3d& v, const Vector3d& n)
    {
        return v - 2.0 * dotProduct(v, n) * n;
    }
}
