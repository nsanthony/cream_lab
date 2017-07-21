#! /home/nsanthony/anaconda3/bin/python


import numpy as np
from math import sqrt
from joblib import Parallel, delayed


#Parallel(n_jobs=2)(delayed(sqrt)(i ** 2) for i in range(10))

#with Parallel(n_jobs=4) as parallel:
#    accumulator = 0.
#    n_iter = 0
#    while accumulator < 10000000000:
#        results = parallel(delayed(sqrt)(accumulator + i ** 2)
#                           for i in range(5))
#        accumulator += sum(results)  # synchronization barrier
#        n_iter += 1
def calcu(i,j):
    
    
    
    
    
#    def trigrate_chip(tree=None,files=None):
#    if files == None:
#        minutes = 10
#    else:
#        minutes = 10*files
#    chips = 40
#    events = len(tree)
#    path = '/home/nsanthony/cream_lab/trigger_rate'
#    os.chdir(path)
#    active_chip = np.zeros([chips,2])
#    chip_list = np.linspace(1,40,num=40)
#    active_chip[:,0] = chip_list #increases if the chip is active in the event
#    print('finding events start')
#    for i in range(0,events):
#        event = tree[i]['cal']
#        for j in range(0,chips):
#            for k in range(0,64):
#                channel = k * (j + 1)
#                if event[channel] != 0:
#                    active_chip[j,1] += 1
#                    break
#                
#    print('fin')
#    time = 60*minutes
#    trigger_rate = active_chip
#    trigger_rate[:,1] = trigger_rate[:,1]/time
#    [trigger_rate,layer2asiic] = fix_asiic(trigger_rate=trigger_rate)
#    return trigger_rate,time,layer2asiic