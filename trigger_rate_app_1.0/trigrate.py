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
    path = '/home/nsanthony/cream_lab/trigger_rate'
    os.chdir(path)
    active_chip = np.zeros([chips,2])
    chip_list = np.linspace(1,40,num=40)
    active_chip[:,0] = chip_list #increases if the chip is active in the event
    print()
    print('Finding Events')
    for i in range(0,events):
        event = tree[i]['cal']
        for j in range(0,chips):
            for k in range(0,64):
                channel = k * (j + 1)
                if event[channel] != 0:
                    active_chip[j,1] += 1
                    break
    
    print()            
    print('Fin')
    time = 60*minutes
    trigger_rate = active_chip
    trigger_rate[:,1] = trigger_rate[:,1]/time
    [trigger_rate,layer2asiic] = fix_asiic(trigger_rate=trigger_rate)
    return trigger_rate,time,layer2asiic
    
    
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
            channel = start_channel + j - 1
            if event[channel] > 0:
                active_channel[j,1] += 1
                break
            
    time = 10*minutes
    trigger_rate = active_channel
    trigger_rate[:,1] = trigger_rate[:,1]/time
    return trigger_rate,time

def fix_asiic(trigger_rate=None):
    layer2asiic = np.loadtxt('layer2asiic.txt',dtype=int)
    trig_rate_asiic = np.zeros((40,2))
    for i in range(0,40):
        trig_rate_asiic[i,0] = i
        trig_rate_asiic[i,1] = trigger_rate[int(layer2asiic[i,1])-1,1]
#        trig_rate_asiic[i,2] = trigger_rate[int(layer2asiic[i,2])-1,1]
    return trig_rate_asiic,layer2asiic



#attempt at parallel processing 
#==============================================================================
# from joblib import Parallel, delayed
# def processTree(event=None,active_chip=None):
#     chips = 40
#     for j in range(0,chips):
#         for k in range(0,64):
#             channel = k * (j + 1)
#             if event[channel] != 0:
#                 active_chip[j,1] += 1
#                 break
#     return active_chip        
# 
# def trigrate_chip(tree=None,files=None):
#     if files == None:
#         minutes = 10
#     else:
#         minutes = 10*files
#     chips = 40
#     events = len(tree)
#     path = '/home/nsanthony/cream_lab/trigger_rate'
#     os.chdir(path)
#     active_chip = np.zeros([chips,2])
#     chip_list = np.linspace(1,40,num=40)
#     active_chip[:,0] = chip_list #increases if the chip is active in the event
#     print('finding events start')
#     active_chip = Parallel(n_jobs=4)(delayed(processTree)(event=tree[i]['cal'],active_chip=active_chip) for i in range(0,events))
#     print('fin')
#     time = 60*minutes
#     trigger_rate = active_chip
#     trigger_rate = np.asarray(trigger_rate)
#     print(trigger_rate)
#     
# #    np.savetxt('data_stuff.txt',trigger_rate)
# #    for i in range(0,40):
# #        trigger_rate[i,1] = trigger_rate[i,1]/time
#     trigger_rate[:,1] = trigger_rate[:,1]/time
#     [trigger_rate,layer2asiic] = fix_asiic(trigger_rate=trigger_rate)
#     return trigger_rate,time,layer2asiic
#==============================================================================
