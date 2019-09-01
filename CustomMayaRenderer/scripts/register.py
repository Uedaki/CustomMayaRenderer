import pymel.core as pm
import maya.cmds as mc
from renderer import (createRenderMelProcedure)
import globalsNode
from globalsNode import (
    createRenderTabsMelProcedure,
    renderBuildSettingsCallback,
    addRenderGlobalsScriptJob)

def register():
    pm.renderer("customMayaRenderer", rendererUIName = "Custom Maya Renderer")

    createRenderMelProcedure()
    pm.renderer("customMayaRenderer", edit = True,
                renderProcedure = "customMayaRendererRenderProcedureProxy",
		        renderRegionProcedure = "customMayaRendererRenderRegionProcedure")

    createRenderTabsMelProcedure()
    renderBuildSettingsCallback("customMayaRenderer")
    pm.renderer("customMayaRenderer", edit=True, addGlobalsNode="defaultRenderGlobals")
    pm.renderer("customMayaRenderer", edit = True, addGlobalsNode = "customMayaRendererGlobalsNode")
    
    pm.callbacks(addCallback = renderBuildSettingsCallback,
                 hook = "renderSettingsBuilt",
                 owner = "customMayaRenderer")

    addRenderGlobalsScriptJob()
  

def unregister():
    mc.flushUndo()
    pm.callbacks(clearCallbacks=True, owner="customMayaRenderer")

    pm.renderer('customMayaRenderer', unregisterRenderer = True)