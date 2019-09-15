#include "pch.h"
#include "DielectricShader.h"

MHWRender::MPxSurfaceShadingNodeOverride *DielectricShader::creator(const MObject &obj)
{
	return (new DielectricShader(obj));
}

DielectricShader::DielectricShader(const MObject &obj)
	: MPxSurfaceShadingNodeOverride(obj)
{}

DielectricShader::~DielectricShader()
{}

MHWRender::DrawAPI DielectricShader::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
}

MString DielectricShader::fragmentName() const
{
	return ("mayaLambertSurface");
}

void DielectricShader::getCustomMappings(MHWRender::MAttributeParameterMappingList &mappings)
{
	MHWRender::MAttributeParameterMapping diffuseMapping(
		"color", "outColor", true, true);
	mappings.append(diffuseMapping);
}

MString DielectricShader::primaryColorParameter() const
{
	return ("outColor");
}

MString DielectricShader::transparencyParameter() const
{
	return ("transparency");
}

MString DielectricShader::bumpAttribute() const
{
	return ("normalCamera");
}