#! /home/nsanthony/anaconda3/bin/python

from PyQt5.QtWidgets import (QWidget , QPushButton, QFileDialog, QComboBox)
from PyQt5.QtCore import QSize

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
from time import gmtime, strftime, localtime
import pylab

#custom function imports
from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
from trigrate import trigrate_chip
from trigrate import trigrate_channel

#UNUSED imports 
#from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
#from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
#from PyQt5.QtWidgets import (QLabel, QLineEdit, QApplication, QMessageBox,
#                             qApp, QGridLayout, QTextEdit, QHBoxLayout, QVBoxLayout,
#                             QLCDNumber, QSlider, QInputDialog, QSplitter, QDockWidget,
#                             QFrame, QMainWindow, QAction, QToolTip)
#import pyqtgraph as pg
#from PyQt5.QtGui import QFont, QStatusBar   
#from PyQt5.QtCore import QCoreApplication, QRect, Qt, pyqtSignal, QObject, 





#Global definitions to be used later
files = []
chip_select = 1
num_files = 0
chips = 40
events = 0
tree = 0
trigger_rate = None
time = None
start = ''
end = ''
last_year = 2030
first_year = 2015
strftime("%Y%m%d-000000", localtime())
current_year = strftime("%Y", localtime())
current_month = strftime("%m",localtime())
current_day = strftime("%d",localtime())
dates = [['%s'%first_year,'%s'%current_year],['01','%s'%current_month],['01','%s'%current_day]]

class yeardrop(QWidget):
        
    def __init__(self,se=None):
        super().__init__()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        self.initUI(se=self.startend)
        
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self,se=None):
        global current_year,first_year       
        yearlist = QComboBox(self)
        for i in range(first_year,last_year+1):
            yearlist.addItem("%d"%i)
        if self.startend == 'end':
            acy = int(current_year) - first_year
            yearlist.setCurrentIndex(acy)
        yearlist.activated[str].connect(self.onActivated)
            
    def onActivated(self, text):
#        print(self.startend)
        if self.startend == 'start':
            dates[0][0] = text
#            print(dates)
        elif self.startend == 'end':
            dates[0][1] = text
#            print(dates)
        
class monthdrop(QWidget):
    def __init__(self,se=None):
        super().__init__()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        self.initUI(se=self.startend)
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self,se=None):
        global current_month
        monthlist = QComboBox(self)
        for i in range(1,13):
            if i < 10:
                monthlist.addItem("0%d"%i)
            else:
                monthlist.addItem("%d"%i)
        if self.startend == 'end':
            monthlist.setCurrentIndex(int(current_month)-1)
        monthlist.activated[str].connect(self.onActivated)
    def onActivated(self, text):
#        print(self.startend)
        if self.startend == 'start':
            dates[1][0] = text
#            print(dates)
        elif self.startend == 'end':
            dates[1][1] = text
#            print(dates)


class daydrop(QWidget):
    def __init__(self,se=None):
        super().__init__()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        self.initUI(se=self.startend)
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self,se=None):
        global current_day
        daylist = QComboBox(self)
        for i in range(1,10):
            daylist.addItem("0%d"%i)
        for i in range(10,32):
            daylist.addItem("%d"%i)
        if self.startend == 'end':
            daylist.setCurrentIndex(int(current_day)-1)
        daylist.activated[str].connect(self.onActivated)
    def onActivated(self, text):
#        print(self.startend)
        if self.startend == 'start':
            dates[2][0] = text
#            print(dates)
        elif self.startend == 'end':
            dates[2][1] = text
#            print(dates)
        
