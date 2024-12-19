#pragma once

#include "Utils.h"

struct Interval
{
	double min{ -Utils::infinity };
	double max{ Utils::infinity };

	Interval() = default;

	Interval(double min, double max)
	{
		this->min = min;
		this->max = max;
	}

	double size() const
	{
		return max - min;
	}

	bool contains(double x) const
	{
		return (min <= x && x <= max);
	}

	bool surrounds(double x) const
	{
		return (min < x && x < max);
	}

	double clamp(double x) const 
	{
		return std::fmax(min, std::fmin(max, x));
	}

	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+Utils::infinity, -Utils::infinity);
const Interval Interval::universe = Interval(-Utils::infinity, +Utils::infinity);

