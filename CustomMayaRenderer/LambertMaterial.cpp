#include "pch.h"
#include "LambertMaterial.h"

#include <Maya/MFloatVector.h>
#include <Maya/MObject.h>
#include <Maya/MPlug.h>

#include "HitRecord.h"
#include "LambertNode.h"
#include "Ray.h"
#include "utils.h"

Material *LambertMaterial::create(MObject mShaderNode)
{
	LOG_MSG("Lambert material created");

	LambertMaterial *mat = new LambertMaterial;

	MPlug mAlbedoPlug(mShaderNode, LambertNode::aAlbedo);
	MFloatVector mAlbedo = mAlbedoPlug.asMDataHandle().asFloatVector();
	mat->albedo = glm::vec3(mAlbedo.x, mAlbedo.y, mAlbedo.z);
	LOG_MSG("albedo %f %f %f", mAlbedo.x, mAlbedo.y, mAlbedo.z);

	return (mat);
}

LambertMaterial::LambertMaterial(const glm::vec3 &newAlbedo)
	: albedo(newAlbedo)
{}

bool LambertMaterial::scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const
{
	glm::vec3 target = hit.position + hit.normal + randomInUnitSphere();
	scattered = Ray(hit.position, target - hit.position);
	attenuation = albedo;
	return (true);
}