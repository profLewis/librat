import numpy as np
import pylab as plt

# build some utility functions 
def split_intersections(intersections):
    """
     split 2-D array intersections
     into list on NaN
    """
    split = np.where(np.isnan(intersections[0]))[0]
    xy = np.array_split(intersections.T,split)
    data = []
    for i in range(1,len(xy)):
        data.append(xy[i][1:].T)
    return(data)

def plotter6(file,file1):
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots()
    ax.axis('equal')
    data=split_intersections(np.loadtxt(file).T)
    objs=np.loadtxt(file1).T

    # plot circles
    radius = 10
    for i in range(objs.shape[1]):
        circle1 = plt.Circle((objs[0,i], objs[1,i]), radius, fill=False)
        ax.add_artist(circle1)

    # plot x, y coordinates
    for i in range(len(data)):
        ax.plot(data[i][0],data[i][1],'+-')
    o=ax.plot(data[0][0,0],data[0][1,0],'ro')
    
