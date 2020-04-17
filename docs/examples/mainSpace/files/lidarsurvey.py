#!/bin/env python
"""
Routines for simulating airborne and terrestrial lidar survey configurations 
John Armston (j.armston@uq.edu.au)
"""

from numpy import *
import utilities


class airborneConfig:
    """
    A class for airborne surveys
    Derived from Baltsavias (1999) for rotating prism scanning lidar (i.e. Riegl LMS sensors)
    The idea is to handle different scan patterns as well, so needs a little more work
    """
    
    def __init__(self):     
        self.items = {}
        self.scanLine = 0

    def initConfig(self):
        """
        Initialise airborne configuration
        """
        self.items["xStart"] = 0.0 # m
        self.items["yStart"] = 0.0 # m
        self.items["time"] = 5.0 # sec
        self.items["fHeight"] = 1000.0 # m
        self.items["fHeading"] = 45.0 # deg
        self.items["sRate"] = 0.080 # kHz
        self.items["pRate"] = 200.0 # kHz
        self.items["fSpeed"] = 0.055 # km/s
        self.items["FOV"] = 60.0 # deg
        self.items["bDivergence"] = 0.5 # mrad
        self.items["angleStep"] = 0.036 # deg
        self.items["laserAperture"] = 0.0

    def calcConfig(self):
        """
        Calculate airborne configuration from initial parameters
        """
        self.items["nPulses"] = int(self.items["pRate"] / self.items["sRate"])
        self.items["lineSpacing"] = self.items["fSpeed"] / self.items["sRate"]
        self.items["nLines"] = int(self.items["time"] * self.items["fSpeed"] * 1e3 / self.items["lineSpacing"])
        self.items["swathWidth"] = 2.0 * self.items["fHeight"] * tan(radians(self.items["FOV"]/2.0))
        self.items["minAngleStep"] = 0.8 / self.items["pRate"] # deg; Riegl LMS-Q680 angle step resolution = 0.0025
        self.items["angleSpeed"] = self.items["sRate"] / radians(self.items["angleStep"]) # mrad/s
        #self.items["acrossPS"] = self.items["fHeight"] / (cos(radians(self.items["FOV"]/2.0)))^2 * (self.items["angleSpeed"]/self.items["pRate"])
        self.items["acrossPS"] = self.items["swathWidth"] / self.items["nPulses"]
        self.items["footprintSize"] = self.items["laserAperture"] + 2.0 * self.items["fHeight"] * tan((self.items["bDivergence"]/1000.0)/2.0)
        self.items["scanArea"] = self.items["swathWidth"] * (self.items["fSpeed"] * 1000.0 * self.items["time"])

    def predScanLine(self):        
        """
        Predict scanline geometry from airborne configuration
        """
        # Calculate spherical coordinates       
        zenith = -(self.items["FOV"]/2.0) + arange(self.items["nPulses"]) * self.items["angleStep"]
        if bool(self.scanLine & 1): zenith = -zenith[::-1]
        azimuth = tile(self.items["fHeading"], self.items["nPulses"])
        azimuth = where(zenith < 0,azimuth + 270.0, azimuth + 90.0) % 360.0
        prange = self.items["fHeight"] / cos(abs(radians(zenith)))    
        # Convert to cartesian coordinates
        longitude = -(azimuth - 90.0)
        longitude = radians(where(longitude <= -180.0, longitude + 360.0, longitude))
        latitude = radians(90.0 - abs(zenith))
        x = prange * cos(longitude) * cos(latitude) + self.items["xStart"] + self.scanLine * self.items["lineSpacing"] * sin(radians(self.items["fHeading"]))
        y = prange * sin(longitude) * cos(latitude) + self.items["yStart"] + self.scanLine * self.items["lineSpacing"] * cos(radians(self.items["fHeading"]))
        # Get instantaneous pulse properties
        a = cos(abs(radians(zenith))) + sin(abs(radians(zenith))) * tan(abs(radians(zenith)) + (self.items["bDivergence"] / 1000.0))
        fSizeAcross = (a * 2.0 * self.items["fHeight"] * sin(self.items["bDivergence"] / 1000.0 / 2.0)) / cos(abs(radians(zenith)) - (self.items["bDivergence"] / 1000.0 / 2.0))
        fSizeAlong = self.items["fHeight"] / (0.5 + 0.5 * cos(2.0 * abs(radians(zenith)))) * (self.items["bDivergence"] / 1000.0) 
        # Return data
        self.scanLine += 1
        return azimuth,zenith,prange,x,y,fSizeAcross,fSizeAlong

    def writeConfig(self, fileName):
        """
        Write airborne configuration to file
        """
        obj = open(fileName, 'w')
        for item in self.items:
            obj.write('%s = %s\n' % (item, self.items[item]))
        obj.close()



