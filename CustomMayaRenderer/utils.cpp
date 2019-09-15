#include "pch.h"
#include "utils.h"

#include <Maya/MSelectionList.h>

#include <random>

float ctmRand()
{
	static uint32_t x = 123456789;
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	w = w ^ (w >> 19) ^ (t ^ (t >> 8));
	return (static_cast<float>(w % 1000) / 1000);
}

glm::vec3 reflect(const glm::vec3 &v, const glm::vec3 &n)
{
	return (v - 2 * glm::dot(v, n) * n);
}

bool refract(const glm::vec3 &v, const glm::vec3 &n, float niOverNt, glm::vec3 &refracted)
{
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return (true);
	}
	else
		return (false);
}

float schlick(float cosine, float refIdx)
{
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow(1 - cosine, 5));
}

glm::vec3 randomUnitDisk()
{
	glm::vec3 p;
	do
	{
		p = 2.0f * glm::vec3(ctmRand(), ctmRand(), 0) - glm::vec3(1, 1, 0);
	} while (glm::dot(p, p) <= 1.0f);
	return (p);
}

glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(ctmRand(),
			ctmRand(),
			ctmRand()) - glm::vec3(1, 1, 1);
	} while (glm::length(p) >= 1);
	return (p);
}

MStatus getDependencyNodeByName(const MString &name, MObject &node)
{
	MSelectionList selList;
	selList.add(name);

	if (selList.isEmpty())
		return MS::kFailure;

	return selList.getDependNode(0, node);
}