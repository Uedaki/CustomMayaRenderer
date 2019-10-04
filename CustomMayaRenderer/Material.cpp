#include "pch.h"
#include "Material.h"

#include <Maya/MFnDependencyNode.h>
#include <Maya/MObject.h>
#include <Maya/MPlug.h>
#include <Maya/MString.h>

#include <functional>
#include <map>
#include <string>

#include "DielectricMaterial.h"
#include "DielectricNode.h"
#include "LambertMaterial.h"
#include "LambertNode.h"
#include "MetalMaterial.h"
#include "MetalNode.h"
#include "utils.h"

Material *Material::create(const MString &mName)
{
	const std::map<std::string, std::function<Material *(MObject)>> creator = {
		{LambertNode::name.asChar(), LambertMaterial::create},
		{MetalNode::name.asChar(), MetalMaterial::create},
		{DielectricNode::name.asChar(), DielectricMaterial::create}
	};

	MObject mShaderObject;
	if (getDependencyNodeByName(mName, mShaderObject) != MS::kSuccess)
		return (new LambertMaterial(glm::vec3(0.5, 0.5, 0.5)));
	MFnDependencyNode mShaderNode(mShaderObject);
	MPlug mMaterialType = mShaderNode.findPlug("materialName");
	if (creator.find(mMaterialType.asString().asChar()) != creator.cend())
		return (creator.at(mMaterialType.asString().asChar())(mShaderObject));
	return (new LambertMaterial(glm::vec3(0.5, 0.5, 0.5)));
}