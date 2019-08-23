#pragma once

#include <vector>

#include "IHitable.h"
#include "Mesh.h"

struct Ray;
struct HitRecord;

class Collection : public IHitable
{
public:
	Collection();

	bool hit(const Ray &ray, float min, float max, HitRecord &record) const override;

private:
	std::vector<Mesh> meshes;
};