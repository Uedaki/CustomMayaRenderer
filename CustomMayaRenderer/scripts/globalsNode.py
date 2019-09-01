import maya.OpenMaya as om
import maya.cmds as mc
import maya.mel as mel
import pymel.core as pm

def createGlobalsNode():
    if mc.objExists("customMayaRendererGlobalsNode"):
        return

    mc.createNode(
        "customMayaRendererGlobalsNode",
        name = "customMayaRendererGlobalsNode",
        shared = True,
        skipSelect = True)

def createRenderTabsMelProcedure():
    pm.mel.source("createMayaSoftwareCommonGlobalsTab.mel")

    mel.eval('''
    global proc customMayaRendererUpdateCommonTabProcedure()
    {
        updateMayaSoftwareCommonGlobalsTab();
    }
    ''')

    mel.eval('''
    global proc customMayaRendererCreateMainTabProcedure()
    {
        python("import CustomMayaRenderer.globalsNode");
        python("CustomMayaRenderer.globalsNode.g_customMayaRendererCommonTab.create()");
    }
    ''')

    mel.eval('''
    global proc customMayaRendererEmptyUpdateProcedure()
    {

    }
    ''')


def renderBuildSettingsCallback(renderer):
    pm.renderer("customMayaRenderer", edit = True,
                addGlobalsTab = (
                    "Common",
                    "createMayaSoftwareCommonGlobalsTab",
                    "customMayaRendererUpdateCommonTabProcedure"))

    pm.renderer("customMayaRenderer", edit = True,
                addGlobalsTab = (
                    "CustomMayaRenderer",
                    "customMayaRendererCreateMainTabProcedure",
                    "customMayaRendererEmptyUpdateProcedure"))


def addRenderGlobalsScriptJob():
    mc.scriptJob(
    attributeChange=[
        "defaultRenderGlobals.currentRenderer",
        "import CustomMayaRenderer.globalsNode; CustomMayaRenderer.globalsNode.currentRendererChanged()"])


def currentRendererChanged():
    newRenderer = mel.eval("currentRenderer()")
    if newRenderer != "customMayaRenderer":
        return

    createGlobalsNode()

    #if not mc.window("unifiedRenderGlobalsWindow", exists=True):
    #    mel.eval("unifiedRenderGlobalsWindow")
    #    mc.window("unifiedRenderGlobalsWindow", edit=True, visible=False)


class CustomMayaRendererGenericTab(object):

    def __init__(self):
        self._uis = {}

    def _addControl(self, ui, attrName, connectIndex=2):
        self._uis[attrName] = ui
        pm.connectControl(ui, "customMayaRendererGlobalsNode." + attrName, index=connectIndex)

    def _getAttributeMenuItems(self, attrName):
        attr = pm.Attribute("customMayaRendererGlobalsNode." + attrName)
        menuItems = [
            (i, v) for i, v in enumerate(attr.getEnums().keys())
        ]
        return menuItems


class CustomMayaRendererCommonTab(CustomMayaRendererGenericTab):

    def create(self):
        createGlobalsNode()

        parentForm = pm.setParent(query=True)
        pm.setUITemplate("renderGlobalsTemplate", pushTemplate=True)
        pm.setUITemplate("attributeEditorTemplate", pushTemplate=True)

        with pm.scrollLayout("customMayaRendererScrollLayout", horizontalScrollBarThickness=0):
            with pm.columnLayout("customMayaRendererColumnLayout", adjustableColumn=True, width=400):

                with pm.frameLayout("generalFrameLayout", label="General", collapsable=True, collapse=False):
                    with pm.columnLayout("generalColumnLayout", adjustableColumn=True, width=380,
                                         rowSpacing=2):

                        pm.separator(height=2)

                        pm.attrFieldSliderGrp(
                                label = "Number of samples:",
                                columnWidth = (3, 160),
                                columnAttach= (1, "left", 4),
                                minValue = 1,
                                maxValue = 100,
                                fieldMinValue = 1,
                                fieldMaxValue = 100,
                                attribute = "customMayaRendererGlobalsNode.samples")


        pm.setUITemplate("renderGlobalsTemplate", popTemplate=True)
        pm.setUITemplate("attributeEditorTemplate", popTemplate=True)
        pm.formLayout(
            parentForm,
            edit=True,
            attachForm=[
                ("customMayaRendererScrollLayout", "top", 0),
                ("customMayaRendererScrollLayout", "bottom", 0),
                ("customMayaRendererScrollLayout", "left", 0),
                ("customMayaRendererScrollLayout", "right", 0)])

    def update(self):
        assert mc.objExists("customMayaRendererGlobalsNode");

g_customMayaRendererCommonTab = CustomMayaRendererCommonTab()