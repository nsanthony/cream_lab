#! /home/nsanthony/anaconda3/bin/python


import sys
#import PyQt5.QtGui as pyqt
from PyQt5.QtWidgets import (QWidget, QToolTip,QLabel, QLineEdit, QPushButton, 
                             QApplication, QMessageBox, QMainWindow, QAction, 
                             qApp, QGridLayout, QTextEdit, QHBoxLayout, QVBoxLayout,
                             QLCDNumber, QSlider, QInputDialog,QFileDialog, QFrame,
                             QSplitter,QComboBox, QDockWidget)
from PyQt5.QtGui import QFont, QIcon
from PyQt5.QtCore import QCoreApplication, Qt, pyqtSignal, QObject



class Example(QMainWindow):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):      

        self.textEdit = QTextEdit()
        self.setCentralWidget(self.textEdit)
        self.statusBar()

        openFile = QAction(QIcon('open.png'), 'Open', self)
        openFile.setShortcut('Ctrl+O')
        openFile.setStatusTip('Open new File')
        openFile.triggered.connect(self.showDialog)

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(openFile)       
        
        self.setGeometry(300, 300, 350, 300)
        self.setWindowTitle('File dialog')
        self.show()
        
    def showDialog(self):

        fname = QFileDialog.getOpenFileName(self, 'Open file', '/home')

        if fname[0]:
            f = open(fname[0], 'r')

            with f:
                data = f.read()
                self.textEdit.setText(data)     


def main():

    app = QApplication(sys.argv)
    ex = Example()
    ex.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()


#==============================================================================
# from PyQt5.QtGui import QIcon
# import PyQt5.QtGui as QtGui
# 
# class Example(QWidget):
#     def __init__(self):
#         super(Example, self).__init__()
#     
#         self.initUI()
#     
#     def initUI(self):
#     
#         frame = QFrame()
#         frame.setFrameStyle(QFrame.Panel |
#                 QFrame.Plain)
#     
#         label = QLabel('This is random text')
#     
#         dockWidget = QDockWidget('Main', self)
#         # set the widget to non-movable, non-floatable and non-closable
#         dockWidget.setFeatures(dockWidget.NoDockWidgetFeatures)
#         dockWidget.setWidget(label)
#     
#         # add the QDockWidget to the QLayout
#         hbox = QHBoxLayout()
#         hbox.addWidget(dockWidget)
#     
#         # set the layout of the QFrame
#         frame.setLayout(hbox)
#     
#         # create another QLayout to add QFrame
#         vbox = QVBoxLayout()
#         vbox.addStretch(1)
#         vbox.addWidget(frame)
#     
#         self.setLayout(vbox)
#     
#         self.setGeometry(300, 300, 500, 400)
#         self.setWindowTitle('Test')
# 
#==============================================================================

#==============================================================================
# class Example(QWidget):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):      
# 
#         self.lbl = QLabel("Ubuntu", self)
# 
#         combo = QComboBox(self)
#         combo.addItem("Ubuntu")
#         combo.addItem("Mandriva")
#         combo.addItem("Fedora")
#         combo.addItem("Arch")
#         combo.addItem("Gentoo")
# 
#         combo.move(50, 50)
#         self.lbl.move(50, 150)
# 
#         combo.activated[str].connect(self.onActivated)        
#          
#         self.setGeometry(300, 300, 300, 200)
#         self.setWindowTitle('QComboBox')
#         self.show()
#         
#         
#     def onActivated(self, text):
#       
#         self.lbl.setText(text)
#         self.lbl.adjustSize()  
# 
#     
# app = QApplication(sys.argv)
# ex = Example()
# sys.exit(app.exec_()) 
#==============================================================================


#==============================================================================
# 
# #Possible use to split the different areas of the app
# class Example(QWidget):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):      
# 
#         hbox = QHBoxLayout(self)
# 
#         topleft = QFrame(self)
#         topleft.setFrameShape(QFrame.StyledPanel)
#  
#         topright = QFrame(self)
#         topright.setFrameShape(QFrame.StyledPanel)
# 
#         bottom = QFrame(self)
#         bottom.setFrameShape(QFrame.StyledPanel)
# 
#         splitter1 = QSplitter(Qt.Horizontal)
#         splitter1.addWidget(topleft)
#         splitter1.addWidget(topright)
# 
#         splitter2 = QSplitter(Qt.Vertical)
#         splitter2.addWidget(splitter1)
#         splitter2.addWidget(bottom)
# 
#         hbox.addWidget(splitter2)
#         self.setLayout(hbox)
#         
#         self.setGeometry(300, 300, 300, 200)
#         self.setWindowTitle('QSplitter')
#         self.show()
#         
#         
#     def onChanged(self, text):
#         
#         self.lbl.setText(text)
#         self.lbl.adjustSize()    
# 
# 
#         
#==============================================================================

