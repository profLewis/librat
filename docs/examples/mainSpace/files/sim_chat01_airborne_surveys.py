#!/bin/env python
"""
Simulate airborne survey configurations for the chat01 study site
John Armston (j.armston@uq.edu.au)
"""

from numpy import *
import lidarsurvey


# CHAT01 field centre locations
sites = ['chat0101','chat0102','chat0103']
xCen = [355889.44,358647.70,353853.48]
yCen = [7787401.90,7790208.68,7785360.50]
width = [50.0,50.0,50.0]
  
# CHAT01 acquisition configurations
acquisitions = ['0550_1','0550_2','0550_3','1000_1','1000_2','1000_3','1300_1','1300_2','1300_3']
fHeight = [550.0,550.0,550.0,1000.0,1000.0,1000.0,1300.0,1300.0,1300.0]
sRate = [0.113,0.113,0.113,0.08,0.08,0.08,0.063,0.063,0.063]
pRate = [200.0,200.0,200.0,200.0,200.0,200.0,150.0,150.0,150.0]
xStart = [352814.0,352973.0,353132.0,352683.0,352973.0,353263.0,352596.0,352973.0,353350.0]
yStart = [7784697.0,7784534.0,7784371.0,7784830.0,7784534.0,7784238.0,7784919.0,7784534.0,7784149.0]
xEnd = [364044.0,364203.0,364362.0,363913.0,364203.0,364493.0,363826.0,364203.0,364580.0]
yEnd = [7795704.0,7795541.0,7795378.0,7795837.0,7795541.0,7795245.0,7795926.0,7795541.0,7795156.0]

# Init survey object
surveyObj = lidarsurvey.airborneConfig()
surveyObj.initConfig()
surveyObj.setItem("fSpeed", 0.055)
surveyObj.setItem("FOV", 60.0)
surveyObj.setItem("bDivergence", 0.5)


# Run the simulation for the flight path over each field site
for j,site in enumerate(sites):
        
    # Process each flight path
    for i,survey in enumerate(acquisitions):
        
        # Simulation name
        name = sites[j] + '_' + acquisitions[i]
        outObj = open(name + '.csv', 'w')
      
        # Set flight path atttributes
        xdiff = array(abs(xEnd[i] - xStart[i]))
        ydiff = array(abs(yEnd[i] - yStart[i]))
        surveyObj.setItem("fHeading", degrees(arctan(xdiff/ydiff)))
        surveyObj.setItem("fHeight", fHeight[i])
        surveyObj.setItem("sRate", sRate[i])
        surveyObj.setItem("pRate", pRate[i]) 
    
        # Set field site attributes
        xLower = xCen[j] - width[j] * sin(radians(surveyObj.items["fHeading"]))
        yLower = yCen[j] - width[j] * cos(radians(surveyObj.items["fHeading"]))
        plint,side = lidarsurvey.planeInt([xStart[i],yStart[i]], [xEnd[i],yEnd[i]], 1, [xLower,yLower])
        surveyObj.setItem("xStart", plint[0])
        surveyObj.setItem("yStart", plint[1])
        surveyObj.setItem("time", width[j] * 2.0 / 1000.0 / surveyObj.items["fSpeed"])

        # Update configuration
        surveyObj.calcConfig()
        surveyObj.writeConfig(name + '.config')

        # For each scan line
        surveyObj.calcConfig()
        for scanLine in range(surveyObj.items["nLines"]):
            
            # Simulate
            azimuth,zenith,prange,x,y,fSizeAcross,fSizeAlong = surveyObj.predScanLine()
            withinSite = logical_and(x < (xCen[j] + width[j]/2.0), logical_and(x > (xCen[j] - width[j]/2.0), \
                         logical_and(y < (yCen[j] + width[j]/2.0), y > (yCen[j] - width[j]/2.0))))
            index = where(withinSite)

            # Write to file
            if len(index) > 0:
                for p in index[0]:
                    outList = [surveyObj.items["fHeight"],azimuth[p],zenith[p],prange[p],x[p]-xCen[j],y[p]-yCen[j],fSizeAcross[p],fSizeAlong[p]]
                    outStr = ','.join([str(element) for element in outList])
                    outObj.write(outStr + '\n')
        
        # Close output file
        surveyObj.scanLine = 0
        outObj.close()


