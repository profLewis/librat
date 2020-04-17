#!/bin/env python
"""
General purpose numerical utilities I could not find in existing python libraries
Currently includes some statistical, geometry, and miscellaneous functions
John Armston (j.armston@uq.edu.au)
"""

from numpy import *

    
def defineNeymanA(nPoints=100, m2=1, nQuad=25):
    """
    m = mean number of points in a quadrat
    m1 = mean number of cluster per quadrat
    m2 = cluster mean size (number of points per group), i.e. clumping factor
    nQuad = number of quadrats in the scene
    Output is a tuple of the number of points per quadrat and the corresponding probability
    """
    maxPoints = math.ceil(sqrt(nPoints) * 4)
    points = arange(maxPoints)
    m = nPoints / nQuad
    m1 =  m / m2
    maxGroups = math.ceil(m1 * 4)
    prob = zeros(maxPoints)
    for point in points:
        clump = zeros(maxGroups)
        for group in arange(maxGroups):
            clump[group] = ((m1 * exp(-m2))**group / math.factorial(group)) * (group**point)
        prob[tree] = (exp(-m1) * ((m2**point) / math.factorial(point))) * sum(clump)
    return (points, prob)      
    
    
def cartesian2spherical(x,y,z):
    """
    Converts cartesian to spherical coordinates
    """
    r = sqrt(x**2 + y**2 + z**2)
    index = (r > 0)
    phi = where(index, arctan(y / x), 0)
    theta = where(index, arctan(z / sqrt(x**2 + y**2)), 0)
    return (phi,theta,r)


def spherical2cartesian(phi,theta,r):
    """
    Converts spherical to cartesian coordinates
    """
    x = r * cos(phi) * cos(theta)
    y = r * sin(phi) * cos(theta)
    z = r * sin(theta)
    return (x,y,z)    


def calcHeronArea(x, y, z):
    """
    Calculates area of a 3D triangle using Herons method
    """
    dims = x.shape
    if (len(dims) == 1):
        x = x.reshape((1,dims[0]))
        y = y.reshape((1,dims[0]))
        z = z.reshape((1,dims[0]))
    a = sqrt((x[:,1]-x[:,0])**2 + (y[:,1]-y[:,0])**2 + (z[:,1]-z[:,0])**2)
    b = sqrt((x[:,2]-x[:,0])**2 + (y[:,2]-y[:,0])**2 + (z[:,2]-z[:,0])**2)
    c = sqrt((x[:,2]-x[:,1])**2 + (y[:,2]-y[:,1])**2 + (z[:,2]-z[:,1])**2)
    s = (a + b + c) / 2.0
    area = sqrt(s*(s-a)*(s-b)*(s-c))
    return area


def calc3DPolyArea(fx=None,fy=None,fz=None):
    """
    Calculates area of a 3D polygon
    Leave fx,fy or fz as None to optionally project to 2D along a specified axis
    """     
    if (fx is None):
        n = len(fy)
        fx = repeat(0, n)
    elif (fy is None):
        n = len(fx)
        fy = repeat(0, n)    
    elif (fz is None):
        n = len(fx)
        fz = repeat(0, n)
    else:
        n = len(fx)
    area = 0.0
    for i in range(n):
        j = (i + 1) % n
        a = array([fx[i],fy[i],fz[i]])
        b = array([fx[j],fy[j],fz[j]])
        area += cross(a,b)
    return sqrt(sum((area/2.0)**2))


def convex_hull(x,y):
    """
    Computes the convex hull of a set of 2D points. 
    Input: an iterable sequence of (x, y) pairs representing the points.
    Output: a list of vertices of the convex hull in counter-clockwise order,
      starting from the vertex with the lexicographically smallest coordinates.
    Implements Andrew's monotone chain algorithm. O(n log n) complexity.
    http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain#Python
    """
    # Convert x and y to an interable list of tuples
    points = [(x[i],y[i]) for i in range(len(x))]
     
    # Sort the points lexicographically (tuples are compared lexicographically).
    # Remove duplicates to detect the case we have just one unique point.
    points = sorted(set(points))
 
    # Boring case: no points or a single point, possibly repeated multiple times.
    if len(points) <= 1:
        return points
 
    # 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
    # Returns a positive value, if OAB makes a counter-clockwise turn,
    # negative for clockwise turn, and zero if the points are collinear.
    def cross(o, a, b):
        return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])
 
    # Build lower hull 
    lower = []
    for p in points:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)
 
    # Build upper hull
    upper = []
    for p in reversed(points):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)
 
    # Concatenation of the lower and upper hulls gives the convex hull.
    # Last point of each list is omitted because it is repeated at the beginning of the other list. 
    chull = lower[:-1] + upper[:-1]
    x = array([i[0] for i in chull])
    y = array([i[1] for i in chull])
    return array([x,y]).transpose()


