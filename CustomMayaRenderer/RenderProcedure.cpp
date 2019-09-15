#include "pch.h"
#include "RenderProcedure.h"

#include <Maya/MArgDatabase.h>
#include <Maya/MArgList.h>
#include <Maya/MRenderView.h>
#include <Maya/MSyntax.h>

#include "Camera.h"
#include "Collection.h"
#include "GlobalsNode.h"
#include "Ray.h"
#include "Raytracer.h"
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

	const Context &context = GlobalsNode::fetchContext();
	MRenderView::startRender(width, height);
	LOG_MSG("Start rendering");
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			RV_PIXEL pix = {};
			for (int s = 0; s < context.samples; s++)
			{
				float u = static_cast<float>(x + ctmRand()) / width;
				float v = static_cast<float>(y + ctmRand()) / height;

				Ray ray = camera.getRay(u, v);
				glm::vec3 color = Raytracer::computeRayColor(ray, collection, 0);
				pix.r += color[0];
				pix.g += color[1];
				pix.b += color[2];
				pix.a = 255;
			}
			pix.r = pix.r / context.samples;
			pix.g = pix.g / context.samples;
			pix.b = pix.b / context.samples;
			MRenderView::updatePixels(x, x + 1, y, y + 1, &pix);
		}
	}
	MRenderView::endRender();
	//Hitable::avgTime();
	//Mesh::avgTime();
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