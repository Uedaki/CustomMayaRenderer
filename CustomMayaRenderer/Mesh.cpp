#include "pch.h"
#include "Mesh.h"

#include <Maya/MDagPath.h>
#include <Maya/MFloatVectorArray.h>
#include <Maya/MIntArray.h>
#include <Maya/MMatrix.h>
#include <Maya/MPointArray.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/ext.hpp>

#include <algorithm>
#include <chrono>

#include "HitRecord.h"
#include "Material.h"
#include "Ray.h"
#include "utils.h"

Mesh::Mesh(MFnMesh &mMesh)
	: name(mMesh.name().asChar())
{

	MDagPath mDag;
	MMatrix mMatrix;
	if (mMesh.getPath(mDag) == MS::kSuccess)
		mMatrix = mDag.inclusiveMatrix();

	MPointArray mVertices;
	mMesh.getPoints(mVertices);
	vertices.resize(mVertices.length());
	MPoint mLowerBound = mVertices[0] * mMatrix;
	MPoint mUpperBound = mVertices[0] * mMatrix;
	for (unsigned int i = 0; i < mVertices.length(); i++)
	{
		MPoint point = mVertices[i] * mMatrix;
		vertices[i] = glm::vec3(point.x, point.y, point.z);

		mLowerBound.x = std::min(mLowerBound.x, point.x);
		mLowerBound.y = std::min(mLowerBound.y, point.y);
		mLowerBound.z = std::min(mLowerBound.z, point.z);

		mUpperBound.x = std::max(mUpperBound.x, point.x);
		mUpperBound.y = std::max(mUpperBound.y, point.y);
		mUpperBound.z = std::max(mUpperBound.z, point.z);
	}
	setBound(BoundType::LOWER_BOUND, glm::vec3(mLowerBound.x, mLowerBound.y, mLowerBound.z));
	setBound(BoundType::UPPER_BOUND, glm::vec3(mUpperBound.x, mUpperBound.y, mUpperBound.z));

	MIntArray mTrianglesCount;
	MIntArray mVerticesIndice;
	mMesh.getTriangles(mTrianglesCount, mVerticesIndice);
	indices.resize(mVerticesIndice.length());
	for (unsigned int i = 0; i + 2 < mVerticesIndice.length(); i = i + 3)
	{
		indices[i] = { mVerticesIndice[i], mVerticesIndice[i + 1], mVerticesIndice[i + 2]};
	}

	LOG_MSG("Object %s recorded with %d triangles using %d vertices", name, mVerticesIndice.length() / 3, mVertices.length());


	MObjectArray mShaderArray;
	MIntArray mShaderIndices;
	mMesh.getConnectedShaders(0, mShaderArray, mShaderIndices);
	if (mShaderArray.length() >= 1)
	{
		MFnDependencyNode mShaderWrapperNode(mShaderArray[0]);
		MString mShaderName = mShaderWrapperNode.name().substring(0, mShaderWrapperNode.name().length() - 3);
		material = Material::create(mShaderName);
	}
	else
	{
		material = Material::create();
	}
}

bool Mesh::hit(const Ray &ray, float min, float max, HitRecord &record) const
{ 
	if (!intersectRay(ray, min, max))
		return (false);

	bool hasHit = false;
	for (auto &indice : indices)
	{		
		glm::vec3 e1 = vertices[indice.b] - vertices[indice.a];
		glm::vec3 e2 = vertices[indice.c] - vertices[indice.a];
		glm::vec3 normal = glm::cross(e1, e2);
		if (glm::dot(ray.direction, normal) > 0)
			continue;

		float t;
		glm::vec2 coord;
		if (glm::intersectRayTriangle(ray.origin, ray.direction, vertices[indice.a], vertices[indice.b], vertices[indice.c], coord, t))
		{
			if (min < t && t < max)
			{
				record.distance = t;
				record.position = vertices[indice.a] + coord.x * e1 + coord.y * e2;
				record.normal = glm::normalize(normal);
				record.material = material;
				max = t;
				hasHit = true;
			}
		}
	}
	return (hasHit);
}