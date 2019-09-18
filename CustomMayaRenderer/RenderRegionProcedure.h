#pragma once

#include <Maya/MPxCommand.h>

class RenderRegionProcedure : public MPxCommand
{
public:
	static void *creator();

	MStatus doIt(const MArgList &args);

	MSyntax parametersSyntax();
};