#! /home/nsanthony/anaconda3/bin/python

from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
#import numpy as np
import matplotlib.pyplot as plt
from trigrate import trigrate_chip
from trigrate import trigrate_channel


start = '20160928-000000'
end = '20160929-213000'
yesterday = '20170629-000000'
files = getfilelist(start=start)
#makeroot(files)
tree = complete_tree(file_list=files)
num_files = len(files)
[trigger_rate,time] = trigrate_chip(tree=tree,files=num_files)




chips = 40
events = len(tree)
plt.close()
plt.figure(1)
plt.barh(trigger_rate[:,0],trigger_rate[:,1])
plt.ylim(chips+1,0)
plt.xlim(0,events/time)
plt.xlabel('Events (Hz)')
plt.ylabel('Asiic Chip')
plt.title('Asiic Chip Trigger Rate through event: %s.root'%files[len(files)-1][len(files[0])-20:len(files[0])-5])
plt.show()

chip = 4
[trigger_rate,time] = trigrate_channel(tree=tree,chip = chip,files=num_files)
chips = 40
events = len(tree)
#plt.close()
plt.figure(2)
plt.barh(trigger_rate[:,0],trigger_rate[:,1])
plt.ylim(trigger_rate[0,0],trigger_rate[63,0])
#plt.xlim(0,events/time)
plt.xlabel('Events (Hz)')
plt.ylabel('Channel Number')
plt.title('Asiic Chip %d Trigger Rate through event: %s.root'%(chip,files[len(files)-1][len(files[0])-20:len(files[0])-5]))
plt.show()