class chipdrop(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        chiplist = QComboBox(self)
        for i in range(1,41):
            chiplist.addItem("%d"%i)
        chiplist.activated[str].connect(self.chip_selection)
    def chip_selection(self, text):
        global chip_select
        chip_select = int(text)

class select_file(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        select_file = QPushButton('Select File',self)
        select_file.setToolTip('Select specific files to be used')
        select_file.clicked.connect(self.showDialog)
        
    def showDialog(self):
        global files,start,end
        app_dir = '/home/nsanthony/cream_lab/trigger_rate'
        makeroot_path = '/home/nsanthony/cream_lab/L0data/'
        path = makeroot_path + '/cream'
        file_dialog = QFileDialog()
        file_dialog.setFileMode(QFileDialog.ExistingFiles)
        fname = file_dialog.getOpenFileNames(self, 'Open files', directory="/home/nsanthony/cream/data/cdps2/cream",filter='*.dat')
        if fname[0] != []: 
            #checks to maek sure that the dialog box was not closed 
            #without making a selection   
            num_files = len(fname[0])
            files = ['']*num_files
            for i in range(0,num_files):
                files[i] = fname[0][i] + ' '
            
            
            os.chdir(makeroot_path)
            os.system('./list_override')
            os.chdir(path)
            file_list = open('LIST','w')
            file_list.writelines(["%s\n"%lines for lines in files])
            file_list.close()
            os.chdir(app_dir)
            
            numf = len(files)
            start = files[0][50:65]
            end = files[numf-1][50:65]
            
            
            get_tree_data()
        

class fullday(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        full_day = QPushButton('Full Day',self)
        full_day.clicked.connect(self.get)
        full_day.setToolTip('Gets the data and plots it for the entire day selected by end date/time')
        
    def get(self):
        global files, start, end
        start = '%s%s%s-000000'%(dates[0][1],dates[1][1],dates[2][1])
        end = '%s%s%s-235959'%(dates[0][1],dates[1][1],dates[2][1])
        files = getfilelist(start=start,end=end)
        
        numf = len(files)
        start = files[0][50:65]
        end = files[numf-1][50:65]

        get_tree_data()
        
        
class getdata(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        get_data = QPushButton('Get Data',self)
        get_data.clicked.connect(self.get)
        get_data.setToolTip('Gets the data and plots it for the date range selected')
        
    def get(self):
        global files, start, end
        start = '%s%s%s-000000'%(dates[0][0],dates[1][0],dates[2][0])
        end = '%s%s%s-235959'%(dates[0][1],dates[1][1],dates[2][1])
        files = getfilelist(start=start,end=end)
        numf = len(files)
        start = files[0][50:65]
        end = files[numf-1][50:65]
        get_tree_data()
        
class last(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        last_file = QPushButton('Last File',self)
        last_file.clicked.connect(self.file)
        last_file.setToolTip('This selects the last full .dat file that was created either on the date selected or if none selected most recent')
    
    def file(self):
        global files, start, end, current_year,current_month,current_day
        if dates == [['%s'%first_year,'%s'%current_year],['01','%s'%current_month],['01','%s'%current_day]]:
#            end = strftime("%Y%m%d-000000", gmtime())
            files = getfilelist(last=1)
            end = files[0][50:65]
#            print('Most Recent File')
        else:
            start = '%s%s%s-000000'%(dates[0][0],dates[1][0],dates[2][0])
            end = '%s%s%s-235999'%(dates[0][0],dates[1][0],dates[2][0])
            files = getfilelist(start=start,end=end,last=1)
#            print('Printing Last File in Date Range')

        get_tree_data()
#        print('done that')

        


def get_tree_data():
        global tree, events, num_files,files, start, end
        makeroot(files)
        tree = complete_tree(file_list=files)
        num_files = len(files)
        [trigger_rate,time,layer2asiic] = trigrate_chip(tree=tree,files=num_files)
        events = len(tree)        
        plt.close(1)
        plt.figure(1)
        y_data = layer2asiic[:,0]
        y_data = y_data.astype(int)
        x_data = trigger_rate[:,1]
        np.savetxt('trigger_test.txt',x_data)
        xdat = pd.Series.from_array(x_data)
        plt.figure(1)
        this_manager = pylab.get_current_fig_manager()
        this_manager.window.resize(600,800)
        ax = xdat.plot(kind='barh')
        ax.set_title('Trigger Rate: %s -> %s'%(start,end))
        ax.set_xlabel('Events (Hz)')
        ax.set_ylabel('Layer')
        ax.set_xlim(0,20)
        ax.legend(['Asiic Chip'])
        ax.set_yticklabels(y_data)
        for label in ax.yaxis.get_ticklabels()[1:40:2]:
            label.set_visible(False)
        plt.gca().invert_yaxis()
        rects = ax.patches
        

        for rect, label in zip(rects, layer2asiic[:,1]):
            width = rect.get_width()
            ax.text(width + 0.8, rect.get_y() + rect.get_height() - 0.18, label, ha='right', va='center')
        
        plt.show()

class getchipdata(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        get_chip_plot = QPushButton('Plot Chip Data',self)
        get_chip_plot.clicked.connect(self.call_chip)
        get_chip_plot.setToolTip('Gets the data and plots it for the ASIIC Chip selected')
        
    def call_chip(self):
        getchip(self)

    
class nextchip(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
    def initUI(self):
        next_chip = QPushButton('>',self)
        next_chip.resize(QSize(20, 20))
        next_chip.clicked.connect(self.call_chip)
        next_chip.setToolTip('Next ASIIC Chip')
    def call_chip(self):
        getchip(self,nextc=0)

            
class previouschip(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
    def initUI(self):
        next_chip = QPushButton('<',self)
        next_chip.clicked.connect(self.call_chip)
        next_chip.resize(QSize(20, 20))
        next_chip.setToolTip('Previous ASIIC Chip')
    def call_chip(self):
        getchip(self,previousc=0)           
        
def getchip(self,nextc=None,previousc=None):
    global tree, events, num_files,chip_select, start, end
    if events == 0:
        print('Please select a set of data first')
    if nextc == 0:
        if chip_select < 40:
            chip_select += 1
            process_data(self)
        else:
#            print(chip_select)
            print()
            print('Chip number already at 40')
    elif previousc == 0:
        if chip_select > 1:
            chip_select -= 1
            process_data(self)
        else:
            print()
            print('Chip number already at 1')
    else:
        process_data(self)            
def process_data(self):
    global tree, events, num_files,chip_select, start, end
    [trigger_rate,time] = trigrate_channel(tree=tree,chip=chip_select,files=num_files)
    plt.close(2)
    plt.figure(2)
    plt.barh(trigger_rate[:,0],trigger_rate[:,1])
    plt.ylim(trigger_rate[0,0],trigger_rate[63,0])
    plt.xlim(0,100)
    this_manager = pylab.get_current_fig_manager()
    this_manager.window.move(1,1)
    plt.xlabel('Events (Hz)')
    plt.ylabel('Channel Number')
    plt.title('Asiic Chip %d Trigger Rate: %s -> %s'%(chip_select,start,end))
    plt.show()

        
        
        
        