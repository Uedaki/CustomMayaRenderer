#pragma once

#include <vector>

#include "Mesh.h"

struct Ray;
struct HitRecord;

class Collection
{
public:
	Collection();

	bool hit(const Ray &ray, float min, float max, HitRecord &record) const;

private:
	std::vector<Mesh> meshes;
};