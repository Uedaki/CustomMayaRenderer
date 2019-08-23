#include "pch.h"
#include "RenderRegionProcedure.h"

#include <Maya/MRenderView.h>

#include "utils.h"

void *RenderRegionProcedure::creator()
{
	return (new RenderRegionProcedure);
}

MStatus RenderRegionProcedure::doIt(const MArgList &arg)
{
	LOG_MSG("Render region procedure");
	if (!MRenderView::doesRenderEditorExist())
		return (MS::kFailure);

	unsigned int width = 300;
	unsigned int height = 300;

	unsigned int left;
	unsigned int right;
	unsigned int bottom;
	unsigned int top;
	STATUS_BARRIER(MRenderView::getRenderRegion(left, right, bottom, top));

	MRenderView::startRegionRender(width, height, left, right, bottom, top);
	for (unsigned int y = bottom; y < top; y++)
	{
		for (unsigned int x = left; x < right; x++)
		{
			RV_PIXEL pix;
			pix.a = 255;
			pix.b = (float)x / width * 255;
			pix.r = (float)y / height * 255;
			pix.g = 0;
			MRenderView::updatePixels(x, x + 1, y, y + 1, &pix);
		}
	}
	MRenderView::endRender();
	return (MS::kSuccess);
}