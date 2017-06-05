#! /Users/nsanthony/miniconda3/bin/python
import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import os
#import maths
from customfit import gauss 
from customfit import pygauss

detector = 'cal'

path = '/Users/nsanthony/cream/cal/files'
os.chdir(path)

if detector == 'cal':
    low = genfromtxt('cal_low.data',delimiter=",")
    mid = genfromtxt('cal_mid.data',delimiter=",")
    high = genfromtxt('cal_high.data',delimiter=",")

elif detector == 'scd1':
    scd = genfromtxt('scd1.data',delimiter=",")
elif detector == 'scd2':
    scd = genfromtxt('scd2.data',delimiter=",")
elif detector == 'scd3':
    scd = genfromtxt('scd3.data',delimiter=",")
elif detector == 'scd4':
    scd = genfromtxt('scd4.data',delimiter=",")

bins = 100
data_set = high
data = high[:,2]
error = high[:,3]
#data_set = scd
#data = scd[:,2]
#error = scd[:,3]
#guess = [200,33000,1000]
guess = [70,35000,1000]
#==============================================================================
# [x,y,yfit,stats,stater,p0] = gauss(data,error,bins,guess=guess) 
# 
# 
# plt.figure(1)
# plt.plot(x,y,     label='Data')
# plt.plot(x,yfit, label='Fit')
# plt.legend()
#==============================================================================

#[xp,yp,pyfit] = pygauss(data,error,bins)
[xp,yp,pyfit] = pygauss(data,error,bins,guess=guess)
plt.figure(1)
pyfit.specfit.plot_fit()
plt.show()

pyfitA = pyfit.specfit.parinfo['AMPLITUDE0']
pyfitmu = pyfit.specfit.parinfo['SHIFT0']
pyfitsig = pyfit.specfit.parinfo['WIDTH0']

statspy = [pyfitA.value,pyfitmu.value,pyfitsig.value]
cut = pyfitsig.value*5 + pyfitmu.value
cutl = cut - pyfitsig.value*10
print(detector)
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



    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    