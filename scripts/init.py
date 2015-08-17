pos = [0.,0.,0.]
scale = [0.5,1.,1.]
color = [1.,0.,0.,1.]

def create():
    geomFactory.setTrans(pos[0], pos[1], pos[2])
    geomFactory.setScale(scale[0], scale[1], scale[2])
    geomFactory.setColor(color[0], color[1], color[2], color[3])
    geomCollection.AddGeom(geomFactory())

create()

pos = [-1.,-1.,0.]
scale = [1.,1.,1.]
color = [0.,1.,0.,1.]

create()
