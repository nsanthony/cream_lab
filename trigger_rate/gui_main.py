#! /home/nsanthony/anaconda3/bin/python

import sys
import os
#import numpy as np
#import PyQt5.QtGui as pyqt
from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtWidgets import (QWidget, QToolTip,QLabel, QLineEdit, QPushButton, 
                             QApplication, QMessageBox, QMainWindow, QAction, 
                             qApp, QGridLayout, QTextEdit, QHBoxLayout, QVBoxLayout,
                             QLCDNumber, QSlider, QInputDialog,QFileDialog, QFrame,
                             QSplitter,QComboBox,QDockWidget,QStatusBar)
from PyQt5.QtGui import QFont 
from PyQt5.QtCore import QCoreApplication, Qt, pyqtSignal, QObject, QRect
import pyqtgraph as pg

#imports for functions and widgets
from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
import matplotlib.pyplot as plt
from trigrate import trigrate_chip
from trigrate import trigrate_channel
import widgets

#Possible use to split the different areas of the app



class Frames(QWidget):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):      
        
        startbox = QVBoxLayout()
        endbox = QVBoxLayout()
        graphbox = QVBoxLayout()
        channel_box = QHBoxLayout()
        hbox = QHBoxLayout()

        topleft = QFrame()
#        topleft.QLabel('Time Settings')
        topleft.setFrameShape(QFrame.StyledPanel)
        
        bottomleft = QFrame(self)
#        bottomleft.QLabel('Execute')
        bottomleft.setFrameShape(QFrame.StyledPanel)
        
#        right = QFrame(self)
#        right.setFrameShape(QFrame.StyledPanel)
        
        bottom = QFrame(self)
#        bottom.QLabel('Asiic Chip Plotting')
        bottom.setFrameShape(QFrame.StyledPanel)
        
        
        #TOP left box widgets
        year_widget = QDockWidget('Start Year',self)
        year_widget.setFeatures(QDockWidget.DockWidgetMovable)#makes it look better by removed x icon
        year_widget.setWidget(widgets.yeardrop(se='start'))
        startbox.addWidget(year_widget)

        
        month_widget = QDockWidget('Start Month',self)
        month_widget.setFeatures(QDockWidget.DockWidgetMovable)
        month_widget.setWidget(widgets.monthdrop(se='start'))
        startbox.addWidget(month_widget)
        
        day_widget = QDockWidget('Start Day',self)
        day_widget.setFeatures(QDockWidget.DockWidgetMovable)
        day_widget.setWidget(widgets.daydrop(se='start'))
        startbox.addWidget(day_widget)

        yearend_widget = QDockWidget('End Year',self)
        yearend_widget.setFeatures(QDockWidget.DockWidgetMovable)
        yearend_widget.setWidget(widgets.yeardrop(se='end'))
        startbox.addWidget(yearend_widget)        
    
        monthend_widget = QDockWidget('End Month',self)
        monthend_widget.setFeatures(QDockWidget.DockWidgetMovable)
        monthend_widget.setWidget(widgets.monthdrop(se='end'))
        startbox.addWidget(monthend_widget)
        
        dayend_widget = QDockWidget('End Day',self)
        dayend_widget.setFeatures(QDockWidget.DockWidgetMovable)
        dayend_widget.setWidget(widgets.daydrop(se='end'))
        startbox.addWidget(dayend_widget)

        #bottom left box widgets
        full_day_data = QDockWidget(self)
        full_day_data.setFeatures(QDockWidget.DockWidgetMovable)
        full_day_data.setWidget(widgets.fullday())
        endbox.addWidget(full_day_data)
        
        get_data = QDockWidget(self)
        get_data.setFeatures(QDockWidget.DockWidgetMovable) 
        get_data.setWidget(widgets.getdata())
        endbox.addWidget(get_data)
        
        last_file = QDockWidget(self)
        last_file.setFeatures(QDockWidget.DockWidgetMovable) 
        last_file.setWidget(widgets.last())
        endbox.addWidget(last_file)
        
        #right box widget
   
        
        #bottom box widget
        chip_list = QDockWidget(self)
        chip_list.setFeatures(QDockWidget.DockWidgetMovable)
        chip_list.setWidget(widgets.chipdrop())
        graphbox.addWidget(chip_list)
        
        graph_chip = QDockWidget(self)
        graph_chip.setFeatures(QDockWidget.DockWidgetMovable)
        graph_chip.setWidget(widgets.getchipdata())
        graphbox.addWidget(graph_chip)
        
        
        
        #create boxes with widgets and format (width,height)
        
        topleft.setLayout(startbox)
        topleft.setFixedSize(230,300)    
        
        bottomleft.setLayout(endbox)
        bottom.setLayout(graphbox)
        bottomleft.setFixedSize(230,155)
        bottom.setFixedSize(230,110)
        bottom.setLayout(channel_box)
        

        splitter1 = QSplitter(Qt.Vertical)
        splitter1.addWidget(topleft)
        splitter1.addWidget(bottomleft)
#        splitter1.addWidget(bottom)
        
#        splitter2 = QSplitter(Qt.Vertical)
#        splitter2.addWidget(splitter1)
#        splitter2.addWidget(bottom)
        
        splitter3 = QSplitter(Qt.Vertical)
        splitter3.addWidget(splitter1)
        splitter3.addWidget(bottom)

        hbox.addWidget(splitter3)
        self.setLayout(hbox)
        
        self.setGeometry(300, 300, 230, 520)
        self.setWindowTitle('Trigger Rate Monitor')
        self.show()
        
        
    def onChanged(self, text):
        
        self.lbl.setText(text)
        self.lbl.adjustSize()    



class MainWindow(QMainWindow):
    
    def __init__(self, parent=None):

        super(MainWindow, self).__init__(parent)
        self.frames_widget = Frames(self)
        self.setCentralWidget(self.form_widget) 

    

        
app = QApplication(sys.argv)
ex = Frames()
sys.exit(app.exec_()) 












