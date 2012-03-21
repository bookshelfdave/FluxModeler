import sys
import time
segs = sys.path[0].split("\\")
print segs
parent = r"\\".join(segs[0:-1])
print "Parent:",parent
sys.path.append(parent)

from flux.root import *
import flux.io.ObjParser

print "Starting to parse"
start = time.time()
o = flux.io.ObjParser.ObjParser()
o.parseFile(r"c:\src\huge.obj")
finish = time.time()
print "Time:",finish - start

