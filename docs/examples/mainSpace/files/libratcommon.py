#!/bin/env python
"""
Python classes for interacting with librat and developing 3D scenes.
John Armston (j.armston@uq.edu.au)
"""

from numpy import *
from scipy import spatial
from osgeo import gdal
from osgeo.gdalconst import *
from matplotlib import nxutils
import os, sys
import utilities
import librat


class libratMaps: 
    """
    Methods for creating height, cover and material maps of scenes using start
    """
    
    def __init__(self, scene):
        self.scene = scene
        self.extent = [50,50]
        self.centre = [0,0]
        self.resolution = 0.1
        self.map = None
        self.type = None
        self.boom = 200
    
    
    def getDims(self):
        """
        Calculate scene dimensions
        """
        xDim = int(floor(self.extent[0]/self.resolution))
        yDim = int(floor(self.extent[1]/self.resolution))
        return (xDim,yDim)
        
    
    def crownMap(self, objList):
        """
        Create a crown map. Crowns area treated as the convex hull of the tree material.
        Image values represent the number of intercepted tree crowns.
        Returns the vertically projected fractional crown cover of the scene.
        Only live trees are included, as measured in the field.
        """
        xDim, yDim = self.getDims()
        crowns = zeros([xDim,yDim], dtype=uint8)
        xi = repeat([arange(xDim)*self.resolution-self.extent[0]/2.0+self.resolution/2.0],yDim,axis=0)
        yi = repeat([arange(yDim)*self.resolution-self.extent[1]/2.0+self.resolution/2.0],xDim,axis=0).transpose()
        points = array([xi.flatten(),yi.flatten()]).transpose()
        obj = libratScene()
        obj.readObjList(objList,positions=True)
        for i,tree in enumerate(obj.scene["object"]):
            if (obj.scene["status"][i] == "live"):
                treeObj = libratObject(tree)
                xyz = treeObj.get_vertices()
                chull = utilities.convex_hull(xyz[:,0],xyz[:,1])
                xr,yr = utilities.rotate2D(chull[:,0], chull[:,1], obj.scene["rotation"][i])
                xr0 = append(xr,xr[0])
                yr0 = append(yr,yr[0])
                poly = array([xr0+obj.scene["x"][i],yr0+obj.scene["y"][i]]).transpose()
                inside = nxutils.points_inside_poly(points, poly).reshape((xDim,yDim))
                crowns = where(inside, crowns+uint(1), crowns)
                
        self.type = "crown"
        self.map = crowns


    def materialMap(self, outFile=None):
        """
        Created a material map. Depends on a version of librats "start" program with option 17 added.
        Image values represent the ID of the highest intercepted scene material within that pixel.
        """
        xDim, yDim = self.getDims()
        if (outFile is None):
            fparts = self.scene.split(".")
            outFile = fparts[0] + "_material.hips"
        cmd = "echo 17 %i %i %i %i %i %i %i 5 %s | start_v20 -RATtolerance 0.0001 %s" % \
        (self.centre[0],self.centre[1],self.boom,self.extent[0],self.extent[1],xDim,yDim,outFile,self.scene)
        os.system(cmd)
        self.type = "material"
        header,self.map = self.readHIPS(outFile)


    def materialIndex(self):
        """
        Get material index for material map
        """      
        ratObj = librat.RATinit()
        status = librat.RATparse(ratObj, 0, None, None)
        librat.RATsetWavefrontFilename(ratObj,self.scene)
        librat.RATreadObject(ratObj)
        matObj,nMat = librat.RATgetMaterials(ratObj)
        name = []
        index = []
        for i in range(nMat):
            result = librat.RATtranslateMaterial(ratObj,i)
            if result is not -1:
                name.append(result[0])
                index.append(result[1])
        ratObj = None
        matObj = None
        return (name,index)
        

    def heightMap(self, outFile=None):
        """
        Created a height (above ground) map. Depends on librats "start" program.
        Image values represent the height of the first intercepted scene material.
        """
        xDim, yDim = self.getDims()
        if (outFile is None):
            fparts = self.scene.split(".")
            outFile = fparts[0] + "_height.hips"
        cmd = "echo 16 %i %i %i %i %i %i %i 1 %s | start_v20 -RATtolerance 0.0001 %s" % \
        (self.centre[0],self.centre[1],self.boom,self.extent[0],self.extent[1],xDim,yDim,outFile,self.scene)
        os.system(cmd)
        self.type = "height"
        header,self.map = self.readHIPS(outFile)


    def readHIPS(self, inFile):
        """
        Read a HIPS file header and image
        """  
        hipsObj = open(inFile, 'rb')
        header = {}
        header['orig_name'] = hipsObj.readline().strip('\r\n')
        header['seq_name'] = hipsObj.readline().strip('\r\n')
        header['num_frame'] = int(hipsObj.readline().strip('\r\n'))
        header['orig_date'] = hipsObj.readline().strip('\r\n')
        header['rows'] = int(hipsObj.readline().strip('\r\n'))
        header['cols'] = int(hipsObj.readline().strip('\r\n'))
        header['bits_per_pixel'] = int(hipsObj.readline().strip('\r\n'))
        header['bit_packing'] = int(hipsObj.readline().strip('\r\n'))
        header['pixel_format'] = int(hipsObj.readline().strip('\r\n'))
        header['seq_history'] = hipsObj.readline().strip('\r\n')
        header['seq_desc'] = hipsObj.readline().strip('\r\n')
        endHeader = hipsObj.readline()
        while (endHeader != '.\n'):
            header['seq_desc'] += endHeader.strip('\r\n')
            endHeader = hipsObj.readline()
        if (header['pixel_format'] == 0):
            image = fromfile(hipsObj,dtype=uint8).reshape((header['rows'],header['cols']))
        elif (header['pixel_format'] == 1):
            image = fromfile(hipsObj,dtype=uint16).reshape((header['rows'],header['cols']))
        elif (header['pixel_format'] == 2):
            image = fromfile(hipsObj,dtype=uint32).reshape((header['rows'],header['cols']))
        elif (header['pixel_format'] == 3):
            image = fromfile(hipsObj,dtype=float32).reshape((header['rows'],header['cols']))
            image = where(image < finfo(float32).eps, 0.0, image)
        elif (header['pixel_format'] == 4):
            image = fromfile(hipsObj,dtype=complex64).reshape((header['rows'],header['cols']))
        elif (header['pixel_format'] == 5):
            image = fromfile(hipsObj,dtype=str).reshape((header['rows'],header['cols']))
        else:
            print "HIPS pixel format %i not supported" % header['pixel_format']
            sys.exit()
        hipsObj.close()
        return (header, image)
        
        
    def writeImage(self, outFile=None, format="ENVI"):
        """
        Writes a map to a GDAL supported image file format.
        """
        xDim, yDim = self.getDims()
        if (self.type == "height"):
            dType = GDT_CFloat32
        else:
            dType = GDT_Byte
        driver = gdal.GetDriverByName(format)
        if (outFile is None):
            fparts = self.scene.split(".")
            outFile = fparts[0] + "_%s.%s" % (self.type,format.lower())
        dst_ds = driver.Create(outFile, xDim, yDim, 1, dType)
        dst_ds.GetRasterBand(1).WriteArray(self.map)
        dst_ds = None
        


