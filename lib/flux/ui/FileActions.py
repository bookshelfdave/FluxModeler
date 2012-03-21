
from flux.core import *
from flux.io.ObjParser import *
from flux.input import *


class FileOpen(MenuAction):
    def action(self,data):
        o = ObjParser()
        print "Parsing file:",data
        #sb = o.parseFile(r"c:\src\grid.obj")
        sb = o.parseFile(data)
        sb.sendToFlux()
        print "Done sending to flux!"

