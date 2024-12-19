#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"
#include "Ray.h"

using HittablePtr = std::shared_ptr<Hittable>;

class HittableList : public Hittable
{
public:
	std::vector<HittablePtr> objects{};

	HittableList() = default;

	HittableList(HittablePtr object)
	{
		add(object);
	}

	void add(HittablePtr object)
	{
		objects.push_back(object);
	}

	bool hit(const Ray& r, Interval rayT, HitInfo& info) const override
	{
		HitInfo tempInfo;
		bool hitAnything{ false };
		double closestHit{ rayT.max };

		for (const auto& object : objects)
		{
			if (object->hit(r, Interval{ rayT.min, closestHit }, tempInfo))
			{
				hitAnything = true;
				closestHit = tempInfo.t;
			}
		}

		info = tempInfo;

		return hitAnything;
	}
};
