#pragma once

#include <glm/glm.hpp>

class Collection;
class IHitable;
struct Ray;

class Raytracer
{
public:
	static glm::vec3 computeRayColor(const Ray &ray, const Collection &collection, int step);
};