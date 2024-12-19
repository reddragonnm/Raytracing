#pragma once

#include <random>
#include <limits>

namespace Utils
{
	inline constexpr double infinity{ std::numeric_limits<double>::infinity() };
	inline constexpr double pi{ 3.1415926535897932385 };

	inline double degreesToRadians(double degrees)
	{
		return degrees * pi / 180.0;
	}

	inline double randomDouble(double min = 0.0, double max = 1.0)
	{
		static std::mt19937 generator{ std::random_device{}() };
		static std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}
}
