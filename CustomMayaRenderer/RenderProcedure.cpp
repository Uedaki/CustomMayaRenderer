#include "pch.h"
#include "RenderProcedure.h"

#include <Maya/MArgDatabase.h>
#include <Maya/MArgList.h>
#include <Maya/MRenderView.h>
#include <Maya/MSyntax.h>

#include "Camera.h"
#include "Collection.h"
#include "GlobalsNode.h"
#include "HitRecord.h"
#include "Ray.h"
#include "utils.h"

void *RenderProcedure::creator()
{
	return (new RenderProcedure);
}

MStatus RenderProcedure::doIt(const MArgList &args)
{
	LOG_MSG("Render procedure");

	if (!MRenderView::doesRenderEditorExist())
		return (MS::kFailure);

	int width = 1;
	int height = 1;
	MString cameraName;

	MArgDatabase argsData(parametersSyntax(), args);		
	if (!argsData.isFlagSet("-width")
		|| !argsData.isFlagSet("-height")
		|| !argsData.isFlagSet("-camera"))
	{
		LOG_MSG("Wrong number of argument passed to render procedure");
		return (MS::kFailure);
	}
	argsData.getFlagArgument("-width", 0, width);
	argsData.getFlagArgument("-height", 0, height);
	argsData.getFlagArgument("-camera", 0, cameraName);
	LOG_MSG("Width: %d Height: %d Camera: %s", width, height, cameraName.asChar());

	Camera camera(cameraName);
	Collection collection;

	glm::vec3 leftCorner(-2, -1, -1);
	glm::vec3 vertical(0, 2, 0);
	glm::vec3 horizontal(4, 0, 0);
	glm::vec3 origin(0, 0, 0);

	MRenderView::startRender(width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			RV_PIXEL pix;
			float u = static_cast<float>(x) / width;
			float v = static_cast<float>(y) / height;
			//Ray ray = camera.getRay(u, v);

			HitRecord record;

			Ray ray;
			ray.origin = origin;
			ray.direction = leftCorner + u * horizontal + v * vertical;
			if (collection.hit(ray, 0.1f, 1000, record))
			{
				//LOG_MSG("hit x: %d y: %d", x, y);
				//LOG_MSG("distance impact %f", record.distance);
				pix.r = 128 + record.normal.x * 127;
				pix.g = 128 + record.normal.y * 127;
				pix.b = 128 + record.normal.z * 127;
			}
			else
			{
				pix.r = 0;
				pix.g = 0;
				pix.b = 0;
			}
			pix.a = 255;
			MRenderView::updatePixels(x, x + 1, y, y + 1, &pix);
		}
	}
	MRenderView::endRender();
	return (MS::kSuccess);
}

MSyntax RenderProcedure::parametersSyntax()
{
	MSyntax syntax;
	syntax.addFlag("-c", "-camera", MSyntax::kString);
	syntax.addFlag("-w", "-width", MSyntax::kLong);
	syntax.addFlag("-h", "-height", MSyntax::kLong);
	syntax.addFlag("-a", "-action", MSyntax::kString);
	return syntax;
}