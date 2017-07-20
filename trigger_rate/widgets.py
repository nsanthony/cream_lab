#! /home/nsanthony/anaconda3/bin/python

from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtWidgets import (QWidget, QToolTip,QLabel, QLineEdit, QPushButton, 
                             QApplication, QMessageBox, QMainWindow, QAction, 
                             qApp, QGridLayout, QTextEdit, QHBoxLayout, QVBoxLayout,
                             QLCDNumber, QSlider, QInputDialog,QFileDialog, QFrame,
                             QSplitter,QComboBox,QDockWidget)
import pyqtgraph as pg
from PyQt5.QtGui import QFont, QStatusBar   
from PyQt5.QtCore import QCoreApplication, QRect, Qt, pyqtSignal, QObject

from getfilelist import getfilelist
import numpy as np
import matplotlib.pyplot as plt
import os

from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
#from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
import matplotlib.pyplot as plt

#function imports
from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
from trigrate import trigrate_chip
from trigrate import trigrate_channel

#global dates
#global last_year
#global first_year
#global trigger_rate
#global time
#global chip_select, num_files, tree, chips, events
files = ['']
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
dates = [['%s'%first_year,'%s'%first_year],['01','01'],['01','01']]

class yeardrop(QWidget):
        
    def __init__(self,se=None):
        super().__init__()
        self.initUI()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self,se=None):
        self.startend = se
        yearlist = QComboBox(self)
        for i in range(first_year,last_year+1):
            yearlist.addItem("%d"%i)
        yearlist.activated[str].connect(self.onActivated)
            
    def onActivated(self, text):
        print(self.startend)
        if self.startend == 'start':
            dates[0][0] = text
            print(dates)
        elif self.startend == 'end':
            dates[0][1] = text
            print(dates)
        
class monthdrop(QWidget):
    def __init__(self,se=None):
        super().__init__()
        self.initUI()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self):
        monthlist = QComboBox(self)
        for i in range(1,13):
            if i < 10:
                monthlist.addItem("0%d"%i)
            else:
                monthlist.addItem("%d"%i)  
        monthlist.activated[str].connect(self.onActivated)
    def onActivated(self, text):
        print(self.startend)
        if self.startend == 'start':
            dates[1][0] = text
            print(dates)
        elif self.startend == 'end':
            dates[1][1] = text
            print(dates)


class daydrop(QWidget):
    def __init__(self,se=None):
        super().__init__()
        self.initUI()
        if se == None:
            self.startend = '1'
        else:
            self.setstartend(sten=se)
        
    def setstartend(self,sten=None):
        self.startend = sten
        
    def initUI(self):
        daylist = QComboBox(self)
        for i in range(1,10):
            daylist.addItem("0%d"%i)
        for i in range(10,32):
            daylist.addItem("%d"%i)
        daylist.activated[str].connect(self.onActivated)
    def onActivated(self, text):
        print(self.startend)
        if self.startend == 'start':
            dates[2][0] = text
            print(dates)
        elif self.startend == 'end':
            dates[2][1] = text
            print(dates)
        
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
        

class fullday(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        full_day = QPushButton('Full Day',self)
        full_day.clicked.connect(self.get)
        
    def get(self):
        global files, start
        start = '%s%s%s-000000'%(dates[0][0],dates[1][0],dates[2][0])
        end = '%s%s%s-235959'%(dates[0][0],dates[1][0],dates[2][0])
        files = getfilelist(start=start,end=end)
#        print(files)
        get_tree_data()
        
        
class getdata(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        get_data = QPushButton('Get Data',self)
        get_data.clicked.connect(self.get)
        
    def get(self):
        global files, start, end
        start = '%s%s%s-000000'%(dates[0][0],dates[1][0],dates[2][0])
        end = '%s%s%s-235959'%(dates[0][1],dates[1][1],dates[2][1])
        files = getfilelist(start=start,end=end)
#        print(files)
        get_tree_data()
        
class last(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        last_file = QPushButton('Last File',self)
        last_file.clicked.connect(self.file)
    
    def file(self):
        global files, start, end
        if dates == [['%s'%first_year,'%s'%first_year],['01','01'],['01','01']]:
            files[0] = getfilelist(last=0)
            print('Most Recent File')
        else:
            start = '%s%s%s-000000'%(dates[0][0],dates[1][0],dates[2][0])
            end = '%s%s%s-235999'%(dates[0][1],dates[1][1],dates[2][1])
            files[0] = getfilelist(start=start,end=end,last=0)
            print('Printing Last File in Date Range')
        get_tree_data()
        print('done that')

        


def get_tree_data():
        global tree, events, num_files,files, start, end
        makeroot(files)
        tree = complete_tree(file_list=files)
        num_files = len(files)
        [trigger_rate,time] = trigrate_chip(tree=tree,files=num_files)
        events = len(tree)        
        plt.close()
        plt.figure(1)
        plt.barh(trigger_rate[:,0],trigger_rate[:,1])
        plt.ylim(chips+1,0)
        plt.xlim(0,events/time)
        plt.xlabel('Events (Hz)')
        plt.ylabel('Asiic Chip')
        plt.title('Trigger Rate: %s -> %s'%(start,end))
        plt.show()


class getchipdata(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        
    def initUI(self):
        get_chip_plot = QPushButton('Plot Chip Data',self)
        get_chip_plot.clicked.connect(self.getchip)
        
    def getchip(self):
        global tree, events, num_files,chip_select, start, end
        if tree == 0:
            print('Please select a set of data first')
        [trigger_rate,time] = trigrate_channel(tree=tree,chip=chip_select,files=num_files)
        plt.close(2)
        plt.figure(2)
        plt.barh(trigger_rate[:,0],trigger_rate[:,1])
        plt.ylim(trigger_rate[0,0],trigger_rate[63,0])
        plt.xlim(0,events/time)
        plt.xlabel('Events (Hz)')
        plt.ylabel('Channel Number')
        plt.title('Asiic Chip %d Trigger Rate: %s -> %s'%(chip_select,start,end))
        plt.show()

        
        
        
        