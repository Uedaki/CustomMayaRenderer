#pragma once

#include <Maya/MPxCommand.h>

class MString;
class MSyntax;

class RenderProcedure : public MPxCommand
{
public:
	static void *creator();

	MStatus doIt(const MArgList &arg);
	void startRendering(int width, int height, MString mCameraName);
private:
	MSyntax parametersSyntax();
};