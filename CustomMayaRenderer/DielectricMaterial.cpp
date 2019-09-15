#include "pch.h"
#include "DielectricMaterial.h"

#include <Maya/MFloatVector.h>
#include <Maya/MObject.h>
#include <Maya/MPlug.h>

#include "HitRecord.h"
#include "DielectricNode.h"
#include "Ray.h"
#include "utils.h"

Material *DielectricMaterial::create(MObject mShaderNode)
{
	LOG_MSG("Dielectric material created");

	DielectricMaterial *mat = new DielectricMaterial;

	MPlug mRefractionPlug(mShaderNode, DielectricNode::aRefraction);
	mat->ri = mRefractionPlug.asFloat();
	LOG_MSG("refraction %f", mat->ri);
	return (mat);
}

bool DielectricMaterial::scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const
{
	glm::vec3 outwardNormal;
	glm::vec3 reflected = reflect(ray.direction, hit.normal);

	float niOverNt;
	attenuation = glm::vec3(1, 1, 1);
	glm::vec3 refracted;

	float reflectProb;
	float cosine;
	if (glm::dot(ray.direction, hit.normal) > 0)
	{
		outwardNormal = -1.0f * hit.normal;
		niOverNt = ri;
		cosine = ri * glm::dot(ray.direction, hit.normal) / glm::length(ray.direction);
	}
	else
	{
		outwardNormal = hit.normal;
		niOverNt = 1 / ri;
		cosine = -1.0f * glm::dot(ray.direction, hit.normal) / glm::length(ray.direction);
	}

	if (refract(ray.direction, outwardNormal, niOverNt, refracted))
		reflectProb = schlick(cosine, ri);
	else
	{
		scattered = Ray(hit.position, reflected);
		reflectProb = 1;
	}

	if (ctmRand() < reflectProb)
		scattered = Ray(hit.position, reflected);
	else
		scattered = Ray(hit.position, refracted);

	return (true);
}