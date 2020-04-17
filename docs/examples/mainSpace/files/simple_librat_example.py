#!/bin/env python
import numpy as np
import librat

# Simulation parameters
viewZenith = 57.3
viewAzimuth = 0.0
nBands = 1
minwavelength = [1064.0]
bandwidth = [1.0]
rtd = 1
scene = "chat0101_scene_clones_random.obj"

# Initialise librat object
ratObj = librat.RATinit()
status = librat.RATparse(ratObj, 0, None, None) # Need to do to initialise default values
librat.RATsetWavefrontFilename(ratObj,scene)
ratObj.materialbag.samples.max_diffuse_sampling_depth = rtd
ratObj.flagbag.max_ray_depth = rtd # Do I need to do this?
librat.RATsetNWavebands(ratObj, nBands, minwavelength, bandwidth)

# Read scene
print "Reading %s" % scene
if librat.RATisWavefrontFile(ratObj):
    librat.RATreadObject(ratObj)
else:
    print "%s is not a wavefront file" % scene
    sys.exit()

# Define direction vector
r = np.cos(np.radians(viewZenith))
rDirection = [r*np.cos(np.radians(viewAzimuth)), r*np.sin(np.radians(viewAzimuth)), np.sin(np.radians(viewZenith))]

# Define sensor position
rFrom = [0.0, 0.0, 0.5]

# Do ray tracing
status = librat.RATtraceRay(ratObj,rFrom,rDirection,None)

# Get result
#   ratTree = [n,thisRTD,intersectionPoints,rayLengths,ratmat,
#   diffusePathIntersectType,directPathIntersectType,hitSun,hitSky,
#   diffuseRadiance,directRadiance,lengthToSun,angleToSun]
ratTree = librat.RATgetRatTree(ratObj)

# Print result if there is an interception
if (ratTree[1] != -1):
    hit = 1
    material = ratTree[4][0]
    direct = np.sum(ratTree[10]) 
    diffuse = np.sum(ratTree[9])
