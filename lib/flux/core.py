import math
import sys
import flux.mode

if not flux.mode.standalone:
    from flux.root import *

#else:
#    segs = sys.path[0].split("\\")
#    parent = "\\".join(segs[0:-1])
#    sys.path.append(parent)
#    import flux.changes
#    from flux.changes import *

from flux.changes import *

class MetaObj:
    changeQueue = set([])
    def __init__(self,params):
        for k,v in params.iteritems():
            self.__dict__[k]=v
        self.__dict__["dirty"]=False

    def __setattr__(self,attr,value):
        self.__dict__["dirty"]=True
        if not self.__dict__.has_key("_" + attr):
            self.__dict__["_" + attr]=self.__dict__[attr]
        self.__dict__[attr]=value
        MetaObj.changeQueue.add(self)

    def rollback(self):
        print "Not implemented!"
        pass

class Point(MetaObj):
    def __init__(self,x,y,z):
        params = {"x":x,"y":y,"z":z}
        MetaObj.__init__(self,params)

    def __str__(self):
        return "Point <%r,%r,%r> %s" % (self.x,self.y,self.z,self.dirty)




class Flux(object):
    def __init__(self):
        print "Flux root object initialized"
        self.sceneObjects = []
        self.inputStateStack = InputStateStack()
        self.app = App()


    def sync(self):
        self.sceneObjects = []
        for soid in range(0,self.app.sceneObjectCount()):
            so = Object3d(soid)
            self.sceneObjects.append(so)

class V3(object):
    def __init__(self,newx,newy,newz):
        self.x = newx
        self.y = newy
        self.z = newz

    def __mul__(self,other):
        """Vector dot product"""
        return V3(self.x * other.x, self.y * other.y, self.z * other.z)

    def __mod__(self,other):
        """Vector cross product"""
        return V3(self.y * other.z - self.z * other.y,
                  self.z * other.x - self.x * other.z,
                  self.x * other.y - self.y * other.x)

    def __add__(self,other):
        """Vector addition"""
        return V3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self,other):
        """Vector subtraction"""
        return V3(self.x - other.x, self.y - other.y, self.z - other.z)

    def length(self):
        return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z)

    def normalize(self):
        l = self.length();
        return V3(self.x / l, self.y / l, self.z / l);

    def __str__(self):
        return "V3<%r,%r,%r>" % (self.x,self.y,self.z)


class Vert(V3):
    def __init__(self,vid,newx,newy,newz,vertselected):
        super(Vert,self).__init__(newx,newy,newz)
        self.vertselected = vertselected
        self.vid = vid

    def __str__(self):
        return "Vert (%i) <%r,%r,%r>" % (self.vid,self.x,self.y,self.z)

class Triangle(object):
    def __init__(self,vid1,vid2,vid3):
        self.points = (vid1,vid2,vid3)

    def show(self):
        print "Triangle:", self.points[0],self.points[1], self.points[2]

class Edge(object):
    def __init__(self,so, edgeid, vida, vidb, selected):
            self.so = so
            self.edgeid = edgeid
            self.vida = vida
            self.vidb = vidb
            self.selected = selected
            self.a = so.verts[vida]
            self.b = so.verts[vidb]
    def __eq__(self,other):
        if self.vida == other.vida and self.vidb == other.vidb:
            true
        else:
            false

    def __str__(self):
        #return "Edge (%i) <%i,%i>" % (self.edgeid, self.vida, self.vidb)
        return "Edge (%i) <%s,%s>" % (self.edgeid, self.a, self.b)


class Face(object):
    def __init__(self,so,faceid,vertids,undirectedEdgeIds,triangleVids,centroid,normal):
        self.so = so
        self.faceid = faceid
        self.vertids = vertids
        self.verts = [so.verts[vertid] for vertid in self.vertids]
        self.undirectedEdgeIds = undirectedEdgeIds
        self.undirectedEdges = [so.edges[edgeid] for edgeid in self.undirectedEdgeIds]
        self.triangleVids = triangleVids
        self.centroid = centroid
        self.normal = normal
        self.selected = False

    def __str__(self):
        return "Face <%i,%s,%s,%s>" % (self.faceid,self.vertids,self.centroid,self.normal)

