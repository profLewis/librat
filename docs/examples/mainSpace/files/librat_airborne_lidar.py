#!/bin/env python
"""
Use librat and HPC to simulate airborne lidar for the chat01 study site
John Armston (j.armston@uq.edu.au)
"""

import libratcommon
import lidarsurvey

# Get commands line arguments
cmdargs = libratcommon.cmdArgs()

# Setup batch processing on HPC
pbsObj = libratcommon.pbsBatch()
pbsObj.path = cmdargs.path
pbsObj.object = cmdargs.objectFile
pbsObj.wavebands = cmdargs.waveBands
pbsObj.checkPaths()

# Set global parameters
sceneObj = libratcommon.wavefrontFiles(cmdargs.objectFile)
maxH = sceneObj.get_maxH()
pbsObj.nOrder = int(cmdargs.nOrder)
    
# Define camera object
camObj = libratcommon.libratFiles()
camObj.setItem("rpp", int(cmdargs.rpp))
camObj.initAirborneCamera(lidar=True)

# Define light object
lightObj = libratcommon.libratFiles()
lightObj.initAirborneLight(lidar=False)

# Open survey configuration file and Loop through each lidar pulse
surveyObj = open(cmdargs.surveyFile, 'r')
for i,line in enumerate(surveyObj):

    # Initialise files
    jobName = '_'.join([cmdargs.lidarSim, str(i)])
    pbsFile = jobName + '.sh'
    cameraFile = jobName + '.camera'
    lightFile = jobName + '.light'
    
    # Read pulse attributes
    pulse = lidarsurvey.readAirborneSurveyLine(line)
        
    # Create light file
    lightObj.calcAirborneGeometry(azimuth=pulse["Azimuth"], zenith=pulse["Zenith"], altitude=pulse["Altitude"], footprint=pulse["fSizeAlong"])
    lightObj.writeFile(fileName=cameraFile)
    
    # Create camera file
    camObj.calcAirborneGeometry(azimuth=pulse["Azimuth"], zenith=pulse["Zenith"], altitude=pulse["Altitude"], footprint=pulse["fSizeAlong"])
    camObj.calcAirborneLidar(maxH, delta=5, binStep=0.299792458)
    camObj.writeFile(fileName=lightFile)

    # Create PBS file and send the job to the HPC
    pbsObj.writeJob(pbsFile, cameraFile, lightFile)
    pbsObj.submitJob(jobName, pbsFile)

# Close survey configuration file
print '%i jobs submitted' % (i+1)
surveyObj.close()
