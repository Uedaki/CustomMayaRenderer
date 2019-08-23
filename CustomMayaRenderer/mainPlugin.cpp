#include "pch.h"

#include <Maya/MDagPath.h>
#include <Maya/MGlobal.h>
#include <Maya/MItDag.h>
#include <Maya/MFn.h>
#include <Maya/MFnDagNode.h>
#include <Maya/MFnPlugin.h>
#include <Maya/MPxCommand.h>

#include "GlobalsNode.h"
#include "RenderProcedure.h"
#include "RenderRegionProcedure.h"
#include "utils.h"

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Custom Maya Renderer", "0.1", "Any");
	
	STATUS_BARRIER(plugin.registerNode(GlobalsNode::name, GlobalsNode::id, GlobalsNode::creator, GlobalsNode::initialize));

	STATUS_BARRIER(plugin.registerCommand("customMayaRendererRenderProcedure", RenderProcedure::creator));
	STATUS_BARRIER(plugin.registerCommand("customMayaRendererRenderRegionProcedure", RenderRegionProcedure::creator));

	STATUS_BARRIER(MGlobal::executePythonCommand("import CustomMayaRenderer", false, false));
	STATUS_BARRIER(MGlobal::executePythonCommand("import CustomMayaRenderer.register; CustomMayaRenderer.register.register()", false, false));

	LOG_MSG("Plugin loaded successfully");
	return (MS::kSuccess);
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	STATUS_CHECK(plugin.deregisterCommand("customMayaRendererRenderProcedure"));
	STATUS_CHECK(plugin.deregisterCommand("customMayaRendererRenderRegionProcedure"));

	GlobalsNode::clean();
	STATUS_CHECK(plugin.deregisterNode(GlobalsNode::id));

	STATUS_CHECK(MGlobal::executePythonCommand("import CustomMayaRenderer.register; CustomMayaRenderer.register.unregister()", false, false));

	LOG_MSG("Plugin unloaded successfully");
	return (MS::kSuccess);
}
