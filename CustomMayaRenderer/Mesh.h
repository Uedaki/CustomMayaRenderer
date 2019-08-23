#pragma once

#include <Maya/MFnMesh.h>

#include <glm/glm.hpp>

#include <vector>

#include "IHitable.h"

struct Ray;
struct HitRecord;

class Mesh : public IHitable
{
	struct Triangle
	{
		int a;
		int b;
		int c;
		glm::vec3 na;
		glm::vec3 nb;
		glm::vec3 nc;
	};

public:
	Mesh(MFnMesh &obj);

	bool hit(const Ray &ray, float min, float max, HitRecord &record) const override;

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<Triangle> indices;
};