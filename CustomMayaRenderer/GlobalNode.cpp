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

MObject GlobalsNode::minDist;
MObject GlobalsNode::maxDist;
MObject GlobalsNode::samples;
MObject GlobalsNode::maxBounces;

void *GlobalsNode::creator()
{
	return (new GlobalsNode);
}

MStatus GlobalsNode::initialize()
{
	MStatus status;
	MFnNumericAttribute numAttr;

	minDist = numAttr.create("minDist", "minDist", MFnNumericData::kFloat, 0.1, &status);
	CHECK_MSTATUS(status);
	numAttr.setMin(0);
	addAttribute(minDist);

	maxDist = numAttr.create("maxDist", "maxDist", MFnNumericData::kFloat, 1000000, &status);
	CHECK_MSTATUS(status);
	numAttr.setMin(0);
	addAttribute(maxDist);

	samples = numAttr.create("samples", "samples", MFnNumericData::kInt, 8, &status);
	if (status != MS::kSuccess)
		LOG_MSG("Sample attribute failed");
	numAttr.setMin(1);
	numAttr.setMax(1024);
	addAttribute(samples);

	maxBounces = numAttr.create("maxBounces", "mb", MFnNumericData::kInt, 16, &status);
	CHECK_MSTATUS(status);
	numAttr.setMin(1);
	numAttr.setMax(1024);
	addAttribute(maxBounces);

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

	LOG_MSG("Fetch context:");

	MPlug mMinDistPlug(mObj, minDist);
	mMinDistPlug.getValue(context.min);
	LOG_MSG("- Minimal distance: %f", context.min);

	MPlug mMaxDistPlug(mObj, maxDist);
	mMaxDistPlug.getValue(context.max);
	LOG_MSG("- Maximal distance: %f", context.max);

	MPlug mSamplesPlug(mObj, samples);
	mSamplesPlug.getValue(context.samples);
	LOG_MSG("- Nb samples %d", context.samples);

	MPlug mMaxBouncesPlug(mObj, maxBounces);
	mMaxBouncesPlug.getValue(context.maxBounces);
	LOG_MSG("- Max bounce %d", context.maxBounces);

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

	modifier.deleteNode(minDist);
	modifier.doIt();

	modifier.deleteNode(maxDist);
	modifier.doIt();

	modifier.deleteNode(samples);
	modifier.doIt();

	modifier.deleteNode(maxBounces);
	modifier.doIt();

	modifier.deleteNode(mObj);
	modifier.doIt();
}