class libratScene:    
    """
    Methods for creating and querying scenes of librat wavefront object files
    Currently only flat ground of constant material ("soil") is implemented
    """

    def __init__(self):
        self.scene = {}
        self.nObjects = 0
        self.extent = [50,50]


    def sortBoundingBoxes(self, nBoxesX=20, nBoxesY=20):
        """
        Sorts the object positions into bounding boxes
        Returns a tuple of arrays with the x,y,n box number each object falls within
        """      
        xspacing = self.extent[0] / nBoxesX
        yspacing = self.extent[1] / nBoxesY
        
        xbins = (arange(nBoxesX+1) * xspacing) - (self.extent[0] / 2.0)
        ybins = (arange(nBoxesY+1) * yspacing) - (self.extent[1] / 2.0)
        
        dx = digitize(self.scene["x"], xbins)
        dy = digitize(self.scene["y"], ybins)
        
        dn = (dx-1) + (nBoxesX * (dy-1))
        return (dx,dy,dn)


    def readObjList(self, objList, positions=False):
        """
        Read a comma delimited file (e.g., plants.dat) of objects to include within the scene
        When positions are defined (e.g., by field measurement), the CSV fields are:
            object,species,status,x,y,z,rotation (set rotation to zero if done later)
        Status can be "live" or "dead"
        When a random or Neyman Type A distribution will be used to define object positions, the CSV fields are:
            object,species,status,nClones
        A dictionary with the keys object,species,status,x,y,z,rotation is returned.
        """
        sceneObj = open(objList, 'r')
        sceneDict = {}
        sceneDict["object"] = []
        sceneDict["species"] = []
        sceneDict["status"] = []
        sceneDict["x"] = []
        sceneDict["y"] = []
        sceneDict["z"] = []
        sceneDict["rotation"] = []

        for line in sceneObj:
            lparts = line.strip('\r\n').split(',')

            if positions:
                sceneDict["object"].append(lparts[0])
                sceneDict["species"].append(lparts[1])
                sceneDict["status"].append(lparts[2])
                sceneDict["x"].append(float(lparts[3]))
                sceneDict["y"].append(float(lparts[4]))
                sceneDict["z"].append(float(lparts[5]))
                sceneDict["rotation"].append(float(lparts[6]))
            else:
                nClones = int(lparts[3])
                for clone in range(nClones):
                    sceneDict["object"].append(lparts[0])
                    sceneDict["species"].append(lparts[1])
                    sceneDict["status"].append(lparts[2])
                    sceneDict["x"].append(0.0)
                    sceneDict["y"].append(0.0)
                    sceneDict["z"].append(0.0)
                    sceneDict["rotation"].append(0.0)             

        sceneObj.close()
        self.nObjects = len(sceneDict["object"])
        self.scene = sceneDict


    def writeObjList(self, outFile):
        """
        Write a comma delimited file (e.g., plants.dat) of objects included within a scene and their positions
        """
        sceneObj = open(outFile, 'w')
        for i in range(self.nObjects):
            line = "%s,%s,%s,%f,%f,%f,%f\n" % (self.scene["object"][i],self.scene["species"][i],self.scene["status"][i], \
            self.scene["x"][i],self.scene["y"][i],self.scene["z"][i],self.scene["rotation"][i])
            sceneObj.write(line)
        sceneObj.close()


    def setExtent(self, value):
        """
        Set the spatial extent of the scene
        Input needs to be a two element list: [x,y]
        """
        self.extent = value


    def rotateObjects(self, low=0.0, high=360.0):
        """
        Randomly rotate objects between low and high thresholds in degrees.
        If not called, object rotation is set to zero in readObjList by default.
        """
        rTemp = random.uniform(low=low, high=high, size=self.nObjects)
        self.scene["rotation"] = [r for r in rTemp]


    def calcObjectHeights(self):
        """
        Currently sets the scene height of each object to zero.
        This function will be modified in future once complex terrain is introduced
        """
        self.scene["z"] = [0] * self.nObjects

        
    def writeScene(self, outFile, matlib, nBoxes=20, upSize=True, cloneScene=1, rotateCloneScene=True, grassScene=None):
        """
        Writes the scene to a wavefront format file for input to librat
        Assumes the ground is flat and of constant material ("soil")
        Set upSize to True to clone scene by the levels set.
        """
        sceneObj = open(outFile, 'w')
        sceneObj.write("!{\n")
        sceneObj.write("#define\n")
        sceneObj.write("g lidarScene0 0\n")
        sceneObj.write("!{\n")
        sceneObj.write("mtllib %s\n" % matlib)
        sceneObj.write("!{\n")
        sceneObj.write("usemtl soil\n")
        sceneObj.write("v 0 0 0\n")
        sceneObj.write("v 0 0 1\n")
        sceneObj.write("plane -1 -2\n")

        uobject = unique(self.scene["object"])
        for tree in uobject:
            sceneObj.write("#include %s\n" % tree)
        
        if (grassScene is not None):
            grassObj = libratObject(grassScene)
            grassInc = grassObj.get_incFiles()
            grassName = grassObj.get_objName()
            for grass in grassInc:
                sceneObj.write("#include %s\n" % grass)
            sceneObj.write("#include %s\n" % grassScene)
            sceneObj.write("!{\n")
            sceneObj.write("clone 0 0 0 0 %s 0\n" % grassName)
            sceneObj.write("!}\n")
        
        dx,dy,dn = self.sortBoundingBoxes(nBoxesX=nBoxes, nBoxesY=nBoxes)
        udn = unique(dn)
        for box in udn:
            sceneObj.write("!{\n")
            index = where(dn == box)
            for i in index[0]:
                sceneObj.write("clone %f %f %f %f %s 0\n" % (self.scene["x"][i],self.scene["y"][i],self.scene["z"][i],self.scene["rotation"][i],self.scene["object"][i]))
            sceneObj.write("!}\n")

        sceneObj.write("!}\n")
        sceneObj.write("!}\n")
        sceneObj.write("!}\n")

        if cloneScene:
            sRotations = [0,90,180,270]
            for level in range(cloneScene):
                sceneObj.write("!{\n")
                if (level == 0): sceneObj.write("#define\n")
                sceneObj.write("g lidarScene%i 0\n" % (level+1))
                offset = 2**(level+1)-1
                for x in range(-1,2,1):
                    for y in range(-1,2,1):
                        if rotateCloneScene:
                            if logical_and(x == 0, y == 0):
                                sRotation = 0.0
                            else:
                                sRotation = sRotations[floor(random.uniform() * 4)]
                        else:
                            sRotation = 0.0
                        sceneObj.write("clone %f %f 0 %f lidarScene%i 0\n" % (self.extent[0]*x*offset,self.extent[1]*y*offset,sRotation,level))
                sceneObj.write("!}\n")

        sceneObj.close()

        
    def positionsNeymanA(self, m2=1, quadDims=[5,5]):
        """
        Calculates x and y positions for the Neyman Type A distribution
        See the defineNeymanA function for details on input parameters
        """
        nQuad = quadDims[0] * quadDims[1]
        xQuadSize = self.extent[0] / quadDims[0]
        yQuadSize = self.extent[1] / quadDims[1]

        trees, prob = utilities.defineNeymanA(nPoints=self.nObjects, m2=m2, nQuad=nQuad)
        cumNeyman = cumsum(prob)
        maxTrees = amax(trees)
        index = random.uniform(low=0, high=max(cumNeyman), size=nQuad)

        xPos = []
        yPos = []
        for yquad in arange(quadDims[1]):
            for xquad in arange(quadDims[0]):
                quad = xquad + (quadDims[0] * yquad)
                idx = where(logical_and(index[quad] >= cumNeyman, index[quad] < cumNeyman))
                nTrees = trees[idx]
                xCen = (xQuadSize * xquad) + xQuadSize
                yCen = (yQuadSize * yquad) + yQuadSize
                xTemp = random.uniform(low=-(xQuadSize/2.0)+xCen, high=(xQuadSize/2.0)+xCen, size=trees[idx])
                yTemp = random.uniform(low=-(yQuadSize/2.0)+yCen, high=(yQuadSize/2.0)+yCen, size=trees[idx])
                xPos.append([x for x in xTemp])
                yPos.append([y for y in xTemp])

        self.scene["x"] = [x for x in xPos]
        self.scene["y"] = [y for y in yPos]


    def positionsRandom(self):
        """
        Calculates x and y positions for a random distribution of trees
        """    
        xPos = random.uniform(low=-(self.extent[0]/2.0), high=(self.extent[0]/2.0), size=self.nObjects)
        yPos = random.uniform(low=-(self.extent[1]/2.0), high=(self.extent[1]/2.0), size=self.nObjects)
        self.scene["x"] = [x for x in xPos]
        self.scene["y"] = [y for y in yPos]


    def checkTreeSpacing(self, minSpacing=2, maxIter=1000):
        """
        Recalculates random x/y coordinates for points that are within a set distance to their nearest neighbor.
        If the maximum number of iterations is reached, the points within minSpacing are retained.
        Returns the remaining number of points within minSpacing
        """
        pos = array([self.scene["x"],self.scene["y"]]).transpose()

        for i in arange(maxIter):

            dm = spatial.distance.pdist(pos, 'euclidean')
            index = where(dm < minSpacing)
            n = len(index[0])

            if (n > 0):
                xTemp = random.uniform(low=-(self.extent[0]/2.0), high=(self.extent[0]/2.0), size=n)
                yTemp = random.uniform(low=-(self.extent[1]/2.0), high=(self.extent[1]/2.0), size=n)
                #ix,jx = triu_indices(self.nObjects,1)
                ix,jx = where(subtract.outer(arange(self.nObjects), arange(self.nObjects)) < 0)
                pos[ix[index]] = array([xTemp,yTemp]).transpose()
            else:
                break

        self.scene["x"] = [item[0] for item in pos]
        self.scene["y"] = [item[1] for item in pos]
        return n
        


