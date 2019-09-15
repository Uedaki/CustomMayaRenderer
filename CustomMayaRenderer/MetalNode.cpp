#include "pch.h"
#include "MetalNode.h"

#include <Maya/MFloatVector.h>
#include <Maya/MFnNumericAttribute.h>
#include <Maya/MFnStringData.h>
#include <Maya/MFnTypedAttribute.h>
#include <maya/MFnLightDataAttribute.h>

#include "utils.h"

const MString MetalNode::name("customMetal");
const MTypeId MetalNode::id(0x7f7f9);

MObject  MetalNode::nameData;
MObject  MetalNode::nameAttr;

MObject  MetalNode::aAlbedo;
MObject  MetalNode::aAlbedoR;
MObject  MetalNode::aAlbedoG;
MObject  MetalNode::aAlbedoB;
MObject  MetalNode::aFuzz;
MObject  MetalNode::aOutColor;
MObject  MetalNode::aOutColorR;
MObject  MetalNode::aOutColorG;
MObject  MetalNode::aOutColorB;

MObject  MetalNode::aNormalCamera;
MObject  MetalNode::aNormalCameraX;
MObject  MetalNode::aNormalCameraY;
MObject  MetalNode::aNormalCameraZ;
MObject  MetalNode::aLightData;
MObject  MetalNode::aLightDirection;
MObject  MetalNode::aLightDirectionX;
MObject  MetalNode::aLightDirectionY;
MObject  MetalNode::aLightDirectionZ;
MObject  MetalNode::aLightIntensity;
MObject  MetalNode::aLightIntensityR;
MObject  MetalNode::aLightIntensityG;
MObject  MetalNode::aLightIntensityB;
MObject  MetalNode::aLightAmbient;
MObject  MetalNode::aLightDiffuse;
MObject  MetalNode::aLightSpecular;
MObject  MetalNode::aLightShadowFraction;
MObject  MetalNode::aPreShadowIntensity;
MObject  MetalNode::aLightBlindData;

void *MetalNode::creator()
{
	return (new MetalNode);
}

