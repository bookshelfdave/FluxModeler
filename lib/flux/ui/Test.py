from flux.core import *
from flux.io.ObjParser import *

def HandleKeyboard():
    o = ObjParser()
    print "Parsing file!!"
    sb = o.parseFile(r"c:\src\grid.obj")
    sb.sendToFlux()
    print "Done sending to flux!"
    #print "Total verts = ",len(flux.sceneObjects[0].verts)
    #print "Total faces = ",len(flux.sceneObjects[0].faces)
    #print "Face:",flux.sceneObjects[0].faces[0]
    #print flux.sceneObjects[0].halfEdges["0-5"].henext()
    #for v in flux.sceneObjects[0].verts:
    #    if v.x > 10.0:
    #        print v.x

    #print "Edge count:", flux.app.edgeCount(0)
    #print "Face count:", flux.app.getFace(0,2)
    #print "Edge 0:", flux.sceneObjects[0].edges[0]
    #print "Done!"

def HandleMouse(x,y):
    print "!!!Mouse clicked at %i %i" % (x,y)
    app.sceneObjects[0].verts[0].show()

