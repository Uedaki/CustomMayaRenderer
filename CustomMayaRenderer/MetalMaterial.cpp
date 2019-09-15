#include "pch.h"
#include "MetalMaterial.h"

#include <Maya/MFloatVector.h>
#include <Maya/MObject.h>
#include <Maya/MPlug.h>

#include "HitRecord.h"
#include "MetalNode.h"
#include "Ray.h"
#include "utils.h"

Material *MetalMaterial::create(MObject mShaderNode)
{
	LOG_MSG("Metal material created");

	MetalMaterial *mat = new MetalMaterial;

	MPlug mAlbedoPlug(mShaderNode, MetalNode::aAlbedo);
	MFloatVector mAlbedo = mAlbedoPlug.asMDataHandle().asFloatVector();
	mat->albedo = glm::vec3(mAlbedo.x, mAlbedo.y, mAlbedo.z);
	LOG_MSG("albedo %f %f %f", mAlbedo.x, mAlbedo.y, mAlbedo.z);

	MPlug mFuzzPlug(mShaderNode, MetalNode::aFuzz);
	mat->fuzz = mFuzzPlug.asFloat();

	return (mat);
}

MetalMaterial::MetalMaterial(const glm::vec3 &newAlbedo)
	: albedo(newAlbedo)
{}

bool MetalMaterial::scatter(const Ray &ray, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const
{
	glm::vec3 reflected = reflect(glm::normalize(ray.direction), hit.normal);
	scattered = Ray(hit.position, reflected + fuzz * randomInUnitSphere());
	attenuation = albedo;
	return (glm::dot(scattered.direction, hit.normal) > 0);
}