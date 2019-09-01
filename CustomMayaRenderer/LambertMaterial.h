#pragma once

#include "Material.h"

class LambertMaterial : public Material
{
public:
	static Material *create(MObject mShaderNode);

	LambertMaterial() = default;
	LambertMaterial(const glm::vec3 &newAlbedo);

	bool scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const override;

private:
	glm::vec3 albedo = {};

	glm::vec3 randomInUnitSphere() const;
};