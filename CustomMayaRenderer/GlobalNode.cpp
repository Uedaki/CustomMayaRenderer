#include "pch.h"
#include "GlobalsNode.h"

#include <Maya/MDGModifier.h>
#include <Maya/MFnDependencyNode.h>
#include <Maya/MFnNumericAttribute.h>
#include <Maya/MPlug.h>
#include <Maya/MPlugArray.h>
#include <Maya/MSelectionList.h>

#include "utils.h"

const MString GlobalsNode::name("customMayaRendererGlobalsNode");
const MTypeId GlobalsNode::id(0x7f7f7);

MObject GlobalsNode::width;
MObject GlobalsNode::height;
MObject GlobalsNode::nbSamples;

namespace
{
	MStatus getDependencyNodeByName(const MString &name, MObject &node)
	{
		MSelectionList selList;
		selList.add(name);

		if (selList.isEmpty())
			return MS::kFailure;

		return selList.getDependNode(0, node);
	}
}

void *GlobalsNode::creator()
{
	LOG_MSG("GlobalsNode created");
	return (new GlobalsNode);
}

MStatus GlobalsNode::initialize()
{
	LOG_MSG("GlobalsNode initialize");

	MStatus status;
	MFnNumericAttribute numAttr;

	width = numAttr.create("width", "width", MFnNumericData::kInt, 400, &status);
	if (status != MS::kSuccess)
		LOG_MSG("Width attribute failed");
	numAttr.setMin(1);
	addAttribute(width);


	height = numAttr.create("height", "height", MFnNumericData::kInt, 400, &status);
	if (status != MS::kSuccess)
		LOG_MSG("Height attribute failed");
	numAttr.setMin(1);
	addAttribute(height);

	nbSamples = numAttr.create("nbSample", "nbSample", MFnNumericData::kInt, 10, &status);
	if (status != MS::kSuccess)
		LOG_MSG("Sample attribute failed");
	numAttr.setMin(1);
	addAttribute(nbSamples);

	return (MS::kSuccess);
}

void GlobalsNode::clean()
{
	MObject obj;
	if (getDependencyNodeByName(GlobalsNode::name, obj) != MS::kSuccess)
		return;
	
	MDGModifier modifier;
	modifier.deleteNode(obj);
}

MStatus GlobalsNode::compute(const MPlug &plug, MDataBlock &data)
{
	LOG_MSG("Compute");
	LOG_MSG("%d", data.inputValue(width).asInt());
	LOG_MSG("%d", data.inputValue(height).asInt());
	return (MS::kSuccess);
}