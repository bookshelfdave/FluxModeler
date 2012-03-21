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
        pass
        
class Point(MetaObj):
    def __init__(self,x=0.0,y=0.0,z=0.0,selected=False):
        params = {"x":x,"y":y,"z":z,"selected":selected}
        MetaObj.__init__(self,params)
        
    def __str__(self):
        return "Point <%r,%r,%r> %s" % (self.x,self.y,self.z,self.dirty)


    
for item in MetaObj.changeQueue:
    print item
p0 = Point()
p = Point(1.0,2.0,3.0)
p2 = Point(4.0,5.0,6.0)
p3 = Point(100.0,200.0,300.0)
print p0
print p
print p2
print p3
print "xxxxxx"
p.x=5.0
p3.y = 50
print "-----"
for item in MetaObj.changeQueue:
    print item