class HalfEdge(object):
    def __init__(self,so,key,vertid,faceid,normalizedEdgeId,reversedEdge):
        self.so = so
        self.vertid = vertid
        self.faceid = faceid
        self.normalizedEdgeId = normalizedEdgeId
        self.reversedEdge = reversedEdge
        self.pairid = None # will eventually point to a HalfEdge
        self.pair = None
        self.previd = None # will eventually point to a HalfEdge
        self.prev = None
        self.nextid = None # will eventually point to a HalfEdge
        self.next = None
        self.key = key

    def henext(self):
        if self.nextid != None:
            self.next = self.so.HalfEdges[self.nextid]
        return self.next

    def heprev(self):
        if self.previd != None:
            self.prev = self.so.HalfEdges[self.previd]
        return self.prev

    def hepair(self):
        if self.pairid != None:
            self.pair = self.so.HalfEdges[self.pairid]
        return self.pair

    def __str__(self):
        return "HalfEdge <%s>" % (self.key,)

class ItemCache:
    def __init__(self):
        self.cache = {}
        self.size = self._itemCacheSize()

    def __getitem__(self,item):
        if item > (self.size-1):
            raise IndexError
        if self.cache.has_key(item):
            return self.cache[item]
        else:
            i = self._itemGet(item)
            self.cache[item]=i
            return i

    def __len__(self):
        return self.size

    def refresh(self):
        self.cache={}
        self.size = sizefn()

    def _itemCacheSize(self):
        pass

    def _itemGet(self,item):
        pass


# _itemGet must return None for an invalid key
class DictItemCache(ItemCache):
    def __init__(self):
        ItemCache.__init__(self)

    def __getitem__(self,item):
        if self.cache.has_key(item):
            return self.cache[item]
        else:
            i = self._itemGet(item)
            if i == None:
                raise IndexError
            else:
                self.cache[item]=i
                return i

    def __iter__(self):
        self.iterkey = 0
        return self

    def next(self):
        key = self.cache.keys()[self.iterkey]
        self.cache.keys[key]
        self.iterkey = self.iterkey + 1

class ObjVerts(ItemCache):
    def __init__(self,so):
        self.so = so
        ItemCache.__init__(self)

    def _itemCacheSize(self):
        return flux.app.vertCount(self.so.objectId)

    def _itemGet(self,item):
        vtuple = flux.app.getVert(self.so.objectId,item)
        newVert = Vert(item, vtuple[0],vtuple[1],vtuple[2],False)
        return newVert


class ObjEdges(ItemCache):
    def __init__(self,so):
        self.so = so
        ItemCache.__init__(self)

    def _itemCacheSize(self):
        return flux.app.edgeCount(self.so.objectId)

    def _itemGet(self,item):
        etuple = flux.app.getEdge(self.so.objectId,item)
        newEdge = Edge(self.so, item, etuple[0],etuple[1],False)
        return newEdge

class ObjFaces(ItemCache):
    def __init__(self,so):
        self.so = so
        ItemCache.__init__(self)

    def _itemCacheSize(self):
        return flux.app.faceCount(self.so.objectId)

    def _itemGet(self,item):
        ftuple = flux.app.getFace(self.so.objectId,item)
        cTuple = ftuple[2]
        centroid = V3(cTuple[0],cTuple[1],cTuple[2])
        normalTuple = ftuple[3]
        normal = V3(normalTuple[0],normalTuple[1],normalTuple[2])
        newFace = Face(self.so,item,ftuple[0],ftuple[1],ftuple[4],centroid,normal)
        return newFace

class ObjHalfEdges(dict):
    def __init__(self, so):
        self.cache = {}
        self.so = so

    def __getitem__(self,item):
        if self.cache.has_key(item):
            return self.cache[item]
        else:
            i = self._itemGet(item)
            self.cache[item]=i
            return i

    def _itemGet(self,item):
        hetuple = flux.app.getHalfEdge(self.so.objectId,item)
        if hetuple == None:
            return None
        he = HalfEdge(self.so,hetuple[0],0,0,0,0)
        return he

    def refresh(self):
        self.cache={}

class Object3d(object):
    def __init__(self,soid):
        self.objectId = soid
        self.faces = ObjFaces(self)
        self.edgeMap = {}
        self.edges = ObjEdges(self)
        self.verts = ObjVerts(self)
        self.halfEdges = ObjHalfEdges(self)
        self.bb = []

class ObjectBuilder:
    def __init__self(self):
        self.verts = []
        self.faces = []

class SceneView(object):
    pass


#### MODULE FUNCTIONS

def makeEdgeName(vida, vidb):
    return "%i-%i" % (vida,vidb)

if not flux.mode.standalone:
    flux = Flux()
    flux.sync()
