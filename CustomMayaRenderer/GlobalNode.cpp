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

Context GlobalsNode::context;

MObject GlobalsNode::samples;

void *GlobalsNode::creator()
{
	return (new GlobalsNode);
}

MStatus GlobalsNode::initialize()
{
	MStatus status;
	MFnNumericAttribute numAttr;

	samples = numAttr.create("samples", "samples", MFnNumericData::kInt, 8, &status);
	if (status != MS::kSuccess)
		LOG_MSG("Sample attribute failed");
	numAttr.setMin(1);
	numAttr.setSoftMax(1024);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	addAttribute(samples);

	return (MS::kSuccess);
}

MStatus GlobalsNode::compute(const MPlug &plug, MDataBlock &data)
{
	return (MS::kSuccess);
}

const Context &GlobalsNode::fetchContext()
{
	MObject mObj;
	if (getDependencyNodeByName(GlobalsNode::name, mObj) != MS::kSuccess)
		return (context);

	MPlug mPlug(mObj, samples);
	mPlug.getValue(context.samples);
	LOG_MSG("sample %d", context.samples);

	return (context);
}

const Context &GlobalsNode::getContext()
{
	return (context);
}

void GlobalsNode::clean()
{
	MObject mObj;
	getDependencyNodeByName(GlobalsNode::name, mObj);
	MDGModifier modifier;
	modifier.deleteNode(samples);
	modifier.doIt();
	modifier.deleteNode(mObj);
	modifier.doIt();
}