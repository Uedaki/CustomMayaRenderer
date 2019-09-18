#include "pch.h"
#include "RenderRegionProcedure.h"

#include <Maya/MArgDatabase.h>
#include <Maya/MRenderView.h>
#include <Maya/MSyntax.h>

#include "utils.h"

void *RenderRegionProcedure::creator()
{
	return (new RenderRegionProcedure);
}

MStatus RenderRegionProcedure::doIt(const MArgList &args)
{
	LOG_MSG("Render region procedure");
	if (!MRenderView::doesRenderEditorExist())
		return (MS::kFailure);

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

	unsigned int left;
	unsigned int right;
	unsigned int bottom;
	unsigned int top;
	MRenderView::getRenderRegion(left, right, bottom, top);
	LOG_MSG("left %d right %d bottom %d top %d", left, right, bottom, top);

	MRenderView::startRegionRender(width, height, left, right, bottom, top);

	MRenderView::endRender();
	return (MS::kSuccess);
}

MSyntax RenderRegionProcedure::parametersSyntax()
{
	MSyntax syntax;
	syntax.addFlag("-c", "-camera", MSyntax::kString);
	syntax.addFlag("-w", "-width", MSyntax::kLong);
	syntax.addFlag("-h", "-height", MSyntax::kLong);
	syntax.addFlag("-a", "-action", MSyntax::kString);
	return syntax;
}