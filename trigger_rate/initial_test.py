#! /home/nsanthony/anaconda3/bin/python
import os
import sys
path = '/usr/local/root/lib'
sys.path.insert(0, path)
path1 = '/usr/local/root/lib/root'
sys.path.insert(0,path1)
import ROOT
import numpy as np
from root_numpy import root2array, tree2array
from root_numpy.testdata import get_filepath
import matplotlib.pyplot as plt

file_path = '/home/nsanthony/cream/data/dat/2016/09/29'
#file = '20160929-212635.root'
file = '20160929-194244.root'
os.chdir(file_path)

#file_name = get_filepath(file)
#test = root2array(file_name)
rfile = ROOT.TFile(file)
intree = rfile.Get('event')
#intree.Print()
event_tree = tree2array(intree,branches=['evt','time','trig','cal'])
#event1 = event_tree[10]['cal']

chips = int(event_tree[0]['cal'].shape[0]/64)


events = len(event_tree)
active_chip = np.zeros([chips,2])
chip_list = np.linspace(1,40,num=40)
active_chip[:,0] = chip_list
for i in range(0,events):
    event = event_tree[i]['cal']
    for j in range(0,chips):
        for k in range(0,64):
            channel = k * (j + 1)
            if event[channel] != 0:
                active_chip[j,1] += 1
                break
minutes = 10
time = 60*minutes
trigger_rate = active_chip
trigger_rate[:,1] = trigger_rate[:,1]/time
plt.close()




plt.barh(trigger_rate[:,0],trigger_rate[:,1])
plt.ylim(chips+1,0)
plt.xlim(0,events/time)
plt.xlabel('Events (Hz)')
plt.ylabel('Asiic Chip')
plt.title('Asiic Chip Trigger Rate event: %s'%file)
plt.show()


#==============================================================================
# hist = np.zeros([64,chips])
# channel = 0
# 
# for i in range(0,chips):
# 	for j in range(0,64):
# 		hist[j,i] = event1[channel]
# 		channel += 1
# chip_mean = np.zeros([chips,2])
# for i in range(0,chips):
#     chip_mean[i,0] = i + 1
#     chip_mean[i,1] = np.mean(hist[:,i])
#==============================================================================

#this is to plot the data for each chip. NOT USEFUL    
#plt.close()
#plt.barh(chip_mean[:,0],chip_mean[:,1])
#plt.xlim(0,15000)
#plt.show()





