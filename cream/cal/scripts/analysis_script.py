#! /Users/nsanthony/miniconda3/bin/python
from spi_function import analysis
from data_acquisition import acquisition
import numpy as np
det = acquisition(detector='cal',layer='low',date='20160929',time='172216') 

guess = [5000,5000,500]
[scd1x,scd1y,scd1py,scd1stat,scd1cut,scd1num] = analysis(det=det,guess=None,bins=200)
d = det.data
#[scd2x,scd2y,scd2py,scd2stat,scd2cut,scd2num] = analysis(det=det,guess=guess,n=2)
#[scd3x,scd3y,scd3py,scd3stat,scd3cut,scd3num] = analysis(detector='scd3',guess=None,n=3)
#[scd4x,scd4y,scd4py,scd4stat,scd4cut,scd4num] = analysis(detector='scd4',guess=None,n=4)
#[lowx,lowy,lowpy,lowstat,lowcut,lownum] = analysis(det=det,bins=200)
#[midx,midy,midpy,midstat,midcut,midnum] = analysis(detector='cal_mid',bins=200,n=6)


#still need to fix the fit for cal_high, it is throwing error in the pyspec fuction
#in the custom_fit fuction. Parameter becomes NaN when it should not. May be 
#general but in code. 
#[highx,highy,highpy,highstat,highcut,highnum] = analysis(detector='cal_high',bins=100)


##this looks for channels that should be removed based on the RMS is 0 and 
##looks for channels listed as nan
c = 0
channels = np.zeros((1000,6)) #channels to be removed
for i in range(0,len(det.data[:,3])):
    if det.data[i,5] == 0:
        if np.isnan(det.data[i,0]) == False: 
            channels[c,:] = det.data[i,:]
            c += 1
            last = i

##removes those channels from the data set
dimentions = det.data.shape
dimentions = np.asarray(dimentions)
kept_data = np.zeros((dimentions[0]-c,dimentions[1]))
k = 0
l = 0
for i in range(0,dimentions[0]):
    if det.data[i,0] == channels[k,0]:
        k += 1
        continue
    elif np.isnan(det.data[i,0]) == False:
        kept_data[l,:] = det.data[i,:]
        l += 1

##gets the fit for the data that was just corrected for
det.data = kept_data
[scd2x,scd2y,scd2py,scd2stat,scd2cut,scd2num] = analysis(det=det,guess=None,bins=200)

#finds the hot channels so that we can look at them more closely
hot_channels = np.zeros((scd2num,dimentions[1]))
hot = 0
for i in range(0,len(det.data)):
    if det.data[i,2] > scd2cut:
#        print('got one:',i+1)
        hot_channels[hot,:] = det.data[i,:]
        hot += 1
        

#==============================================================================
# out = 0
# questions = np.zeros((644,6))
# for i in range(0,dimentions[0]-c):
#     if det.data[i,2] >= 30000:
#         questions[out,:] = det.data[i,:]
#         out += 1
#==============================================================================
