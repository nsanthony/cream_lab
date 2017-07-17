#! /home/nsanthony/anaconda3/bin/python
import numpy as np
import os

def trigrate_chip(tree=None,files=None):
    if files == None:
        minutes = 10
    else:
        minutes = 10*files
    chips = 40
    events = len(tree)
    active_chip = np.zeros([chips,2])
    chip_list = np.linspace(1,40,num=40)
    active_chip[:,0] = chip_list #increases if the chip is active in the event
    for i in range(0,events):
        event = tree[i]['cal']
        for j in range(0,chips):
            for k in range(0,64):
                channel = k * (j + 1)
                if event[channel] != 0:
                    active_chip[j,1] += 1
                    break
    time = 60*minutes
    trigger_rate = active_chip
    trigger_rate[:,1] = trigger_rate[:,1]/time
    return trigger_rate,time
    
    
def trigrate_channel(tree=None,chip=None,files=None):
    if files == None:
        minutes = 10
    else:
        minutes = 10*files
    path = '/home/nsanthony/cream_lab/trigger_rate'
    os.chdir(path)
    channels = np.loadtxt('channel_list.txt',dtype=int)
    start_channel = channels[chip-1,0]
    end_channel = channels[chip-1,63]
    events = len(tree)
    active_channel = np.zeros([64,2])
    active_channel[:,0] = np.linspace(start_channel,end_channel,num=64)
    for i in range(0,events):
        event = tree[i]['cal']
        for j in range(0,64):
            channel = start_channel + j
            if event[channel] > 0:
                active_channel[j,1] += 1
                break
            
    time = 10*minutes
    trigger_rate = active_channel
    trigger_rate[:,1] = trigger_rate[:,1]/time
    return trigger_rate,time
