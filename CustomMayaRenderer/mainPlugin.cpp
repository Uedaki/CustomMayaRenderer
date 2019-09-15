#include "pch.h"

#include <Maya/MDagPath.h>
#include <maya/MDrawRegistry.h>
#include <Maya/MGlobal.h>
#include <Maya/MItDag.h>
#include <Maya/MFn.h>
#include <Maya/MFnDagNode.h>
#include <Maya/MFnPlugin.h>
#include <Maya/MPxCommand.h>

#include "DielectricNode.h"
#include "DielectricShader.h"
#include "GlobalsNode.h"
#include "LambertNode.h"
#include "LambertShader.h"
#include "MetalNode.h"
#include "MetalShader.h"
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

	MString sDrawLambertDBClassification("drawdb/shader/surface/" + LambertNode::name);
	MString sLambertFullClassification("shader/surface:" + sDrawLambertDBClassification);
	plugin.registerNode(LambertNode::name, LambertNode::id, LambertNode::creator, LambertNode::initialize, MPxNode::kDependNode, &sLambertFullClassification);
	MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(sDrawLambertDBClassification, LambertNode::name, LambertShader::creator);

	MString sDrawMetalDBClassification("drawdb/shader/surface/" + MetalNode::name);
	MString sMetalFullClassification("shader/surface:" + sDrawMetalDBClassification);
	plugin.registerNode(MetalNode::name, MetalNode::id, MetalNode::creator, MetalNode::initialize, MPxNode::kDependNode, &sMetalFullClassification);
	MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(sDrawMetalDBClassification, MetalNode::name, MetalShader::creator);

	MString sDrawDielectricDBClassification("drawdb/shader/surface/" + DielectricNode::name);
	MString sDielectricFullClassification("shader/surface:" + sDrawDielectricDBClassification);
	plugin.registerNode(DielectricNode::name, DielectricNode::id, DielectricNode::creator, DielectricNode::initialize, MPxNode::kDependNode, &sDielectricFullClassification);
	MHWRender::MDrawRegistry::registerSurfaceShadingNodeOverrideCreator(sDrawDielectricDBClassification, DielectricNode::name, DielectricShader::creator);

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

	MString command("if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"");

	MString sDrawLambertDBClassification("drawdb/shader/surface/" + LambertNode::name);
	plugin.deregisterNode(LambertNode::id);
	MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(sDrawLambertDBClassification, LambertNode::name);

	MString sDrawMetalDBClassification("drawdb/shader/surface/" + MetalNode::name);
	plugin.deregisterNode(MetalNode::id);
	MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(sDrawMetalDBClassification, MetalNode::name);

	MString sDrawDielectricDBClassification("drawdb/shader/surface/" + DielectricNode::name);
	plugin.deregisterNode(DielectricNode::id);
	MHWRender::MDrawRegistry::deregisterSurfaceShadingNodeOverrideCreator(sDrawDielectricDBClassification, DielectricNode::name);

	STATUS_CHECK(MGlobal::executePythonCommand("import CustomMayaRenderer.register; CustomMayaRenderer.register.unregister()", false, false));

	LOG_MSG("Plugin unloaded successfully");
	return (MS::kSuccess);
}
