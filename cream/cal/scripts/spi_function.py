#! /Users/nsanthony/miniconda3/bin/python
import matplotlib.pyplot as plt
import numpy as np
#from numpy import genfromtxt
#import os
#import maths
#from customfit import gauss 
from customfit import pygauss

def analysis(det,guess=None,bins=None,sig_cut=None,n=None):
    """Please input detector object. Input guesses for Amplitude, mean, 
    and sig of a gauss, and input number of bins  desired. 
    Guess and bins are not required.
    This outputs xp,yp (fit x and y historgram values), pyfit object
    that containts info of the pyspecfit function, and statspy which are the 
    important gauss stats of amplitude,mean, and sigma and their associated
    errors."""
    
    
    if bins == None:
        bins = 100
    #set up data values to be passed to the pyguass function
    data_set = det.data
    data = det.data[:,2]
    error = det.data[:,3]
    
    #calls the pyguass fuction which uses the pyspeck package 
    [xp,yp,pyfit] = pygauss(data,error,bins,guess=guess)
    
    #this gets the important relavant statistics to be used in analysis
    pyfitA = pyfit.specfit.parinfo['AMPLITUDE0']
    pyfitmu = pyfit.specfit.parinfo['SHIFT0']
    pyfitsig = pyfit.specfit.parinfo['WIDTH0']
    statspy = [pyfitA.value,pyfitmu.value,pyfitsig.value]
    
    #this does an analysis of the data based on a 5 sigma cut
    if sig_cut == None:
        sig_cut = 5
    cut = pyfitsig.value*sig_cut + pyfitmu.value
    print(det.name)
    print('cut:',cut)
    hot_channels = np.zeros((1000,1))
    j = 0
    for i in range(0,len(data)):
        if data_set[i,2] > cut:
            hot_channels[j,0] = data_set[i,0]
            j += 1
    
    print('Total Channels:',len(data))
    print('Hot Channel Count:',j)
    print('Precent of total:',(j/len(data))*100)
    print()
    
    #before terminating this plots the fit and data
    pyfit.specfit.plot_fit()
    plt.show()
    
    return xp,yp,pyfit,statspy,cut,j