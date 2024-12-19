#pragma once

#include <memory>

#include "Ray.h"
#include "VectorAddons.h"
#include "Interval.h"

class Material; // forward declaration of class to avoid circular includes

struct HitInfo
{
	Vector3d point;
	Vector3d normal;

	std::shared_ptr<Material> mat;

	// to calculate hit only if t is in a range
	// this essentially defines a set of surfaces in which intersection is valid as ray direction is normalized
	// if t = 1, it defines the viewport surface
	double t;
	bool frontFace;

	void setFaceNormal(const Ray& r, const Vector3d& outwardNormal)
	{
		frontFace = VectorAddons::dotProduct(r.direction, outwardNormal) < 0.0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval rayT, HitInfo& info) const = 0;
};