class terrestrialConfig:
    """
    A class for terrestrial surveys
    Very basic - written with Leica ScanStation-II settings in mind
    With real data the azimuth angle along a scan line can change +/- 1 deg    
    """
        
    def __init__(self):     
        self.items = {}
        self.scanLine = 0
        
    def initConfig(self):
        """
        Initialise terrestrial lidar configuration
        """
        self.items["spacing"] = 0.03 # m
        self.items["distance"] = 25.0 # m
        self.items["zFOV"] = 270.0 # deg
        self.items["aFOV"] = 360.0 # deg
        self.items["zenithStart"] = -45.0 # deg
        self.items["bDivergence"] = 0.3 # mrad
        self.items["laserAperture"] = 0.0
        self.items["maxRange"] = 300.0
      
    def calcConfig(self):
        """
        Calculate terrestrial lidar configuration from initial parameters
        """
        self.items["angleStep"] = sin((self.items["spacing"] / 2.0) / self.items["distance"]) * 2.0 # rad
        self.items["nLines"] = int(radians(self.items["aFOV"]) / self.items["angleStep"])
        self.items["nPulses"] = int(radians(self.items["zFOV"] / 2.0) / self.items["angleStep"])
    
    def predScanLine(self):
        """
        Predict scanline from terrestrial lidar configuration
        """
        # Determine spherical coordinates       
        zenith = radians(self.items["zenithStart"]) + arange(self.items["nPulses"]) * self.items["angleStep"]
        azimuth = tile(self.items["angleStep"] * self.scanLine, self.items["nPulses"])
        # Get instantaneous pulse properties
        a = cos(abs(radians(zenith))) + sin(abs(radians(zenith))) * tan(abs(radians(zenith)) + (self.items["bDivergence"] / 1000.0))
        fSizeAcross = (a * 2.0 * self.items["distance"] * sin(self.items["bDivergence"] / 1000.0 / 2.0)) / cos(abs(radians(zenith)) - (self.items["bDivergence"] / 1000.0 / 2.0))
        fSizeAlong = self.items["distance"] / (0.5 + 0.5 * cos(2.0 * abs(radians(zenith)))) * (self.items["bDivergence"] / 1000.0)
        # Return data
        self.scanLine += 1
        return azimuth,zenith,fSizeAcross,fSizeAlong
     
    def writeConfig(self, fileName):
        """
        Write terrestrial configuration to file
        """
        obj = open(fileName, 'w')
        for item in self.items:
            obj.write('%s = %s\n' % (item, self.items[item]))
        obj.close()


class surveyFiles:
    """
    A class for reading survey files   
    """

    def readAirborneSurveyLine(line):
        """
        Parse a line from a standard airborne survey file
        """
        lparts = line.strip('\r\n').split(',')
        dictn = {"Altitude" : float(lparts[0]), \
                 "Azimuth" : float(lparts[1]), \
                 "Zenith" : float(lparts[2]), \
                 "Range" :float(lparts[3]) , \
                 "X" : float(lparts[4]), \
                 "Y" : float(lparts[5]), \
                 "fSizeAcross" : float(lparts[6]), \
                 "fSizeAlong" : float(lparts[7])}
        return dictn

    def readTerrestrialSurveyLine(line):
        """
        Parse a line from a standard terrestrial survey file
        """
        lparts = line.strip('\r\n').split(',')
        dictn = {"Azimuth" : float(lparts[0]), \
                 "Zenith" : float(lparts[1]), \
                 "Range" :float(lparts[2]) , \
                 "X" : float(lparts[3]), \
                 "Y" : float(lparts[4]), \
                 "Z" : float(lparts[5]), \
                 "fSizeAcross" : float(lparts[6]), \
                 "fSizeAlong" : float(lparts[7])}
        return dictn