MStatus MetalNode::initialize()
{
	MStatus status;
	MFnNumericAttribute nAttr;
	MFnLightDataAttribute lAttr;
	MFnTypedAttribute typedAttr;

	MFnStringData strData;
	MObject nameData = strData.create();
	strData.set(name);
	nameAttr = typedAttr.create("materialName", "mn", MFnData::kString, nameData, &status);
	CHECK_MSTATUS(typedAttr.setWritable(false));

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

	aFuzz = nAttr.create("fuzz", "f", MFnNumericData::kFloat, 0.3f, &status);
	CHECK_MSTATUS(nAttr.setMin(0));
	CHECK_MSTATUS(nAttr.setMax(1));
	CHECK_MSTATUS(nAttr.setKeyable(true));
	CHECK_MSTATUS(nAttr.setStorable(true));

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

	// Camera Normals
//
	aNormalCameraX = nAttr.create("normalCameraX", "nx",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aNormalCameraY = nAttr.create("normalCameraY", "ny",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aNormalCameraZ = nAttr.create("normalCameraZ", "nz",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aNormalCamera = nAttr.create("normalCamera", "n", aNormalCameraX,
		aNormalCameraY, aNormalCameraZ, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));
	CHECK_MSTATUS(nAttr.setHidden(true));


	// Light Direction
	//
	aLightDirectionX = nAttr.create("lightDirectionX", "ldx",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightDirectionY = nAttr.create("lightDirectionY", "ldy",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightDirectionZ = nAttr.create("lightDirectionZ", "ldz",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightDirection = nAttr.create("lightDirection", "ld",
		aLightDirectionX, aLightDirectionY, aLightDirectionZ,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));


	// Light Intensity
	//
	aLightIntensityR = nAttr.create("lightIntensityR", "lir",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightIntensityG = nAttr.create("lightIntensityG", "lig",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightIntensityB = nAttr.create("lightIntensityB", "lib",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightIntensity = nAttr.create("lightIntensity", "li",
		aLightIntensityR, aLightIntensityG, aLightIntensityB,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f, 1.0f, 1.0f));


	// Light
	//
	aLightAmbient = nAttr.create("lightAmbient", "la",
		MFnNumericData::kBoolean, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(true));

	aLightDiffuse = nAttr.create("lightDiffuse", "ldf",
		MFnNumericData::kBoolean, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(true));

	aLightSpecular = nAttr.create("lightSpecular", "ls",
		MFnNumericData::kBoolean, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(false));

	aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi",
		MFnNumericData::kFloat, 0, &status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));
	CHECK_MSTATUS(nAttr.setDefault(1.0f));

	aLightBlindData = nAttr.createAddr("lightBlindData", "lbld",
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(nAttr.setStorable(false));
	CHECK_MSTATUS(nAttr.setHidden(true));
	CHECK_MSTATUS(nAttr.setReadable(true));
	CHECK_MSTATUS(nAttr.setWritable(false));

	aLightData = lAttr.create("lightDataArray", "ltd", aLightDirection,
		aLightIntensity, aLightAmbient, aLightDiffuse, aLightSpecular,
		aLightShadowFraction, aPreShadowIntensity, aLightBlindData,
		&status);
	CHECK_MSTATUS(status);
	CHECK_MSTATUS(lAttr.setArray(true));
	CHECK_MSTATUS(lAttr.setStorable(false));
	CHECK_MSTATUS(lAttr.setHidden(true));
	CHECK_MSTATUS(lAttr.setDefault(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		true, true, false, 1.0f, 1.0f, NULL));

	CHECK_MSTATUS(addAttribute(nameAttr));
	CHECK_MSTATUS(addAttribute(aAlbedo));
	CHECK_MSTATUS(addAttribute(aFuzz));
	CHECK_MSTATUS(addAttribute(aOutColor));
	CHECK_MSTATUS(addAttribute(aNormalCamera));
	CHECK_MSTATUS(addAttribute(aLightData));

	CHECK_MSTATUS(attributeAffects(aAlbedoR, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedoG, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedoB, aOutColor));
	CHECK_MSTATUS(attributeAffects(aAlbedo, aOutColor));
	CHECK_MSTATUS(attributeAffects(aNormalCameraX, aOutColor));
	CHECK_MSTATUS(attributeAffects(aNormalCameraY, aOutColor));
	CHECK_MSTATUS(attributeAffects(aNormalCameraZ, aOutColor));
	CHECK_MSTATUS(attributeAffects(aNormalCamera, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightDirectionX, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightDirectionY, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightDirectionZ, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightDirection, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightAmbient, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightSpecular, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightDiffuse, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightShadowFraction, aOutColor));
	CHECK_MSTATUS(attributeAffects(aPreShadowIntensity, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightBlindData, aOutColor));
	CHECK_MSTATUS(attributeAffects(aLightData, aOutColor));

	return(MS::kSuccess);
}

MStatus MetalNode::compute(const MPlug &plug, MDataBlock &block)
{
	MStatus status;
	MFloatVector resultColor(0.0, 0.0, 0.0);

	MFloatVector &surfaceColor = block.inputValue(aAlbedo,
		&status).asFloatVector();
	CHECK_MSTATUS(status);

	MFloatVector &surfaceNormal = block.inputValue(aNormalCamera,
		&status).asFloatVector();
	CHECK_MSTATUS(status);

	float fuzz = block.inputValue(aFuzz,
		&status).asFloat();
	CHECK_MSTATUS(status);

	resultColor = surfaceColor;

	// start
	MArrayDataHandle lightData = block.inputArrayValue(aLightData,
		&status);
	CHECK_MSTATUS(status);

	int numLights = lightData.elementCount(&status);
	CHECK_MSTATUS(status);


	// Calculate the effect of the lights in the scene on the color
	//

	// Iterate through light list and get ambient/diffuse values
	//
	for (int count = 1; count <= numLights; count++)
	{
		// Get the current light out of the array
		//
		MDataHandle currentLight = lightData.inputValue(&status);
		CHECK_MSTATUS(status);


		// Get the intensity of that light
		//
		MFloatVector &lightIntensity = currentLight.child(
			aLightIntensity).asFloatVector();


		// Find ambient component
		//
		if (currentLight.child(aLightAmbient).asBool())
		{
			resultColor += lightIntensity;
		}

		// Find diffuse component
		//
		if (currentLight.child(aLightDiffuse).asBool())
		{
			MFloatVector &lightDirection = currentLight.child(
				aLightDirection).asFloatVector();
			float cosln = lightDirection * surfaceNormal;

			if (cosln > 0.0f) {
				resultColor += lightIntensity
					* (cosln * fuzz);
			}
		}


		// Advance to the next light.
		//
		if (count < numLights) {
			status = lightData.next();
			CHECK_MSTATUS(status);
		}
	}


	// Factor incident light with surface color and add incandescence
	//
	resultColor[0] = resultColor[0] * surfaceColor[0];
	resultColor[1] = resultColor[1] * surfaceColor[1];
	resultColor[2] = resultColor[2] * surfaceColor[2];


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

void MetalNode::postConstructor()
{
	setMPSafe(true);
}