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
	if (!MRenderView::doesRenderEditorExist())
		return (MS::kFailure);

	int width = 1;
	int height = 1;
	MString mCameraName;

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
	argsData.getFlagArgument("-camera", 0, mCameraName);

	startRendering(width, height, mCameraName);

	return (MS::kSuccess);
}

void RenderProcedure::startRendering(int width, int height, MString mCameraName)
{
	LOG_MSG("----    Render procedure    ----");
	LOG_MSG("Image rendered is %dx%d with %s camera", width, height, mCameraName.asChar());

	MRenderView::startRender(width, height);

	Camera camera(mCameraName);
	Collection collection;
	const Context &context = GlobalsNode::fetchContext();

	int step = 42;
	RV_PIXEL *pix = new RV_PIXEL[static_cast<size_t>(width) * height];
	memset(pix, 0, static_cast<size_t>(width) * height * sizeof(RV_PIXEL));
	for (float s = 0; s < context.samples; s += 1)
	{
		LOG_MSG("Rendering sample %f", s);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int idx = x + y * width;
				float u = (static_cast<float>(x) + ctmRand()) / width;
				float v = (static_cast<float>(y) + ctmRand()) / height;

				Ray ray = camera.getRay(u, v);
				glm::vec3 color = Raytracer::computeRayColor(ray, collection, context.min, context.max, 0);

				pix[idx].r = (pix[idx].r * s + color.r) / (s + 1);
				pix[idx].g = (pix[idx].g * s + color.g) / (s + 1);
				pix[idx].b = (pix[idx].b * s + color.b) / (s + 1);
				pix[idx].a = 255;
			}
		}
		MRenderView::updatePixels(0, width - 1, 0, height - 1, pix);
		MRenderView::refresh(0, width - 1, 0, height - 1);
	}
	delete[] pix;

	MRenderView::endRender();
	LOG_MSG("-> Rendering finished!");
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