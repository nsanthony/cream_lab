#! /home/nsanthony/anaconda3/bin/python

from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
#import numpy as np
import matplotlib.pyplot as plt
from trigrate import trigrate_chip
from trigrate import trigrate_channel
import os
import sys
import numpy as np
from joblib import Parallel, delayed
global tree
sys.setrecursionlimit(10000)
def processTree(event=None,active_chip=None,i=None):
    chips = 40
    for j in range(0,chips):
        for k in range(0,64):
            channel = k * (j + 1)
            if event[channel] != 0:
                active_chip[j,1,1] += 1
                break
    for l in range(0,chips):
        active_chip[l,1,i] += active_chip[l,1,i]
    return active_chip     
start = '20160928-000000'
end = '20160929-213000'
yesterday = '20170629-000000'
files = getfilelist(start=yesterday,last=0)
makeroot(files)
tree = complete_tree(file_list=files)
num_files = len(files)
#[trigger_rate,time] = trigrate_chip(tree=tree,files=num_files)

   

#def trigrate_chip(tree=None,files=None):
if files == None:
    minutes = 10
else:
    minutes = 10*files
chips = 40
events = len(tree)
path = '/home/nsanthony/cream_lab/trigger_rate'
os.chdir(path)
active_chip = np.zeros([chips,2,events])
chip_list = np.linspace(1,40,num=40)
active_chip[:,0,:] = chip_list #increases if the chip is active in the event
#print('finding events start')
active_chip = Parallel(n_jobs=4)(delayed(processTree)(event=tree[i]['cal'],active_chip=active_chip,i=i) for i in range(0,events))
print('fin')
#time = 60*minutes
#trigger_rate = active_chip[:,:,0]
#trigger_rate = np.asarray(trigger_rate)
#print(trigger_rate)
#    np.savetxt('data_stuff.txt',trigger_rate)
#    for i in range(0,40):
#        trigger_rate[i,1] = trigger_rate[i,1]/time
#trigger_rate[:,1] = trigger_rate[:,1]/time
#[trigger_rate,layer2asiic] = fix_asiic(trigger_rate=trigger_rate)
#return trigger_rate,time,layer2asiic

#
#chips = 40
#events = len(tree)
#plt.close()
#plt.figure(1)
#plt.barh(trigger_rate[:,0],trigger_rate[:,1])
#plt.ylim(chips+1,0)
#plt.xlim(0,events/time)
#plt.xlabel('Events (Hz)')
#plt.ylabel('Asiic Chip')
#plt.title('Asiic Chip Trigger Rate through event: %s.root'%files[len(files)-1][len(files[0])-20:len(files[0])-5])
#plt.show()
#
#chip = 4
#[trigger_rate,time] = trigrate_channel(tree=tree,chip = chip,files=num_files)
#chips = 40
#events = len(tree)
##plt.close()
#plt.figure(2)
#plt.barh(trigger_rate[:,0],trigger_rate[:,1])
#plt.ylim(trigger_rate[0,0],trigger_rate[63,0])
##plt.xlim(0,events/time)
#plt.xlabel('Events (Hz)')
#plt.ylabel('Channel Number')
#plt.title('Asiic Chip %d Trigger Rate through event: %s.root'%(chip,files[len(files)-1][len(files[0])-20:len(files[0])-5]))
#plt.show()