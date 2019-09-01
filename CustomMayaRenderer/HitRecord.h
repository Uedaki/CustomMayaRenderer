#pragma once

#include <glm/glm.hpp>

#include "Material.h"

struct HitRecord
{
	float distance;
	glm::vec3 position;
	glm::vec3 normal;
	Material *material;
};