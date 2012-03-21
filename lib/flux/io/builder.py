from flux.root import *
from flux.core import *

class SceneBuilder:
    def __init__(self):
        self.objectBuilders = []

    def sendToFlux(self):
        print "Sending to flux!"
        for o in self.objectBuilders:
            print "Creating new scene object:", flux.app.createSceneObject()
            print "-> Vert count = ", len(o.verts)
            for v in o.verts:
                flux.app.createVert(v.x,v.y,v.z)
            for f in o.faces:
                flux.app.createFace(f)
            flux.app.buildSceneObject()

class ObjectBuilder:
    def __init__(self):
        self.verts = []
        self.faces = []