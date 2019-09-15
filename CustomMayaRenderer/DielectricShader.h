#pragma once

#include <Maya/MPxSurfaceShadingNodeOverride.h>

class DielectricShader : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
	static MHWRender::MPxSurfaceShadingNodeOverride *creator(const MObject &obj);

	~DielectricShader() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	MString fragmentName() const override;
	void getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings) override;

	MString primaryColorParameter() const override;
	MString transparencyParameter() const override;
	MString bumpAttribute() const override;

private:
	DielectricShader(const MObject &obj);
};