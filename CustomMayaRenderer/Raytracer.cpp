#include "pch.h"
#include "Raytracer.h"

#include <Maya/MRenderView.h>

#include <glm/glm.hpp>

#include "Collection.h"
#include "GlobalsNode.h"
#include "HitRecord.h"
#include "Ray.h"
#include "utils.h"

glm::vec3 Raytracer::computeRayColor(const Ray &ray, const Collection &collection, float min, float max, int depth)
{
	HitRecord record;
	glm::vec3 color = {};

	if (collection.hit(ray, 0, 1000000, record))
	{
		Ray scattered;
		glm::vec3 attenuation = {};
		if (!record.material)
			LOG_MSG("Missing material parameter");
		if (depth < GlobalsNode::getContext().maxBounces && record.material && record.material->scatter(ray, record, attenuation, scattered))
			color = attenuation * computeRayColor(scattered, collection, 0, max, depth + 1);
		else
			return (glm::vec3(0, 0, 0));
	}
	else
	{
		float t = 0.5f * (glm::normalize(ray.direction).y + 1);
		color = (1 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1);
		color = 255.0f * color;
	}
	return (color);
}
