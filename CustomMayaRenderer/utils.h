#pragma once

#include <Maya/MStatus.h>

#include <glm/glm.hpp>

#include <string>

#define LOG_MSG(a, ...) printMsg(a, __VA_ARGS__)

#ifdef _DEBUG
#define DEBUG_MSG(a, ...) printMsg(a, __VA_ARGS__)
#else
#define DEBUG_MSG(a, ...)
#endif

template <typename ...Args>
void printMsg(const char *msg, Args... args)
{
	printf("CustomMayaRenderer | ");
	printf(msg, args...);
	printf("\n");
}

float ctmRand();

glm::vec3 reflect(const glm::vec3 &v, const glm::vec3 &n);
bool refract(const glm::vec3 &v, const glm::vec3 &n, float niOverNt, glm::vec3 &refracted);
float schlick(float cosine, float refIdx);

glm::vec3 randomUnitDisk();
glm::vec3 randomInUnitSphere();

MStatus getDependencyNodeByName(const MString &name, MObject &node);