class libratObject:    
    """
    Methods for querying and modifying librat objects included within a scene
    """

    def __init__(self, wavefrontFile):     
        self.object = wavefrontFile

    
    def get_materials(self):
        """
        Returns a list of materials referenced in object
        """
        mat = list(set([line[6:].strip() for line in open(self.object, 'r') if line[0:6] == "usemtl"]))
        incFiles = self.get_incFiles()
        for incFile in incFiles:
            tempObj = self.__class__(incFile)
            temp = tempObj.get_materials()
            for tempMat in temp:
                mat.append(tempMat)
        return set(mat)


    def get_sceneName(self):
        """
        Returns the scene name
        """
        groups = [line.split(" ")[1] for line in open(self.object, 'r') if line[0:3] == "g "]
        return groups[0]
    
    
    def replace_material(self, old, new):
        """
        Replaces a material name with another
        Used when using different materials for different species
        """
        oldMaterial = "usemtl %s" % old
        newMaterial = "usemtl %s" % new
        newLines = [line.replace(oldMaterial,newMaterial) for line in open(self.object, 'r')]
        outFile = open(self.object, 'w')
        for line in newLines:
            outFile.write(line)    
        outFile.close()
    
    
    def get_bbox(self):
        """
        Get bounding box of object
        """
        xyz = self.get_vertices()
        if len(xyz) > 0:
            return array([amin(xyz, axis=0), amax(xyz, axis=0)])
        else:
            return None
   
    
    def get_maxH(self):
        """
        Get maximum height of object
        """
        maxH = 0.0
        bbox = self.get_bbox()
        if bbox is not None: maxH = bbox[1,2]
        incFiles = self.get_incFiles()
        for incFile in incFiles:
            tempObj = self.__class__(incFile)
            bbox = tempObj.get_bbox()
            if bbox is not None: maxH = max(maxH, bbox[1,2])
        return maxH
    
    
    def get_nMat(self):
        """
        Returns the total number of materials referenced in object, including sub-objects referenced by #include statements
        """
        mat = self.get_materials()
        return len(set(mat))
    
    
    def get_incFiles(self):
        """
        Returns list of object files referenced by #include statements
        """
        incFiles = [line[8:].strip() for line in open(self.object, 'r') if line[0:8] == "#include"]
        return incFiles
    
    
    def get_vertices(self):
        """
        Returns x,y,z of all vertices as an m by n array
        """
        xyz = [line[2:].strip("\r\n").split(" ") for line in open(self.object, 'r') if line[0] == "v"]
        return array(xyz, dtype="Float32")
        
    
    
