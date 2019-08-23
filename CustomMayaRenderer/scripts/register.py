import pymel.core as pm
from renderer import (createRenderMelProcedure)
from globalsNode import (
    createRenderTabsMelProcedure,
    renderBuildSettingsCallback)

def register():
    createRenderMelProcedure()

    pm.renderer("customMayaRenderer", rendererUIName = "Custom Maya Renderer")
    pm.renderer("customMayaRenderer", edit = True,
                renderProcedure = "customMayaRendererRenderProcedureProxy",
		        renderRegionProcedure = "customMayaRendererRenderRegionProcedure")

    createRenderTabsMelProcedure()
    renderBuildSettingsCallback("customMayaRenderer")
    pm.renderer("customMayaRenderer", edit = True, addGlobalsNode = "customMayaRendererGlobalsNode")
    pm.callbacks(
                 addCallback = renderBuildSettingsCallback,
                 hook = "renderSettingsBuilt",
                 owner = "customMayaRenderer")
  

def unregister():
    pm.callbacks(clearCallbacks=True, owner="appleseed")

    pm.renderer('customMayaRenderer', unregisterRenderer = True)