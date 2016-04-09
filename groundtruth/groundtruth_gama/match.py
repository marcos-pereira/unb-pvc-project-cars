#!/usr/bin/python

import sys

import numpy as np

if len(sys.argv) > 2:
	GT = np.loadtxt( sys.argv[1], delimiter= ';' )
	Results = np.loadtxt( sys.argv[2], delimiter= ';' )

print "\nResults Obtained:\n"
print Results 
print "\nGT:\n"
print GT 

lastGT = GT.shape
#i = GT[lastGT]

print "Last GT"
print lastGT

i = lastGT[0] - 1

lastFrame = GT[ i, 0 ]
print "lastFrame"
print lastFrame

for x in range(lastFrame.astype(int) + 1):
	print "Frame ", x
	for row in GT:
		if row[0] == x:
			print row