class OnyxTree:
    """
    Methods for querying and modifying *.obj wavefront files produced by the software OnyxTree
    Filtering/bounding of OnyxTree files for input to librat uses UCL AWK scripts by Mat Disney.
    Vertex indices and normals are present, therefore we can do more here than with filtered librat objects.
    Remember that OnyxTree uses a "left-handed" coordinate system so need to switch y and z vertices
    """
    
    def __init__(self, objFile):     
        self.fileName = objFile
        self.object = {}


    def readObj(self):
        """
        Read OnyxTree wavefront object
        """
        self.object["v"] = []
        self.object["vn"] = []
        self.object["vt"] = []
        self.object["f"] = []
        self.object["vid"] = []
        self.object["fid"] = []
        self.object["subObject"] = []
        self.object["g"] = []
        self.object["s"] = []

        obj = open(self.fileName, 'r')
        for line in obj:
            lparts = line.strip('\r\n').split(' ')

            if (len(lparts) > 1):
                if (lparts[1] == "Sub-Object"):
                    self.object["subObject"].append(lparts[2])
                elif (lparts[0] == "v"):
                    self.object["v"].append([float(i) for i in lparts[1:]])
                    self.object["vid"].append(self.object["subObject"][-1])
                elif (lparts[0] == "vn"):
                    self.object["vn"].append([float(i) for i in lparts[1:]])                   
                elif (lparts[0] == "vt"):
                    self.object["vt"].append([float(i) for i in lparts[1:]])
                elif (lparts[0] == "f"):
                    self.object["f"].append([int(index.split('/')[0]) for index in lparts[1:]])
                    self.object["fid"].append(self.object["subObject"][-1])
                elif (lparts[0] == "s"):
                    self.object["s"].append(int(lparts[1]))
                elif (lparts[0] == "g"):
                    self.object["g"].append(lparts[1])                                       

        obj.close()

    
    def getItem(self, item):
        """
        Get OnyxTree object item
        """
        return self.object[item]            

    
    def writeOnyxObj(self, outFileName):
        """
        Write the OnyxTree wavefront object
        outFileName must be provided
        Original copyright statement is retained.
        """
        obj = open(outFileName, 'w')
        obj.write("# Onyx Obj Export Version 1.4 Aug 17th, 2000\n")
        obj.write("# Copyright 2000 by Onyx Computing, Inc. All rights reserved.\n")
        obj.write("\n")
        obj.write("# Warning: This computer model is protected by copyright law and\n")
        obj.write("# international treaties. Unauthorized reproduction or distribution of\n")
        obj.write("# this model, or any derivative or portion of it, may result in severe\n")
        obj.write("# civil or criminal penalties, and will be prosecuted to the maximum\n")
        obj.write("# extent possible under the law.\n")
        obj.write("\n")
        obj.write("mtllib %s.mtl\n" % self.fileName.split('.')[0])
        obj.write("\n")
        obj.write("g OnyxTREE\n")
        obj.write("\n")

        for i,subObject in enumerate(self.object["subObject"]):
            obj.write("# Sub-Object %s\n" % subObject)
            obj.write("#\n")

            nv = self.object["vid"].count(subObject)
            iv = self.object["vid"].index(subObject)
            for vertex in self.object["v"][iv:iv+nv]:
                obj.write("v %f %f %f\n" % (vertex[0],vertex[1],vertex[2]))
            obj.write("# %i vertices\n" % nv)
            obj.write("\n")

            for vertex in self.object["vt"][iv:iv+nv]:
                obj.write("vt %f %f %f\n" % (vertex[0],vertex[1],vertex[2]))
            obj.write("# %i texture vertices\n" % nv)            
            obj.write("\n")

            for vertex in self.object["vn"][iv:iv+nv]:
                obj.write("vn %f %f %f\n" % (vertex[0],vertex[1],vertex[2]))
            obj.write("# %i normals\n" % nv)
            obj.write("\n")
            obj.write("usemtl %s\n" % subObject)
            obj.write("s %i\n" % self.object["s"][i])
            obj.write("g %s\n" % self.object["g"][i+1])

            nv = self.object["fid"].count(subObject)
            iv = self.object["fid"].index(subObject)
            for face in self.object["f"][iv:iv+nv]:
                obj.write("f %i/%i/%i %i/%i/%i %i/%i/%i\n" % (face[0],face[0],face[0],face[1],face[1],face[1],face[2],face[2],face[2]))            
            if (subObject == "Leaf01"):
                obj.write("# %i faces\n" % nv)
            obj.write("\n")

        obj.write("# EOF\n")                  
        obj.close()
        self.FileName = outFileName

        
    def filterObj(self, outFile):
        """
        Filters OnyxTree wavefront objects files and sorts into bounding boxes for librat.
        This is simply a wrapper for the filtOnyx AWK/CSH script by Mat Disney (renamed filtOnyxTree and with minor modifications)
        Dependencies (need to be in your path):
           filtOnyxTree
           filtOnyxTree.awk
           boundObj.awk
           clearEmpties.awk
           wavefront_bbox
        """
        if not os.path.isfile(outFile):
            os.system("filtOnyxTree -i %s -o %s -bbox -matmap" % (self.fileName, outFile))
        else:
            print "      %s already exists" % outFile

    
    def getLeafAngles(self):
        """
        Calculates zenith of all individual leaves from vector normals.
        Assumes each leaf has two triangles and constant vertex normals (i.e., flat).
        The dot product of two unit vectors is equal to the cosine of the angle between them.
        The arccos of the dot product of the vector normal and the "up" vector normal gives
        the angle of the leaf facet (from the horizontal)      
        """
        index = where(logical_or(array(self.object["vid"]) == "Leaf1",array(self.object["vid"]) == "Leaf01"))
        if (len(index[0]) > 0):
            a = array(self.object["vn"])[index][arange(0,len(index[0])-1,4)]
            b = array([0,1,0])
            angles = arccos(dot(a,b)) * 180.0 / pi
            return angles
        else:
            return None


    def setLeafAngles(self,distribution="random"):
        """
        Adjusts zenith and vertex normal of all leaf plates to correspond to a known leaf angle distribution.
        Currently only uniform random distribution (spherical) is implemented.
        """
        
        # Translate so rotation axis runs through origin
        # Origin is leaf base
        leafVertices = self.getLeafVertices()
        if leafVertices is not None:
            x,y,z,nx,ny,nz = leafVertices
            xv = x[:,0] - x[:,1]
            x0 = repeat([x[:,0] + xv/2.0],4,axis=0).transpose()
            xt = x - x0
            yv = y[:,0] - y[:,1]
            y0 = repeat([y[:,0] + yv/2.0],4,axis=0).transpose()
            yt = y - y0
            zv = z[:,0] - z[:,1]
            z0 = repeat([z[:,0] + zv/2.0],4,axis=0).transpose()
            zt = z - z0

            # Reformat axis vectors about which to perform rotation
            # For leaves these are the equivalent of the plate base (side leaf base is on)
            xv = repeat([xv],4,axis=0).transpose()
            yv = repeat([yv],4,axis=0).transpose()
            zv = repeat([zv],4,axis=0).transpose()

            # Adjust zenith
            zenith = repeat([self.getLeafAngles()],4,axis=0).transpose()
            if (distribution == "random"):
                dims = zenith.shape
                newLeafAngle = repeat([random.uniform(low=0.0001, high=90.0-0.0001, size=dims[0])],4,axis=0).transpose()
                adjustment = newLeafAngle - zenith 
            else:
                print "%s not yet implemented" % distribution
                sys.exit()
            xr,yr,zr = utilities.rotate3D(xv, yv, zv, xt, yt, zt, adjustment)

            # Recalculate normals
            nx,ny,nz = utilities.calcFaceNormal(xr+x0,yr+y0,zr+z0)

            # Send the results
            self.putLeafVertices(xr+x0,yr+y0,zr+z0,nx,ny,nz)
        
               
    def getFacetVertices(self,material="Leaf01"):
        """
        Returns n by m arrays for x, y and z vertices of facets.
        n is the number of facets and m is the number vertices per facet (3)
        """
        x = array(self.object["v"])[:,0]
        y = array(self.object["v"])[:,2]
        z = array(self.object["v"])[:,1]
        indexf = where(array(self.object["fid"]) == material)
        if (len(indexf[0]) > 0):       
            fx = x[array(self.object["f"])[indexf]-1]
            fy = y[array(self.object["f"])[indexf]-1]
            fz = z[array(self.object["f"])[indexf]-1]
            return (fx,fy,fz)
        else:
            return None


    def getVertices(self,material=None):
        """
        Returns 1D arrays for x, y and z vertices for a specified material
        """
        if (material is None):
            x = array(self.object["v"])[:,0]
            y = array(self.object["v"])[:,2]
            z = array(self.object["v"])[:,1]
        else:    
            index = where(array(self.object["vid"]) == material)
            x = array(self.object["v"])[index][:,0]
            y = array(self.object["v"])[index][:,2]
            z = array(self.object["v"])[index][:,1]
        return (x,y,z)


    def getLeafVertices(self):
        """
        Returns n by m arrays for x, y and z vertices and facet normals of entire leaves.
        n is the number of leaves and m is the number vertices per leaf (4)
        Assumes each leaf is flat and has two triangles
        """
        index = where(logical_or(array(self.object["vid"]) == "Leaf1",array(self.object["vid"]) == "Leaf01"))
        count = len(index[0])
        if (count > 0):
            x = array(self.object["v"])[index][:,0].reshape((count/4,4))
            y = array(self.object["v"])[index][:,2].reshape((count/4,4))
            z = array(self.object["v"])[index][:,1].reshape((count/4,4))
            nx = array(self.object["vn"])[index][:,0].reshape((count/4,4))
            ny = array(self.object["vn"])[index][:,2].reshape((count/4,4))
            nz = array(self.object["vn"])[index][:,1].reshape((count/4,4))
            return (x,y,z,nx,ny,nz)
        else:
            return None

   
    def putLeafVertices(self,x,y,z,nx,ny,nz):
        """
        Transforms n by m arrays for x, y and z vertices and vertex normals (n) of leaves
        resulting from getLeafVertices and updates self.object with the updated vertices
        """
        v = array([x.flatten(),z.flatten(),y.flatten()]).transpose()
        vn = array([[nx,nx,nx,nx],[nz,nz,nz,nz],[ny,ny,ny,ny]]).transpose().reshape(-1,3)
        nLeaf = len(v)
        newV = [v[i] for i in range(nLeaf)]
        newN = [vn[i] for i in range(nLeaf)]
        leafStart = self.object["vid"].index("Leaf01")
        self.object["v"][leafStart:leafStart+nLeaf] = newV
        self.object["vn"][leafStart:leafStart+nLeaf] = newN

  
    def getFacetArea(self,material="Leaf01",axis=None):
        """
        Calculates total area and centroid height of facets for a specified material
        For projected 2D area, set axis to 1 (x), 2 (y) or 3 (z). Default is 3D area.
        """
        fxyz = self.getFacetVertices(material=material)
        if (fxyz is not None):    
            if (axis == 1):
                fxyx[0][:] = 0.0
            elif (axis == 2):
                fxyz[1][:] = 0.0
            elif (axis == 3):
                fxyz[2][:] = 0.0
            area = utilities.calcHeronArea(fxyz[0], fxyz[1], fxyz[2])
            zc = mean(fxyz[2],axis=1)
            return (zc, area)
        else:
            return (0.0, 0.0)
        
        
    def getFacetAreaBin(self,material="Leaf01",binsize=0.5):
        """
        Calculates total area in height bins for a size and material
        There's got to be a more elegant way of doing this
        """
        fxyz = self.getFacetVertices(material=material)
        bins = arange(start=0.0, stop=50.0, step=binsize)
        area = zeros(len(bins))
        normal = array([0.0,0.0,1.0])

        for n,bin in enumerate(bins):

            # Get all facets overlapping bin
            index = where(logical_and(amax(fxyz[2], axis=1) >= bins[n], amin(fxyz[2], axis=1) <= (bin+binsize)))

            # Intersect each facet with bin
            for i in index[0]:

                # Get vertices of facets at bin edges
                fxBin = fxyz[0][i]
                fyBin = fxyz[1][i]
                fzBin = fxyz[2][i]
                for j in [[0,1],[0,2],[1,2]]:

                    p1 = array([fxBin[j[0]],fyBin[j[0]],fzBin[j[0]]])
                    p2 = array([fxBin[j[1]],fyBin[j[1]],fzBin[j[1]]])

                    # Intersect with lower bin edge
                    plint = utilities.planeInt(p1, p2, normal, array([0.0,0.0,bin]))
                    if logical_and(min(p1[2],p2[2]) < bin, max(p1[2],p2[2]) > bin):
                        fxBin = hstack((fxBin,plint[0]))
                        fyBin = hstack((fyBin,plint[1]))
                        fzBin = hstack((fzBin,plint[2]))

                    # Intersect with upper bin edge
                    plint = utilities.planeInt(p1, p2, normal, array([0.0,0.0,bin+binsize]))
                    if logical_and(min(p1[2],p2[2]) < (bin+binsize), max(p1[2],p2[2]) > (bin+binsize)):
                        fxBin = hstack((fxBin,plint[0]))
                        fyBin = hstack((fyBin,plint[1]))
                        fzBin = hstack((fzBin,plint[2]))                

                # Remove vertices outside bin and get area of resulting 3D polygon
                idx = where(logical_and(fzBin >= bin, fzBin <= (bin+binsize)))
                s = utilities.counterClockwiseSort(array([fxBin[idx],fyBin[idx],fzBin[idx]]).transpose())
                area[n] += utilities.calc3DPolyArea(fx=fxBin[idx][s], fy=fyBin[idx][s], fz=fzBin[idx][s]) 

        return (bins, area)

        
    def getCrownAttributes(self):
        """
        Calculates the projected vertical area and maximum width of the crown
        """    
        x,y,z = self.getVertices()
        chull = utilities.convex_hull(x,y)
        area = utilities.calc3DPolyArea(fx=chull[:,0], fy=chull[:,1], fz=None)
        dm = spatial.distance.pdist(chull, 'euclidean')
        return area, amax(dm)

        
    def getHeightAttributes(self):
        """
        Calculates top height, and base crown height defined by lowest leaf and lowest bough
        """      
        indexLeaf = where(logical_or(array(self.object["vid"]) == "Leaf1",array(self.object["vid"]) == "Leaf01"))
        indexBough = where(array(self.object["vid"]) == "Bough")
        z = array(self.object["v"])[:,1]
        if (len(indexLeaf[0]) > 0):
            hLeaf = amin(z[indexLeaf])
        else:
            hLeaf = -1
        if (len(indexBough[0]) > 0):
            hBough = amin(z[indexBough])
        else:
            hBough = -1            
        return (amax(z), hLeaf, hBough)
    
    
    def getStemDiameter(self, height=1.3):    
        """
        Calculates stem diameter at a nominated height in m
        Works out where facet edges intersect a plane at the set height.
        As done in the field, we then take the perimeter of convex hull and assume a circle for diameter.
        """          
        fx,fy,fz = self.getFacetVertices(material="Trunk")
        index = where(logical_and(amin(fz, axis=1) <= height, amax(fz, axis=1) >= height))

        for i in index[0]:

            for j in [[0,1],[0,2],[1,2]]:
                p1 = array([fx[i][j[0]],fy[i][j[0]],fz[i][j[0]]])
                p2 = array([fx[i][j[1]],fy[i][j[1]],fz[i][j[1]]])
                plint = utilities.planeInt(p1, p2, array([0.0,0.0,1.0]), array([0.0,0.0,height]))

                if logical_and(min(p1[2],p2[2]) <= height, max(p1[2],p2[2]) >= height):
                    try:
                        tVertices
                    except NameError:
                        tVertices = plint
                    else:
                        tVertices = vstack((tVertices,plint))

        chull = utilities.convex_hull(tVertices[:,0],tVertices[:,1])
        perimeter = sum(sqrt((chull[:,0]-roll(chull[:,0],1))**2+(chull[:,1]-roll(chull[:,1],1))**2))
        return 2.0 * perimeter / (2 * pi)
            



