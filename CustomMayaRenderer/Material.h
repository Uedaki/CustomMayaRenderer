#pragma once

#include <Maya/MString.h>

#include <glm/glm.hpp>

struct HitRecord;
struct Ray;

class Material
{
public:
	static Material *create(const MString &mName = MString(""));

	virtual bool scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const = 0;

protected:
	glm::vec3 color;
};