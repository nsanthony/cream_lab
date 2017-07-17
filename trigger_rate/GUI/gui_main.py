#! /home/nsanthony/anaconda3/bin/python

import sys
import os
#import PyQt5.QtGui as pyqt
from PyQt5.QtWidgets import QApplication, QWidget
app_dir= "/home/nsanthony/cream_lab/trigger_rate"
os.chdir(app_dir)


#imports for functions and widgets
from getfilelist import getfilelist
from makeroot import makeroot
from complete_tree import complete_tree
#import numpy as np
import matplotlib.pyplot as plt
from trigrate import trigrate_chip
from trigrate import trigrate_channel



a = QApplication(sys.argv)
w = QWidget()
w.resize(320,240)
w.setWindowTitle("Hello World")
w.show()
sys.exit(a.exec_())