class libratConfig: 
    """
    Defines the light and camera files
    """
    
    def __init__(self):
        """
        Initialize dictionary
        """      
        self.items = {}

        
    def initAirborneLight(self, lidar=True):
        """
        Initialise an airborne lidar light file
        """
        self.items["camera.name"] = "simple illumination"
        self.items["geometry.perspective"] = True
        self.items["geometry.twist"] = 0.0
        self.items["geometry.lookAt"] = 0.0, 0.0, 0.0
        self.items["geometry.boomLength"] = 0.0
        self.items["geometry.azimuth"] = 0.0
        self.items["geometry.zenith"] = 0.0
        self.items["geometry.fieldOfView"] = 0.0
        self.items["samplingPattern.sd"] = 2.5, 2.5
        if (lidar is True):
            self.items["lidar.pulseStart"] = 500
            self.items["lidar.pulseForm"] = "gaussian"
            self.items["lidar.pulseSD"] = 0.53271920616767998
            self.items["lidar.pulseLength"] = 1000.0
  
            self.items["lidar.pulseSamples"] = 100

    def initAirborneCamera(self, lidar=True, images=False):
        """
        Initialise an airborne lidar camera file
        """   
        self.items = {}
        self.items["camera.name"] = "lidar"
        self.items["geometry.perspective"] = True
        self.items["geometry.lookAt"] = 0, 0, 0
        self.items["geometry.boomLength"] = 1000
        self.items["geometry.azimuth"] = 0.0
        self.items["geometry.zenith"] = 0.0
        self.items["geometry.twist"] = 0.0
        self.items["geometry.fieldOfView"] = 0.115
        self.items["samplingCharacteristics.nPixels"] = 262144
        self.items["samplingCharacteristics.rpp"] = 1

        if (images is True):
            self.items["result.image"] = "results/result.hips"

        self.items["result.integral.mode"] = "distance"
        self.items["result.integral"] = "results/results.dat"

        if (images is True):
            self.items["samplingPattern.OPImage"] = "results/testOPImage.hips"
            self.items["samplingPattern.sampleCountImage"] = "results/testsampleCountImage.hips"
            self.items["samplingPattern.gridMap"] = "results/testgridMap.hips"
            self.items["samplingPattern.modulationMap"] = "results/modmap.hips"

        self.items["samplingPattern.size"] = 512, 512
        self.items["samplingPattern.form"] = "gaussian"
        self.items["samplingPattern.sd"] = 2.5, 2.5
        self.items["samplingPattern.centre"] = 0, 0

        if (lidar is True):
            self.items["lidar.nBins"] = 150
            self.items["lidar.binStart"] = 1966
            self.items["lidar.binStep"] = 0.299792458            


    def getItem(self, item):
        """
        Get camera/light file item
        """
        return self.items[item]

           
    def setItem(self, item, value):
        """
        Set camera/light file item
        """
        self.items[item] = value


    def delItem(self, item):
        """
        Delete camera/light file item
        """
        self.items.pop(item)


    def writeFile(self, fileName='camera_default.dat'):
        """
        Write a camera/light file with current configuration
        """
        obj = open(os.path.join('camera',fileName), 'w')
        obj.write('camera {\n')
        for item in self.items:
            obj.write('    %s = %s;\n' % (item, self.items[item]))
        obj.write('}\n')
        obj.close()

    
    def calcAirborneLidar(self, maxH, delta=5.0, binStep=None):
        """
        Calculate derived airborne camera lidar parameters
        """
        if (binStep is not None): self.items["lidar.binStep"] = binStep
        maxHt = maxH / cos(radians(self.items["geometry.zenith"]))
        self.items["lidar.binStart"] = 2.0 * (self.items["geometry.boomLength"] - maxHt - delta)
        self.items["lidar.nBins"] = int(0.5 + (2.0 * self.items["geometry.boomLength"] - self.items["lidar.binStart"] + 2.0 * delta) / self.items["lidar.binStep"])

    
    def calcAirborneGeometry(self, azimuth=None, zenith=None, altitude=1000.0, footprint=0.5):
        """
        Calculate derived airborne lidar geometry parameters
        """
        if (azimuth is not None): self.items["geometry.azimuth"] = azimuth
        if (zenith is not None): self.items["geometry.zenith"] = zenith
        self.items["geometry.boomLen"] = altitude / radians(self.items["geometry.zenith"])
        idealArea = footprint / (2.0*sqrt(2.0*log(2.0))) * self.items["samplingPattern.sd"][0] * self.items["samplingPattern.sd"][1]
        self.items["geometry.fieldOfView"] = degrees(idealArea / altitude)
        

        
        
