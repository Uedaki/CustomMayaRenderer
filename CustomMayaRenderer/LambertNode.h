#pragma once

#include <Maya/MPxNode.h>

class LambertMaterial;

class LambertNode : public MPxNode
{
public:
	static const MString name;
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();

	MStatus compute(const MPlug &plug, MDataBlock &block) override;
	void postConstructor() override;

private:
	friend class LambertMaterial;

	static MObject nameData;
	static MObject nameAttr;

	static MObject  aAlbedoR;
	static MObject  aAlbedoG;
	static MObject  aAlbedoB;
	static MObject  aAlbedo;

	static MObject  aOutColorR;
	static MObject  aOutColorG;
	static MObject  aOutColorB;
	static MObject  aOutColor;

	static MObject samples;
};