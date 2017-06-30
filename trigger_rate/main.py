#! /home/nsanthony/anaconda3/bin/python

from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
import numpy as np
import matplotlib.pyplot as plt



start = '20160928-000000'
end = '20160929-213000'
files = getfilelist(start=start,end=end)
makeroot(files)
tree = complete_tree(file_list=files)

chips = 40
events = len(tree)
active_chip = np.zeros([chips,2])
chip_list = np.linspace(1,40,num=40)
active_chip[:,0] = chip_list
for i in range(0,events):
    event = tree[i]['cal']
    for j in range(0,chips):
        for k in range(0,64):
            channel = k * (j + 1)
            if event[channel] != 0:
                active_chip[j,1] += 1
                break
minutes = 10*len(files)
time = 60*minutes
trigger_rate = active_chip
trigger_rate[:,1] = trigger_rate[:,1]/time
plt.close()



plt.barh(trigger_rate[:,0],trigger_rate[:,1])
plt.ylim(chips+1,0)
plt.xlim(0,events/time)
plt.xlabel('Events (Hz)')
plt.ylabel('Asiic Chip')
plt.title('Asiic Chip Trigger Rate through event: %s.root'%files[len(files)-1][len(files[0])-20:len(files[0])-5])
plt.show()

