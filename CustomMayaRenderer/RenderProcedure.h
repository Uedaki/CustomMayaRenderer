#pragma once

#include <Maya/MPxCommand.h>

class MSyntax;

class RenderProcedure : public MPxCommand
{
public:
	static void *creator();

	MStatus doIt(const MArgList &arg);

private:
	MSyntax parametersSyntax();
};