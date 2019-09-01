#include "pch.h"
#include "LambertNode.h"

#include <Maya/MFloatVector.h>
#include <Maya/MFnNumericAttribute.h>
#include <Maya/MFnStringData.h>
#include <Maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>

#include "utils.h"

const MString LambertNode::name("customMayaRendererLambert");
const MTypeId LambertNode::id(0x7f7f8);

MObject  LambertNode::nameData;
MObject  LambertNode::nameAttr;

MObject  LambertNode::aAlbedo;
MObject  LambertNode::aAlbedoR;
MObject  LambertNode::aAlbedoG;
MObject  LambertNode::aAlbedoB;
MObject  LambertNode::aOutColor;
MObject  LambertNode::aOutColorR;
MObject  LambertNode::aOutColorG;
MObject  LambertNode::aOutColorB;

MObject LambertNode::samples;

void *LambertNode::creator()
{
	return (new LambertNode);
}

MStatus LambertNode::initialize()
{
	MStatus status;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute typedAttr;

	MFnStringData strData;
	MObject nameData = strData.create();
	strData.set(name);
	nameAttr = typedAttr.create("materialName", "mn", MFnData::kString, nameData, &status);

	aAlbedoR = nAttr.create("albedoR", "ar", MFnNumericData::kFloat, 0,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.0f));
	aAlbedoG = nAttr.create("albedoG", "ag", MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.58824f));
	aAlbedoB = nAttr.create("albedoB", "ab", MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.644f));
	aAlbedo = nAttr.create("albedo", "a", aAlbedoR, aAlbedoG, aAlbedoB, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));
	CHECK_MSTATUS(nAttr.setDefault(0.0f, 0.58824f, 0.644f));
	CHECK_MSTATUS(nAttr.setUsedAsColor(true));

	aOutColorR = nAttr.create("outColorR", "ocr", MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	aOutColorG = nAttr.create("outColorG", "ocg", MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	aOutColorB = nAttr.create("outColorB", "ocb", MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	aOutColor = nAttr.create("outColor", "oc", aOutColorR, aOutColorG, aOutColorB, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setHidden(false));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));

	CHECK_MSTATUS(addAttribute(nameAttr));
	CHECK_MSTATUS(addAttribute(aAlbedo));
	CHECK_MSTATUS(addAttribute(aOutColor));

	CHECK_MSTATUS(attributeAffects(aAlbedoR, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedoG, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedoB, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedo, aOutColor));

	return(MS::kSuccess);
}

MStatus LambertNode::compute(const MPlug &plug, MDataBlock &block)
{
	MStatus status;
	MFloatVector resultColor(0.0, 0.0, 0.0);

	MFloatVector &surfaceColor = block.inputValue(aAlbedo,
		&status).asFloatVector();
	CHECK_MSTATUS(status);

	resultColor = surfaceColor;
	if (plug == aOutColor || plug == aOutColorR || plug == aOutColorG
		|| plug == aOutColorB)
	{
		MDataHandle outColorHandle = block.outputValue(aOutColor,
			&status);
		CHECK_MSTATUS(status);
		MFloatVector &outColor = outColorHandle.asFloatVector();

		outColor = resultColor;
		outColorHandle.setClean();
	}
	return(MS::kSuccess);
}

void LambertNode::postConstructor()
{
	setMPSafe(true);
}