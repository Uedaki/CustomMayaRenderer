#pragma once

#include <Maya/MString.h>

#include <glm/glm.hpp>

struct Ray;

class Camera
{
public:
	Camera(const MString &camName);

	Ray getRay(float u, float v);

private:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 lowerLeft;

	float aspect;
	float hAperture;
	float vAperture;
	float focusDistance;
};