class pbsBatch:  
    """
    Running librat start jobs on the Queensland DERM HPC using PBS
    """
    
    def __init__(self):
        """
        Initialise job parameters
        """
        self.mem = 2
        self.walltime = "1:00:00"
        self.ncpu = 1
        self.group = "rscstd"
        self.path = '/scratch/rsc2/armstonj/lidar/simulations'
        self.option = 14
        self.object = "chat0103_scene_50m_v2.obj"
        self.wavebands = "wb.dat.lidar"
        self.nOrder = 100
        self.matlib = "chat0103.matlib"

    
    def writeJob(self, fileName, cameraFile, lightFile, verbose=False):
        """
        Write PBS file    
        """
        obj = open(os.path.join('pbs',fileName), 'w')
        obj.write('#!/bin/bash\n')
        obj.write('source /etc/profile.d/modules.sh\n')
        obj.write('module load librat\n')
        obj.write('cd %s\n' % self.path)

        if (verbose is True):
            obj.write('echo %i camera/%s camera/%s | start -v -m %i -RATsensor_wavebands %s %s\n' % \
                      (self.option, cameraFile, lightFile, self.nOrder, self.wavebands, self.object))
        else:
            obj.write('echo %i camera/%s camera/%s | start -m %i -RATsensor_wavebands %s %s\n' % \
                      (self.option, cameraFile, lightFile, self.nOrder, self.wavebands, self.object))

        obj.close()
        os.chmod(os.path.join('pbs',fileName), 0755)


    def submitJob(self, jobName, pbsFile):
        """
        Submit PBS job   
        """
        os.system('qsub -A %s -N %s -l walltime=%s -l mem=%s -l ncpus=%i -o %s/log pbs/%s' % \
                  (self.group, jobName, self.walltime, self.mem, self.ncpu, self.path, pbsFile))


    def checkPaths(self):
        """
        Check standard paths exists for simulation results    
        """
        logPath = os.path.join(self.path, "log")
        resultsPath = os.path.join(self.path, "results")
        cameraPath = os.path.join(self.path, "camera")
        pbsPath = os.path.join(self.path, "pbs")
        reflPath = os.path.join(self.path, "refl")
        objPath = os.path.join(self.path, "obj")
        if not os.path.exists(resultsPath):
            os.mkdir(resultsPath)
        if not os.path.exists(logPath):
            os.mkdir(logPath)        
        if not os.path.exists(cameraPath):
            os.mkdir(cameraPath)                 
        if not os.path.exists(pbsPath):
            os.mkdir(pbsPath)
        if not os.path.exists(reflPath):
            print "Directory refl does not exist."
            sys.exit()
        if not os.path.exists(self.matlib):
            print "Materials library does not exist."
            sys.exit()
        if not os.path.exists(objPath):
            print "Directory obj does not exist."
            sys.exit()

