#include "pch.h"
#include "Collection.h"

#include <Maya/MItDag.h>
#include <Maya/MFnMesh.h>

#include "HitRecord.h"

Collection::Collection()
{
	for (MItDag it; !it.isDone(); it.next())
	{
		MObject obj = it.currentItem();
		if (obj.apiType() == MFn::kMesh)
		{
			MFnMesh mesh(obj);
			meshes.emplace_back(mesh);
		}
	}
}

bool Collection::hit(const Ray &ray, float min, float max, HitRecord &record) const
{
	bool hasHit = false;
	HitRecord tmpRecord;
	tmpRecord.distance = max;
	for (auto &mesh : meshes)
	{
		if (mesh.hit(ray, min, tmpRecord.distance, tmpRecord))
		{
			record = tmpRecord;
			hasHit = true;
		}
	}
	return (hasHit);
}