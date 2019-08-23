#pragma once

#include <Maya/MPxNode.h>

class GlobalsNode : public MPxNode
{
public:
	static const MString name;
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();
	static void clean();

	MStatus compute(const MPlug &plug, MDataBlock &dataBlock) override;

	static MObject width;
	static MObject height;
	static MObject nbSamples;
};