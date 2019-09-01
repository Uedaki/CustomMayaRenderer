#include "pch.h"
#include "Hitable.h"

#include <chrono>

#include "Ray.h"
#include "utils.h"

namespace
{
	uint64_t nbr = 0;
	double avg = 0;
}

bool Hitable::intersectRay(const Ray &ray, float min, float max) const
{
	//std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

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
		//std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		//avg = (avg * nbr + std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / (nbr + 1);
		//nbr += 1;
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
		//std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		//avg = (avg * nbr + std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / (nbr + 1);
		//nbr += 1;
		return (false);
	}
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	//std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	//avg = (avg * nbr + std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / (nbr + 1);
	//nbr += 1;

	if (min < tmin && tmax < max)
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

void Hitable::avgTime()
{
	LOG_MSG("Volume test as been done %lu with an average time of %f", nbr, avg);
}