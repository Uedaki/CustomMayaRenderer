#include "pch.h"
#include "Material.h"

#include <Maya/MFnDependencyNode.h>
#include <Maya/MObject.h>
#include <Maya/MPlug.h>
#include <Maya/MString.h>

#include <functional>
#include <map>
#include <string>

#include "LambertMaterial.h"
#include "LambertNode.h"
#include "utils.h"

Material *Material::create(const MString &mName)
{
	const std::map<std::string, std::function<Material *(MObject)>> creator = {
		{LambertNode::name.asChar(), LambertMaterial::create}
	};

	if (mName == "")
		return (new LambertMaterial(glm::vec3(0.3, 0.3, 0.3)));

	MObject mShaderObject;
	if (getDependencyNodeByName(mName, mShaderObject) != MS::kSuccess)
		return (nullptr);
	MFnDependencyNode mShaderNode(mShaderObject);
	MPlug mMaterialType = mShaderNode.findPlug("materialName");

	return (creator.at(mMaterialType.asString().asChar())(mShaderObject));
}