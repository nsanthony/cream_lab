#! /home/nsanthony/anaconda3/bin/python
import numpy as np

layer2asiic = np.zeros((40,2))
set = [[6,16,7,17,8,18,9,19,10,20,5,15,4,14,3,13,2,12,1,11],[26,36,27,37,28,38,29,39,30,40,25,35,24,34,23,33,22,32,21,31]]
for i in range(0,40,2):
    layer2asiic[i,0] = i/2
    layer2asiic[i+1,0] = i/2
    layer2asiic[i,1] = set[0][int(i/2)]
    layer2asiic[i+1,1] = set[1][int(i/2)]
np.savetxt('layer2asiic.txt',layer2asiic,fmt='%i')

#trigger_rate = np.loadtxt('trig_test.txt')
#layer2asiic = np.loadtxt('layer2asiic.txt')
#trig_rate_asiic = np.zeros((20,4))
#for i in range(0,20):
#    trig_rate_asiic[i,0] = i
#    trig_rate_asiic[i,1] = trigger_rate[int(layer2asiic[i,1])-1,1]
#    trig_rate_asiic[i,2] = trigger_rate[int(layer2asiic[i,2])-1,1]




#def fix_asiic(trigger_rate=None):
#    layer2asiic = np.loadtxt('layer2asiic.txt')
#    trig_rate_asiic = np.zeros((20,4))
#    for i in range(0,20):
#        trig_rate_asiic[i,0] = i
#        trig_rate_asiic[i,1] = trigger_rate[int(layer2asiic[i,1])-1,1]
#        trig_rate_asiic[i,2] = trigger_rate[int(layer2asiic[i,2])-1,1]
#    return trig_rate_asiic