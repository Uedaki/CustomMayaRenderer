#pragma once

#include "Material.h"

class DielectricMaterial : public Material
{
public:
	static Material *create(MObject mShaderNode);

	DielectricMaterial() = default;

	bool scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const override;

private:
	float ri = 0;


};