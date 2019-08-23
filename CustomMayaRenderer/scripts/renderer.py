import maya.mel as mel

def createRenderMelProcedure():
    mel.eval('''
        global proc customMayaRendererRenderProcedureProxy(int $width, int $height, int $doShadows, int $doGlowPass, string $camera, string $option)
        {
            python("import CustomMayaRenderer.globalsNode");
            python("CustomMayaRenderer.globalsNode.createGlobalsNode()");
            customMayaRendererRenderProcedure -w $width -h $height -c $camera;
        }''')