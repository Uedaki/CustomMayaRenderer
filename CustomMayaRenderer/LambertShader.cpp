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
	return ("mayaLambert");
}

void LambertShader::getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings)
{
	MHWRender::MAttributeParameterMapping diffuseMapping(
		"diffuse", "diffuseReflectivity", true, true);
	mappings.append(diffuseMapping);

	MHWRender::MAttributeParameterMapping translucenceMapping(
		"translucence", "translucenceCoeff", true, true);
	mappings.append(translucenceMapping);
}

MString LambertShader::primaryColorParameter() const
{
	return ("color");
}