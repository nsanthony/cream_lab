#! /home/nsanthony/anaconda3/bin/python
import os

def makeroot(files):
    L0_directory = '/home/nsanthony/cream_lab/L0data'
    os.chdir(L0_directory)
#    os.system('./list_override')
#    os.chdir('cream/')
#    f = open('LIST','w')
#    for i in range(0,len(files)):
#        f.write(files[i])
#    f.close()
    os.chdir(L0_directory)
    os.system('./makeroot_trigger_rate')