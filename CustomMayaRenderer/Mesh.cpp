#include "pch.h"
#include "Mesh.h"

#include <Maya/MDagPath.h>
#include <Maya/MFloatVectorArray.h>
#include <Maya/MIntArray.h>
#include <Maya/MMatrix.h>
#include <Maya/MPointArray.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

#include "HitRecord.h"
#include "Ray.h"
#include "utils.h"

Mesh::Mesh(MFnMesh &obj)
{
	MDagPath dag;
	MMatrix mMatrix;
	if (obj.getPath(dag) == MS::kSuccess)
		mMatrix = dag.inclusiveMatrix();

	MPointArray mVertices;
	obj.getPoints(mVertices);
	LOG_MSG("nbr vertice %d", mVertices.length());
	for (unsigned int i = 0; i < mVertices.length(); i++)
	{
		MPoint point = mVertices[i] * mMatrix;
		vertices.emplace_back(point[0], point[1], point[3]);
	}

	unsigned int i = 0;
	MIntArray mTrianglesCount;
	MIntArray mVerticesIndice;
	obj.getTriangles(mTrianglesCount, mVerticesIndice);
	for (unsigned int faceIdx = 0; faceIdx < mTrianglesCount.length(); faceIdx++)
	{
		unsigned int maxIdx = i + mTrianglesCount[faceIdx];
		for (; i < maxIdx; i++)
		{
			MVector n1;
			MVector n2;
			MVector n3;
			obj.getFaceVertexNormal(faceIdx, i, n1, MSpace::kWorld);
			obj.getFaceVertexNormal(faceIdx, i, n2, MSpace::kWorld);
			obj.getFaceVertexNormal(faceIdx, i, n3, MSpace::kWorld);
			indices.push_back({ mVerticesIndice[i * 3], mVerticesIndice[i * 3 + 1], mVerticesIndice[i * 3 + 2],
				{ n1.x, n1.y, n1.z }, { n2.x, n2.y, n2.z }, { n3.x, n3.y, n3.z }});
		}
	}

	LOG_MSG("nbr vertices indices %d", mVerticesIndice.length());
	LOG_MSG("nbr vertices c indices %d", mTrianglesCount.length());
}

bool Mesh::hit(const Ray &ray, float min, float max, HitRecord &record) const
{
	bool hasHit = false;
	for (auto &indice : indices)
	{
		float trash;
		glm::vec2 impactCoord;
		if (glm::intersectRayTriangle(ray.origin, ray.direction, vertices[indice.a], vertices[indice.b], vertices[indice.c], impactCoord, trash))
		{
			glm::vec3 impact = vertices[indice.a] + impactCoord.x * vertices[indice.b] + impactCoord.y * vertices[indice.c];
			float distance = glm::length(impact - ray.origin);
			if (min < distance && distance < max)
			{
				record.distance = distance;
				record.normal = indice.na;
				max = distance;
				hasHit = true;
			}
		}
	}
	return (hasHit);
}