#pragma once

#include <random>
#include <limits>
#include <numbers>

namespace Utils
{
    inline constexpr double infinity{ std::numeric_limits<double>::infinity() };
    inline constexpr double pi{ std::numbers::pi };

    inline double degreesToRadians(double degrees)
    {
        return degrees * pi / 180.0;
    }

    inline double randomDouble()
    {
        static std::mt19937 generator{ std::random_device{}() };
        static std::uniform_real_distribution<double> distribution(0, 1);
        return distribution(generator);
    }

    inline double randomDouble(double min, double max)
    {
        static std::mt19937 generator{ std::random_device{}() };
        static std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }
}
