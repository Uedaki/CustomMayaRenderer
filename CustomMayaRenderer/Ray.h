#pragma once

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 invDir;
	int sign[3];

	Ray() = default;

	Ray(const glm::vec3 &origin, const glm::vec3 &newDirection)
		: origin(origin), direction(newDirection)
	{
		direction = glm::normalize(direction);
		invDir = 1.0f / direction;
		sign[0] = (invDir.x < 0);
		sign[1] = (invDir.y < 0);
		sign[2] = (invDir.z < 0);
	}

	glm::vec3 pointAt(float time) const
	{
		return (origin + time * direction);
	}
};