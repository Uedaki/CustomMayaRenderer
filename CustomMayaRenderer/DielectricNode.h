#pragma once

#include <Maya/MPxNode.h>

class DielectricMaterial;

class DielectricNode : public MPxNode
{
public:
	static const MString name;
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();

	MStatus compute(const MPlug &plug, MDataBlock &block) override;
	void postConstructor() override;

private:
	friend class DielectricMaterial;

	static MObject nameData;
	static MObject nameAttr;

	static MObject  aTranslucenceCoeff;

	static MObject  aRefraction;

	static MObject  aOutColorR;
	static MObject  aOutColorG;
	static MObject  aOutColorB;
	static MObject  aOutColor;

	static MObject  aInTransR;
	static MObject  aInTransG;
	static MObject  aInTransB;
	static MObject  aInTransparency;

	static MObject  aOutTransR;
	static MObject  aOutTransG;
	static MObject  aOutTransB;
	static MObject  aOutTransparency;

	static MObject  aNormalCamera;
	static MObject  aNormalCameraX;
	static MObject  aNormalCameraY;
	static MObject  aNormalCameraZ;
	static MObject  aLightData;
	static MObject  aLightDirection;
	static MObject  aLightDirectionX;
	static MObject  aLightDirectionY;
	static MObject  aLightDirectionZ;
	static MObject  aLightIntensity;
	static MObject  aLightIntensityR;
	static MObject  aLightIntensityG;
	static MObject  aLightIntensityB;
	static MObject  aLightAmbient;
	static MObject  aLightDiffuse;
	static MObject  aLightSpecular;
	static MObject  aLightShadowFraction;
	static MObject  aPreShadowIntensity;
	static MObject  aLightBlindData;
};