#pragma once

#include <Maya/MPxNode.h>

#include "Context.h"

class GlobalsNode : public MPxNode
{
public:
	static const MString name;
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();

	MStatus compute(const MPlug &plug, MDataBlock &dataBlock) override;

	static const Context &fetchContext();
	static const Context &getContext();

	static void clean();

private:
	static Context context;

	static MObject samples;
};