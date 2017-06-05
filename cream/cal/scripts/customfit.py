#! /Users/nsanthony/miniconda3/bin/python
#import matplotlib.pyplot as plt
import numpy as np
#from numpy import genfromtxt
#import os
from scipy.optimize import curve_fit
import math
import pyspeckit


def value_estimates(x,y):
    """This estimates amplitude,average, and sigma for a gauss. 
    Input x & y data"""
    ##estimate of important values
    flux = y
    velz = x
    
    imax = flux.argmax()
    #print("Max at %d: %g %g" % (imax, velz[imax], flux[imax]))
    A = imax #amplitude estimate
    A = float(A)
    nn = 3      # pick a few nearest neighbors
    flux1 = flux[imax-nn:imax+nn]
    velz1 = velz[imax-nn:imax+nn]
    
    tmp1 = flux1*velz1 
    tmp2 = flux1*velz1*velz1
    zmean1 = tmp1.sum()/flux1.sum()
    zdisp1 = tmp2.sum()/flux1.sum() - zmean1*zmean1
    
    print("mean,var:",zmean1,zdisp1)
    mu = zmean1 #mean value estimate
    if zdisp1 > 0:
        sigma1 = math.sqrt(zdisp1)
    #    print("sigma,FWHM:",sigma1,sigma1*2.355)
    sig = sigma1
    return A,mu,sig
def gau(x, *p):
    A,mu,sigma = p
    return A*np.exp(-(x-mu)**2/(2.*sigma*2))


def pygauss(data,error,bins,guess=None):
    """Input data,error, and desired bins. Outputs x, y, and pyspeckit fit for 
    a gaussian curve (this is an object with all the values you could want"""
    [y,x] = np.histogram(data,bins,weights=error)
    x = x[:len(y)]
    extra = np.zeros(1000-len(x))
#    print(len(extra),len(x))
    x0 = x
    y0 = y
    x = np.append(x,extra)
    y = np.append(y,extra)
    error = np.append(error,extra)
#    print(len(x),len(y))
    #checks to see if there is a guess argument passed. If not it uses
    #value_estimate function ot find estimates for sig,mu,A
    if guess == None:
        [A,mu,sig] = value_estimates(x,y)
        p0 = [A,mu,sig]
    else:
        p0 = guess
     
    sp = pyspeckit.Spectrum(data=y, error=error,xarr=x,
                         xaarkwargs={'units':'Volts'},
                         unit='Counts')
    sp.plotter()
    sp.specfit(fittype='gaussian', guesses=p0)
    
    sp.plotter(errstyle='fill')
    stuff= sp
#    sp.specfit.plot_fit()
    
    return x0,y0,stuff

def gauss(data,error,bins,guess=None):
    """ Please input data, error, and bins. Outputs x,y,yfit,coeff,cm 
    coeff are statitstics amplitude,mean,sig,and FWHM"""
    # if len(p) != 3: raise ValueError("Error, found %d, (%s), need 3" % (len(p),str(p)))
    [y,x] = np.histogram(data,bins,weights=error)
    x = x[:len(y)]
    #checks to see if there is a guess argument passed. If not it uses
    #value_estimate function ot find estimates for sig,mu,A
    if guess == None:
        [A,mu,sig] = value_estimates(x,y)
        p0 = [A,mu,sig]
    else:
        p0 = guess
    
    
    coeff, cm = curve_fit(gau, x, y, p0=p0)
    flux_fit = gau(x, *coeff)
    
    print("Fitted amp            :",coeff[0])
    print("Fitted mean           :",coeff[1])
    print("Fitted sigma and FWHM :",coeff[2], coeff[2]*2.355)
    print("Covariance Matrix     :\n",cm)
    # what are now the errors in the fitted values?
    print("error amp:",math.sqrt(cm[0][0]))
    print("error mean:",math.sqrt(cm[1][1]))
    print("error sigma:",math.sqrt(cm[2][2]))
    
    return x,y,flux_fit,coeff,cm,p0



    
    
    
    
    
    
    
    
    
    
    
    