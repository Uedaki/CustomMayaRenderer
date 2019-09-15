#pragma once

#include "Material.h"

class MetalMaterial : public Material
{
public:
	static Material *create(MObject mShaderNode);

	MetalMaterial() = default;
	MetalMaterial(const glm::vec3 &newAlbedo);

	bool scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const override;

private:
	glm::vec3 albedo = {};
	float fuzz = 0;
};