#import flux.mode
#flux.mode.standalone = True
from flux.root import *
from flux.io.ObjParser import *
o = ObjParser()
print "Parsing file!!"
sb = o.parseFile(r"c:\src\grid.obj")
sb.sendToFlux()
print "Done!"
