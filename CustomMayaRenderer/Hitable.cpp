#include "pch.h"
#include "Hitable.h"

#include <chrono>

#include "Ray.h"
#include "utils.h"

bool Hitable::intersectRay(const Ray &ray, float min, float max) const
{
	float tmin;
	float tmax;
	float tymin;
	float tymax;
	float tzmin;
	float tzmax;

	tmin = (bounds[ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
	tmax = (bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
	tymin = (bounds[ray.sign[1]].y - ray.origin.y) * ray.invDir.y;
	tymax = (bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.invDir.y;

	if (tmin > tymax || tymin > tmax)
	{
		return (false);
	}
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[ray.sign[2]].z - ray.origin.z) * ray.invDir.z;
	tzmax = (bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.invDir.z;

	if (tmin > tzmax || tzmin > tmax)
	{
		return (false);
	}
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	if (tmin < max && min < tmax)
		return (true);
	return (false);
}


void Hitable::setBound(BoundType type, const glm::vec3 &value)
{
	bounds[static_cast<uint8_t>(type)] = value;
}

const glm::vec3 &Hitable::getBound(BoundType type) const
{
	return (bounds[static_cast<uint8_t>(type)]);
}