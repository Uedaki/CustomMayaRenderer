import maya.OpenMaya as om
import maya.cmds as mc
import maya.mel as mel
import pymel.core as pm

def createGlobalsNode():
    if mc.objExists("customMayaRendererGlobalsNode"):
        return

    selection = mc.ls(sl = True)

    mc.createNode(
        "customMayaRendererGlobalsNode",
        name = "customMayaRendererGlobalsNode",
        shared = True,
        skipSelect = True)

    mc.lockNode("customMayaRendererGlobalsNode")
    mc.select(selection, replace = True)


class CustomMayaRendererGenericTab(object):

    def __init__(self):
        self._uis = {}

    def _addControl(self, ui, attrName, connectIndex=2):
        self._uis[attrName] = ui
        attr = pm.Attribute("customMayaRendererGlobalsNode." + attrName)
        pm.connectControl(ui, attr, index=connectIndex)

    def _getAttributeMenuItems(self, attrName):
        attr = pm.Attribute("customMayaRendererGlobalsNode." + attrName)
        menuItems = [
            (i, v) for i, v in enumerate(attr.getEnums().keys())
        ]
        return menuItems


def createRenderTabsMelProcedure():
    pm.mel.source("createMayaSoftwareCommonGlobalsTab.mel")

    mel.eval('''
    global proc customMayaRendererCreateCommonTabProcedure()
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
                    "customMayaRendererEmptyUpdateProcedure"))

    pm.renderer("customMayaRenderer", edit = True,
                addGlobalsTab = (
                    "CustomMayaRenderer",
                    "customMayaRendererCreateCommonTabProcedure",
                    "customMayaRendererEmptyUpdateProcedure"))


def addRenderGlobalsScriptJob():
    mc.scriptJob(
    attributeChange=[
        "defaultRenderGlobals.currentRenderer",
        "import appleseedMaya.renderGlobals; appleseedMaya.renderGlobals.currentRendererChanged()"])


def currentRendererChanged():
    newRenderer = mel.eval("currentRenderer()")
    if newRenderer != "customMayaRenderer":
        return

    createGlobalsNode()


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

                        self._addControl(
                            ui=pm.intFieldGrp(
                                label="Width",
                                columnWidth=(3, 160),
                                columnAttach=(1, "right", 4)),
                            attrName="width")

                        self._addControl(
                            ui=pm.intFieldGrp(
                                label="Height",
                                columnWidth=(3, 160),
                                columnAttach=(1, "right", 4)),
                            attrName="height")

                        self._addControl(
                            ui=pm.intFieldGrp(
                                label="Samples",
                                columnWidth=(3, 160),
                                columnAttach=(1, "right", 4)),
                            attrName="nbSample")

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


g_customMayaRendererCommonTab = CustomMayaRendererCommonTab()