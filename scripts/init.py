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

def createGeom(pos, scale, color):
    g_GeomFactory.setTrans(pos[0], pos[1], pos[2])
    g_GeomFactory.setScale(scale[0], scale[1], scale[2])
    g_GeomFactory.setColor(color[0], color[1], color[2], color[3])
    g_GeomCollection.AddGeom(g_GeomFactory())

def initGeom():
    createGeom(pos = [-1.,-1.,0.], scale = [1.,1.,0.], color = [0.,1.,0.,1.])
    createGeom(pos = [1.,-1.,0.], scale = [1.,0.5,0.], color = [1.,1.,0.,1.])

# you could just expose some InitOrtho, but I guess this is clearer? maybe?
def createOrthoCamera(left, right, bottom, top, near, far):
    g_Camera.Init(left, right, bottom, top, near, far)

def initCamera():
    createOrthoCamera(left=-1., right=1., bottom=-1., top=1., near=1., far=-1.)