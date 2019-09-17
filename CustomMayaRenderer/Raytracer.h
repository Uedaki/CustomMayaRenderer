#pragma once

#include <glm/glm.hpp>

class Camera;
class Collection;
class IHitable;
struct Ray;

struct RegionLimit
{
	int left;
	int right;
	int top;
	int bottom;
};

class Raytracer
{
public:
	//static void computeRegion(const RegionLimit &limit, const Camera &camera, const Collection &collection, RV_PIXEL *region);

	static glm::vec3 computeRayColor(const Ray &ray, const Collection &collection, float min, float max, int step);
};