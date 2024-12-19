#pragma once

#include "VectorAddons.h"

struct Ray
{
    Vector3d origin{};
    Vector3d direction{};

    Vector3d at(const double t) const
    {
        return origin + t * direction;
    }
};

