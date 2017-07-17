#! /home/nsanthony/anaconda3/bin/python


import sys
import os
#import PyQt5.QtGui as pyqt
from PyQt5.QtWidgets import QWidget, QToolTip, QPushButton, QApplication, QMessageBox, QMainWindow, QAction, qApp, QTextEdit
from PyQt5.QtGui import QFont    
from PyQt5.QtCore import QCoreApplication




from PyQt5.QtGui import QIcon


class Exampl(QWidget):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):
        qbtn = QPushButton('Quit', self)
        qbtn.clicked.connect(QCoreApplication.instance().quit)
        qbtn.resize(qbtn.sizeHint())
        qbtn.move(900, 660)
#        QToolTip.setFont(QFont('SansSerif', 10))
#        
#        self.setToolTip('This is a <b>QWidget</b> widget')
#        
#        btn = QPushButton('Button', self)
#        btn.setToolTip('This is a <b>QPushButton</b> widget')
#        btn.resize(btn.sizeHint())
#        btn.move(50, 50)   
        
        self.setGeometry(300, 300, 1000, 700)
        self.setWindowTitle('Quit Button')
#        self.setWindowIcon(QIcon('web.png'))        
    
        self.show()
    def closeEvent(self, event):
        
        reply = QMessageBox.question(self, 'Message',
            "Are you sure to quit?", QMessageBox.Yes | 
            QMessageBox.No, QMessageBox.No)

        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()  
        
class Example(QMainWindow):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):               
        
        textEdit = QTextEdit()
        self.setCentralWidget(textEdit) # sets the central widget 
        #menu bar 
        exitAction = QAction(QIcon('exit24.png'), 'Exit', self)
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit application')
        exitAction.triggered.connect(self.close)
        #status bar
        self.statusBar()

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAction)
        #tool bar sets
        toolbar = self.addToolBar('Exit')
        toolbar.addAction(exitAction)
        
        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('Main window')    
        self.show()

    
app = QApplication(sys.argv)
ex = Example()
sys.exit(app.exec_())  