#! /home/nsanthony/anaconda3/bin/python
import numpy as np

channels= np.zeros([40,64])
for i in range(0,40):
    for j in range(0,64):
        channels[i,j] = i*64 + j + 1
channels = channels.astype(int)
np.savetxt('channel_list.txt',channels,fmt='%i')
