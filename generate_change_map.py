'''
Created on Jan 19, 2016

@author: jackzhang, Zexi Chen
'''
#import gdal
import numpy
import matplotlib.pyplot as plt 


filename = "/home/jay1204/Documents/ChangeDetectionParallel_version1/s_2001_2006_out_parallel_sample_1000_dtw.txt"

#src_ds = gdal.Open(filename)
# myArray  = numpy.loadtxt(filename, skiprows=7)
myArray  = numpy.loadtxt(filename)
print myArray
myArray[myArray==7]=None
cmap = plt.get_cmap("Greys")
cmap.set_bad('r',1.)
plt.imshow(myArray,vmin=0, vmax=7,cmap=cmap)
figure = plt.gcf()
# figure.set_size_inches(6,4.5)
plt.colorbar()
figure.set_size_inches(48,24)
figure.savefig('/home/jay1204/Documents/ChangeDetectionParallel/possibleChangeMap_legend_sample_1000_dtw.png',dpi=60)
