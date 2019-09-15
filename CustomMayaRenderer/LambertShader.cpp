#include "pch.h"
#include "LambertShader.h"

MHWRender::MPxSurfaceShadingNodeOverride *LambertShader::creator(const MObject &obj)
{
	return (new LambertShader(obj));
}

LambertShader::LambertShader(const MObject &obj)
	: MPxSurfaceShadingNodeOverride(obj)
{}

LambertShader::~LambertShader()
{}

MHWRender::DrawAPI LambertShader::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

MString LambertShader::fragmentName() const
{
	return ("mayaLambertSurface");
}

void LambertShader::getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings)
{
	MHWRender::MAttributeParameterMapping diffuseMapping(
		"color", "outColor", true, true);
	mappings.append(diffuseMapping);
}

MString LambertShader::primaryColorParameter() const
{
	return ("outColor");
}

MString LambertShader::bumpAttribute() const
{
	return ("normalCamera");
}