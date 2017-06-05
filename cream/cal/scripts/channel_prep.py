#! /Users/nsanthony/miniconda3/bin/python
#import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import os

def prep(detector,date,time,layer,path_root):
    #run this section to get csv_file read in properly and formated to use
    path = '%s/scripts' %(path_root)
    os.chdir(path)
    datetime = '%s-%s' % (date,time)
    command = 'csv_replace.bash -t %s -d %s -l %s -p %s' % (datetime,detector,layer,path_root)
    os.system(command)
    
    path = '%s/files' %(path_root)
    os.chdir(path)
    if detector == 'scd':
        extension = '.spi'
        file_part1 = '%ss' % (datetime)
        file = '%s%s%s' % (file_part1,layer,extension)
    elif detector == 'cal':
        extension = '.spi'
        file_part1 = '%sc' % (datetime)
        file = '%s%s' % (file_part1,extension)
    
    #file to open and generate arrays for
    cha_map = genfromtxt('channel_list.csv', delimiter=',')
    val = genfromtxt(file, delimiter=',')
    mask_crm = genfromtxt('masking_list.csv', delimiter=',')
    
    path = '%s/files/temp_files' %(path_root)
    os.chdir(path)
    
    
        
    
#==============================================================================
#     #what comes from looking at spi file
#     masked_spi =np.zeros((100,1)) 
#     spi_mask_count = 0
#     for i in range(0,len(val)):
#         if val[i,1] == 0:
#             masked_spi[spi_mask_count,0] = val[i,0]
#             spi_mask_count += 1
#             
#     masked_channels_count = 0;
#     for i in range(0,len(mask_crm)):
#         for k in range(0,spi_mask_count):
#             if mask_crm[i,0] == masked_spi[k,0]:
#                 masked_channels_count += 1
#         
#     if masked_channels_count == spi_mask_count:
#         print('Got them all')
#==============================================================================
    
    if detector == 'scd':
        ###this generate the file with format detector-datetime-layer.data
        gen_file = '%s-%s-%s.data' % (detector,datetime,layer) 
        np.savetxt(gen_file,val,delimiter=",")
    
    elif detector == 'cal':
    #this seciton varifies that we have 64 channels to extrapilate from and 
    #puts the channels in order from 1-64 with correct value for the channel type
    #0 is dead, 1 = low, 2 = mid, 3 = high, 4 = LED
    #count = 0
        channels = np.zeros((2,64))
        for i in range(1,65):
            k = 0
            j = 0
        #    print('on :',i)
            while k == 0:
                if j == 73:
                    k = 1
                if cha_map[0,j] == i:
        #            print('found ',i)
        #            count += 1
                    channels[0,i-1] = i
                    channels[1,i-1] = cha_map[1,j]
                    k = 1
                j += 1
                
                
        fchmap = np.zeros((2,len(val[:,3]))) #full channel mapping
        #gets full channel mapping extrapilating values from chan 1-64
        num_asics_2 = int(len(val[:,3])/64) #number of asics divided by 2
        for i in range(0,num_asics_2):
            asic_off = i*64 #this is the asic offset for channels i.e. asic 3 starts
                            #at channel number 97 
            for j in range(0,64):
                chan_num = asic_off + j #channel number
                fchmap[0,chan_num] = channels[0,j] + asic_off #asigns correct channel num
                fchmap[1,chan_num] = channels[1,j] #asigns correct value
        
        #count of each type of channel for first 64
        count_c = np.zeros((5,1),dtype=int) 
        for i in range(0,64):
            if channels[1,i] == 0:
                count_c[0,0] += 1
            elif channels[1,i] == 1:
                count_c[1,0] += 1
            elif channels[1,i] == 2:
                count_c[2,0] += 1
            elif channels[1,i] == 3:
                count_c[3,0] += 1
            elif channels[1,i] == 4:
                count_c[4,0] += 1
                
        #going to be used to sort the different channel types into arrays
        val_dead = np.zeros((count_c[0,0]*num_asics_2,6))
        val_low = np.zeros((count_c[1,0]*num_asics_2,6))
        val_mid = np.zeros((count_c[2,0]*num_asics_2,6))
        val_high = np.zeros((count_c[3,0]*num_asics_2,6))
        val_led = np.zeros((count_c[4,0]*num_asics_2,6))

        dc = 0
        lc = 0
        mc = 0
        hc = 0
        for i in range(0,len(val[:,3])):
            if fchmap[1,i] == 1:
                val_low[lc,:] = val[i,:]
                lc += 1
            elif fchmap[1,i] == 2:
                val_mid[mc,:] = val[i,:]
                mc += 1
            elif fchmap[1,i] == 3:
                val_high[hc,:] = val[i,:]
                hc += 1
        d = {'low':val_low,'mid':val_mid,'high':val_high}
        for key in d:
            if layer == key:
                val = d[key]
        path = '%s/files/temp_files' %(path_root)
        os.chdir(path)
        gen_file = '%s-%s-%s.data' %(detector,datetime,layer)
        np.savetxt(gen_file,val,delimiter=",")
    return











