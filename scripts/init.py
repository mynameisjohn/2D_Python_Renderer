class Entity:
    def __init__(self):
        self.components = {}

g_Entities = []

#for now everything is quads
def addEntity(pos, color, scale):
    #prior to this call, give graphics factory color, scale, pos, etc.
    g_GraphicsFactory.setTrans(pos[0], pos[1], pos[2])
    g_GraphicsFactory.setScale(scale[0], scale[1], scale[2])
    g_GraphicsFactory.setColor(color[0], color[1], color[2], color[3])
    gComp_cptr = g_GraphicsCollection.AddComponent(g_GraphicsFactory())
    print(gComp_cptr)

    e = Entity()
    e.components["Graphics"] = G_Comp(gComp_cptr)

    g_Entities.append(e)


def initGeom():
    addEntity(pos = [-5.,-1.,0.], scale = [1.,6.,0.], color = [0.,1.,0.,1.])
    addEntity(pos = [4.,3.,0.], scale = [3.,0.5,0.], color = [1.,0.,0.,1.])
    addEntity(pos = [0.,7.,0.], scale = [3.,0.5,0.], color = [0.,0.,1.,1.])

# you could just expose some InitOrtho, but I guess this is clearer?  maybe?
def createOrthoCamera(left, right, bottom, top, near, far):
    g_Camera.Init(left, right, bottom, top, near, far)

def initCamera():
    createOrthoCamera(left=-10., right=10., bottom=-10., top=10., near=10., far=-10.)

def handleShaderErrs(vSrc, fSrc):
    g_Shader.SetSourceFile_V(vSrc)
    g_Shader.SetSourceFile_F(fSrc)
    err = g_Shader.CompileAndLink()
    print {
            0: 'Shaders compiled successfulyl!',
            1: 'Error compiling vertex shader!',
            2: 'Error compiling fragment shader!',
            3: 'Error linking shader program!'
        }[err]

def initShader():
    handleShaderErrs("../Shaders/simple.vert", "../Shaders/simple.frag")