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
		int b;
		int c;
	};

public:
	Mesh(MFnMesh &mMesh);

	bool hit(const Ray &ray, float min, float max, HitRecord &record) const;

private:
	std::string name;
	std::vector<glm::vec3> vertices;
	std::vector<Triangle> indices;
	Material *material;
};