from flux.core import *
from flux.io.builder import *
import unittest

class ObjParser:
    def __init__(self):
        self.sceneBuilder = SceneBuilder()
        self.objBuilder = None

    def parseFile(self,filename):
        fin = open(filename)
        for line in fin:
            chunks = line.split(" ")
            if chunks[0] == "" or chunks[0] == "#":
                pass
            else:
                self.parseLine(chunks)
        return self.sceneBuilder

    def parseLine(self,chunks):
        c0 = chunks[0]
        if c0 == "o":
            self.objBuilder = ObjectBuilder()
            self.sceneBuilder.objectBuilders.append(self.objBuilder)
            print "Found an object:", chunks[1]
        elif c0 == "v":
            pass
            self.objBuilder.verts.append(V3(float(chunks[1]),float(chunks[2]),float(chunks[3])))
            #print "Vert: %f %f %f" % (float(chunks[1]),float(chunks[2]),float(chunks[3]))
        elif c0 == "vn":
            pass
            #print "Found a vert norm %f %f %f" % (float(chunks[1]),float(chunks[2]),float(chunks[3]))
        elif c0 == "f":
            #1//1 6//6 7//7 2//2
            faceids = []
            for chunk in chunks[1:]:
                fchunk = chunk.split(r"/")
                faceids.append(int(fchunk[0])-1)
            self.objBuilder.faces.append(faceids)
            #print "Found a face", faceids
        else:
            pass


#print "Starting to parse"
#o = ObjParser()
#o.parseFile(r"c:\src\huge.obj")
#print "Done!"