#======================================================c========================
# class Example(QMainWindow):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):      
# 
#         self.textEdit = QTextEdit()
#         self.setCentralWidget(self.textEdit)
#         self.statusBar()
# 
#         openFile = QAction(QIcon('open.png'), 'Open', self)
#         openFile.setShortcut('Ctrl+O')
#         openFile.setStatusTip('Open new File')
#         openFile.triggered.connect(self.showDialog)
# 
#         menubar = self.menuBar()
#         fileMenu = menubar.addMenu('&File')
#         fileMenu.addAction(openFile)       
#         
#         self.setGeometry(300, 300, 350, 300)
#         self.setWindowTitle('File dialog')
#         self.show()
#         
#         
#     def showDialog(self):
# 
#         fname = QFileDialog.getOpenFileName(self, 'Open file', '/home')
# 
#         if fname[0]:
#             f = open(fname[0], 'r')
# 
#             with f:
#                 data = f.read()
#                 self.textEdit.setText(data)        
#==============================================================================


#==============================================================================
#this may help with the ascii selector as a slider 
# class Example(QWidget):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):
#         
#         lcd = QLCDNumber(self)
#         sld = QSlider(Qt.Horizontal, self)
#         sld.setMinimum(0)
#         sld.setMaximum(39)
# 
# 
#         vbox = QVBoxLayout()
#         vbox.addWidget(lcd)
#         vbox.addWidget(sld)
# 
#         self.setLayout(vbox)
#         sld.valueChanged.connect(lcd.display)
#         
#         self.setGeometry(300, 300, 250, 150)
#         self.setWindowTitle('Signal & slot')
#         self.show()
#==============================================================================


#==============================================================================
# class Example(QWidget):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):
#         
#         title = QLabel('Title')
#         author = QLabel('Author')
#         review = QLabel('Review')
# 
#         titleEdit = QLineEdit()
#         authorEdit = QLineEdit()
#         reviewEdit = QTextEdit()
# 
#         grid = QGridLayout()
#         grid.setSpacing(10)
# 
#         grid.addWidget(title, 1, 0)
#         grid.addWidget(titleEdit, 1, 1)
# 
#         grid.addWidget(author, 2, 0)
#         grid.addWidget(authorEdit, 2, 1)
# 
#         grid.addWidget(review, 3, 0)
#         grid.addWidget(reviewEdit, 3, 1, 5, 1)
#         
#         self.setLayout(grid) 
#         
#         self.setGeometry(300, 300, 350, 300)
#         self.setWindowTitle('Review')    
#         self.show()
#==============================================================================


#==============================================================================
# class Exampl(QWidget):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):
#         qbtn = QPushButton('Quit', self)
#         qbtn.clicked.connect(QCoreApplication.instance().quit)
#         qbtn.resize(qbtn.sizeHint())
#         qbtn.move(900, 660)
# #        QToolTip.setFont(QFont('SansSerif', 10))
# #        
# #        self.setToolTip('This is a <b>QWidget</b> widget')
# #        
# #        btn = QPushButton('Button', self)
# #        btn.setToolTip('This is a <b>QPushButton</b> widget')
# #        btn.resize(btn.sizeHint())
# #        btn.move(50, 50)   
#         
#         self.setGeometry(300, 300, 1000, 700)
#         self.setWindowTitle('Quit Button')
# #        self.setWindowIcon(QIcon('web.png'))        
#     
#         self.show()
#     def closeEvent(self, event):
#         
#         reply = QMessageBox.question(self, 'Message',
#             "Are you sure to quit?", QMessageBox.Yes | 
#             QMessageBox.No, QMessageBox.No)
# 
#         if reply == QMessageBox.Yes:
#             event.accept()
#         else:
#             event.ignore()  
#         
# class Example(QMainWindow):
#     
#     def __init__(self):
#         super().__init__()
#         
#         self.initUI()
#         
#         
#     def initUI(self):               
#         
#         textEdit = QTextEdit()
#         self.setCentralWidget(textEdit) # sets the central widget 
#         #menu bar 
#         exitAction = QAction(QIcon('exit24.png'), 'Exit', self)
#         exitAction.setShortcut('Ctrl+Q')
#         exitAction.setStatusTip('Exit application')
#         exitAction.triggered.connect(self.close)
#         #status bar
#         self.statusBar()
# 
#         menubar = self.menuBar()
#         fileMenu = menubar.addMenu('&File')
#         fileMenu.addAction(exitAction)
#         #tool bar sets
#         toolbar = self.addToolBar('Exit')
#         toolbar.addAction(exitAction)
#         
#         self.setGeometry(300, 300, 350, 250)
#         self.setWindowTitle('Main window')    
#         self.show()
#==============================================================================
 