#pragma once

#include <Maya/MString.h>

#include <glm/glm.hpp>

struct Ray;

class Camera
{
public:
	Camera(const MString &camName);

	Ray getRay(float s, float t);

private:
	glm::vec3 origin;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 leftCorner;

	glm::vec3 u;
	glm::vec3 v;

	float vLensRadius;
	float hLensRadius;
};