def rotate2D(xt, yt, angle):
    """
    Rotates x,y points by the given angle in degrees
    Translation to origin assumed
    """
    rad = angle * pi / 180.0
    xr = xt * cos(rad) - yt * sin(rad)
    yr = yt * cos(rad) + xt * sin(rad)
    return (xr, yr)
  
    
def planeInt(lp1, lp2, n, pp):
    """"
    Returns the intersection point of a plane and a line
    Inputs:
       lp1, lp2 = two points on the line. lp1 is the 0 point, + toward lp2.
       n = normal to plane
       pp = any point on the plane
    Output:
       pi = intersection point.
    All points are 3 element arrays (x,y,z)
    """
    lp1 = array(lp1)
    lp2 = array(lp2)
    pp = array(pp)
    d = lp2 - lp1
    w = lp1 - pp
    v1 = sum(w*n)
    v2 = sum(d*n)
    t = -v1 / v2
    plint = lp1 + t * d
    return plint


def counterClockwiseSort(xyz):
    """
    Normalise coordinates into [0, 2pi] space and returns sort index
    This function can be used to sort polygon coordinates "counter-clockwise"
    Only does a 2D sort, so takes the two axes with greatest difference.
    """
    index = argsort(amax(xyz,axis=0)-amin(xyz,axis=0))
    xy = xyz[:,index[0:2]]
    return argsort((arctan(xy[:,1] - mean(xy[:,1]), xy[:,0] - mean(xy[:,0])) + 2.0 * pi) % 2.0 * pi)


def calcFaceNormal(fx,fy,fz):
    """
    Calculate the face normal.
    For flat leaves, this is equivalent to the vertex normals OnyxTree gives.
    """
    # cross product   
    vnx = (fz[:,2]-fz[:,0])*(fy[:,1]-fy[:,0])-(fy[:,2]-fy[:,0])*(fz[:,1]-fz[:,0])
    vny = (fx[:,2]-fx[:,0])*(fz[:,1]-fz[:,0])-(fz[:,2]-fz[:,0])*(fx[:,1]-fx[:,0])
    vnz = (fy[:,2]-fy[:,0])*(fx[:,1]-fx[:,0])-(fx[:,2]-fx[:,0])*(fy[:,1]-fy[:,0])
    # normalise
    magnitude = sqrt(vnx**2 + vny**2 + vnz**2)
    return (-vnx/magnitude,-vny/magnitude,-vnz/magnitude)


def rotate3D(xv, yv, zv, xt, yt, zt, angle):
    """
    Return points rotated about an arbitrary axis in 3D.
    Positive angles are counter-clockwise looking down the axis toward the origin.
    Right handed coordinate system and translation to origin assumed.
    Input angle is in degrees.
    Adapted from http://paulbourke.net/geometry/rotate/ 
    """
    # Get rotation axis unit vector
    nm = sqrt(xv**2 + yv**2 + zv**2)
    xn = xv/nm
    yn = yv/nm
    zn = zv/nm

    # Get common trig factors     
    c = cos(angle * pi / 180.0)
    t = (1 - cos(angle * pi / 180.0))
    s = sin(angle * pi / 180.0)

    # Setup rotation matrix
    m11 = t*xn**2 + c
    m12 = t*xn*yn - s*zn
    m13 = t*xn*zn + s*yn
    m21 = t*xn*yn + s*zn
    m22 = t*yn**2 + c
    m23 = t*yn*zn - s*xn
    m31 = t*xn*zn - s*yn
    m32 = t*yn*zn + s*xn
    m33 = t*zn**2 + c

    # Perform rotation
    xr = m11*xt + m12*yt + m13*zt
    yr = m21*xt + m22*yt + m23*zt
    zr = m31*xt + m32*yt + m33*zt

    # Return rotated points    
    return (xr,yr,zr)
