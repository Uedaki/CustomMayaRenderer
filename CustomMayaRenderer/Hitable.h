#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

struct Ray;

class Hitable
{
public:
	enum class BoundType : uint8_t
	{
		LOWER_BOUND = 0,
		UPPER_BOUND = 1
	};

	static void avgTime();

	bool intersectRay(const Ray &ray, float min, float max) const;

	void setBound(BoundType type, const glm::vec3 &value);
	const glm::vec3 &getBound(BoundType type) const;

private:
	glm::vec3 bounds[2];
};