########################################
#### MODEL CHANGES
########################################

class InputStateStack(object):
    def pushSelectMode(self):
        pass
    def pushSceneManipMode(self):
        pass
    def pushObjManipMode(self):
        pass
    def pushGeomManipMode(self):
        pass
    def pushNoneMode(self):
        pass
    def pushInvalidMode(self):
        pass
    def popMode(self):
        pass
    def peekMode(self):
        pass

class ModelChangeUndo:
    pass
        
class ModelChange:   
    ## maybe i'll get that abstractmethod stuff working some time
    def modelChange(self):
        # needs to generate an ModelChangeUndo object
        pass


class MoveVertGroupChange(ModelChange):
    def __init__(self,verts,deltax,deltay,deltaz):
        self.verts=verts
        self.deltax = deltax
        self.deltay = deltay
        self.deltaz = deltaz

    def modelChange(self):
        pass


class MoveVertChange(ModelChange):
    def __init__(self,soid,vid,newx,newy,newz):
        self.vid  = vid
        self.newx = newx
        self.newy = newy
        self.newz = newz
        
    def modelChange(self):
        print "Moving vert:",self.vid


class DeleteVertChange(ModelChange):
    def __init__(self,soid,vid):
        self.vid = vid

    def modelChange(self):
        print "Deleting vert:",self.vid


class SplitFaceChange(ModelChange):
    def __init_(self):
        print "Splitting face"


class CreateEdgeChange(ModelChange):
    def __init(self):
        print "Creating edge"

        
class ChangeQueue:
    def __init__(self):
        self.changes = []

    def queueChange(self,change):
        self.changes.append(change)

    def processChanges(self):
        for change in self.changes:
            change.modelChange()
        self.changes = []


class X:
    def __init__(self):
        print "New X"

    def __getitem__(self, item):
        if item < 10:
            return 1

