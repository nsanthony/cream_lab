#! /Users/nsanthony/miniconda3/bin/python

import os
import csv
import numpy as np

path = "/Users/nsanthony/cream"
os.chdir(path)

file = 'HPDMapping.csv'
with open(file, newline='') as f:
	reader = csv.reader(f, delimiter=',')
	mapp = list(reader) #creates channel map list
    

cha = np.zeros((2,len(mapp)))
temp = np.array([0,0])
#create temp list
for i in range(1,len(mapp)):
    
    if mapp[i][1] != "N/A":
        asic = int(mapp[i][1]) #asic chip number
        asiccha = int(mapp[i][2]) #asic channel number
        channel = ((asic -1 ) * 32) + asiccha #channel number
        channel = np.asarray(channel)
        temp[0] = channel
        cha[0,i-1] = temp[0]
#        print(cha[:,i-1])
    else:
        temp = np.array([-1,-1])
        cha[:,i-1] = temp #dead channel
        continue
    
    if "L-" in mapp[i][3]:
        temp[1] = 1
        cha[:,i-1] = temp #low channel
    elif "M-" in mapp[i][3]:
        temp[1] = 2
        cha[:,i-1] = temp #mid channel
    elif "H-" in mapp[i][3]:
        temp[1] = 3
        cha[:,i-1] = temp #high channel
    elif "Empty" in mapp[i][3]:
        print('found empty:',channel)
        temp[1] = 0
        cha[:,i-1] = temp #empty channel
    elif "LED" in mapp[i][3]:
        temp[1] = 4
        cha[:,i-1] = temp #LED channel
    else:
        print('Something didnt work')
        print(cha[:,i-1])


path = '/Users/nsanthony/cream/cal/files/'
os.chdir(path)
np.savetxt('channel_list.csv',cha,delimiter=",")