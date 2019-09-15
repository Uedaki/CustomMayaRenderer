#include "pch.h"
#include "MetalShader.h"

MHWRender::MPxSurfaceShadingNodeOverride *MetalShader::creator(const MObject &obj)
{
	return (new MetalShader(obj));
}

MetalShader::MetalShader(const MObject &obj)
	: MPxSurfaceShadingNodeOverride(obj)
{}

MetalShader::~MetalShader()
{}

MHWRender::DrawAPI MetalShader::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

MString MetalShader::fragmentName() const
{
	return ("mayaPhongSurface");
}

void MetalShader::getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings)
{
	MHWRender::MAttributeParameterMapping diffuseMapping(
		"color", "outColor", true, true);
	mappings.append(diffuseMapping);
}

MString MetalShader::primaryColorParameter() const
{
	return ("outColor");
}

MString MetalShader::bumpAttribute() const
{
	return ("normalCamera");
}