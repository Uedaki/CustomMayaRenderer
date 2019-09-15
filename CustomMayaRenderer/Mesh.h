#pragma once

#include <Maya/MFnMesh.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Hitable.h"
#include "Material.h"

struct Ray;
struct HitRecord;

class Mesh : public Hitable
{
	struct Triangle
	{
		int a;
		int na;
		int b;
		int nb;
		int c;
		int nc;
	};

public:
	Mesh(MFnMesh &mMesh);

	bool hit(const Ray &ray, float min, float max, HitRecord &record) const;

private:
	std::string name;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<Triangle> indices;
	Material *material;
};