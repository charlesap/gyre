import os, sys
import logging
import fcntl, select, time, math
from PyQt5.QtCore import QThread, pyqtSignal


from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow, QDialog, QPlainTextEdit, QPushButton, QVBoxLayout

from PyQt5.QtWidgets import QWidget, QAction, QTabWidget
from PyQt5.QtWidgets import QHBoxLayout, QGroupBox, QGridLayout
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot

class Sim:
    cores = 0
    hosts = None
    pids = None

sim = Sim()

class App(QMainWindow):

    def __init__(self):
        super().__init__()
        self.title = 'Gyre'
        self.left = 0
        self.top = 0
        self.width = 800
        self.height = 600
        self.initUI()
        
    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.table_widget = MyTableWidget(self)
        self.setCentralWidget(self.table_widget)        
        self.show()


class QTextEditLogger(logging.Handler):
    def __init__(self, parent):
        super().__init__()
        self.widget = QPlainTextEdit(parent)
        self.widget.setReadOnly(True)

    def emit(self, record):
        msg = self.format(record)
        self.widget.appendPlainText(msg)


class StdinReader(QThread):
    inputReceived = pyqtSignal(str)

    def run(self):
        while True:
            line = sys.stdin.readline()
            if not line:
                break
            self.inputReceived.emit(line.strip())


class MyTableWidget(QWidget):

    def __init__(self, parent):
        super(QWidget, self).__init__(parent)
        self.layout = QVBoxLayout(self)

        self.tabs = QTabWidget()
        self.tabOVR = QWidget()
        self.tabDTL = QWidget()
        self.tabDGN = QWidget()
        self.tabLOG = QWidget()
        self.tabs.resize(800,600)

        self.addTabs()
        self.createOverviewTab()
#        self.createDetailTab(80)
        self.createLogTab()

        self.layout.addWidget(self.tabs)
        self.setLayout(self.layout)
        
    def addTabs(self):
        self.tabs.addTab(self.tabOVR,"Overview")
        self.tabs.addTab(self.tabDTL,"Detail")
        self.tabs.addTab(self.tabDGN,"Diagnostics")
        self.tabs.addTab(self.tabLOG,"Log")

    def createOverviewTab(self):
        self.tabOVR.layout = QVBoxLayout(self)
        self.pushButton1 = QPushButton("Push Me")
        self.tabOVR.layout.addWidget(self.pushButton1)
        self.tabOVR.setLayout(self.tabOVR.layout)

    def createDetailTab(self,count):
        self.tabDTL.layout = QVBoxLayout(self)
        self.createGridLayout(count)
        windowLayout = QVBoxLayout(self)
        windowLayout.addWidget(self.horizontalGroupBox)
        self.tabDTL.setLayout(windowLayout)
        self.show()

        #self.tabDTL.setLayout(self.tabDTL.layout)

    def createLogTab(self):
        self.tabLOG.layout = QVBoxLayout(self)
        self.plainTextEditLog = QPlainTextEdit()
        self.tabLOG.layout.addWidget(self.plainTextEditLog)
        self.tabLOG.setLayout(self.tabLOG.layout)

        self.stdin_reader = StdinReader()
        self.stdin_reader.inputReceived.connect(self.logAppend)
        self.stdin_reader.start()

        logTextBox = QTextEditLogger(self.plainTextEditLog)
        logging.getLogger().addHandler(logTextBox)
        logging.getLogger().setLevel(logging.DEBUG)

        self.tabLOG.layout.addWidget(logTextBox.widget)
        self.setLayout(self.tabLOG.layout)

    def createGridLayout(self,count):
        self.horizontalGroupBox = QGroupBox("Nodes")
        layout = QGridLayout()
        #layout.setColumnStretch(1, 4)
        #layout.setColumnStretch(2, 4)


        x=math.floor(math.sqrt(count))
        y=count // x
        z=count % x
        c=0
        labels = [0] * (x*y+1)
        for i in range(y):
            for j in range(x):
                labels[x*i+j] = QLabel("node: "+str(x*i+j)+"\nhost: "+sim.hosts[c]+"\npid: "+str(sim.pids[c]))
                labels[x*i+j].setStyleSheet("border: 1px solid black; background-color: yellow")
                labels[x*i+j].setAlignment(Qt.AlignCenter)
                layout.addWidget(labels[x*i+j], i,j)
                #layout.addWidget(QPushButton(sim.hosts[c]+" "+str(sim.pids[c])),i,j)
                c=c+1
        if z > 0:
            for j in range(x):
                if z > j:
                    labels[x*(y+1)+j] = QLabel("node: "+str(x*i+j)+"\nhost: "+sim.hosts[c]+"\npid: "+str(sim.pids[c]))
                    labels[x*(y+1)+j].setStyleSheet("border: 1px solid black; background-color: yellow")
                    labels[x*(y+1)+j].setAlignment(Qt.AlignCenter)
                    layout.addWidget(labels[x*(y+1)+j], i,j)
                    #layout.addWidget(QPushButton(sim.hosts[c]+" "+str(sim.pids[c])),y+1,j)
                c=c+1

        self.horizontalGroupBox.setLayout(layout)

    def logAppend(self, text):
        logging.debug(text) # logging.debug logging.info logging.warning logging.error
        parts = text.split(',')
        if len(parts)>3:
            first = parts[0].split(':')
            if len(first)>1:
                if first[0]=="hostname":
                    pid = parts[1].split(':')
                    rank = parts[2].split(':')
                    world = parts[3].split(':')
                    #self.topwin.setWindowTitle(first[1])
                    global sim
                    #global pids
                    if sim.hosts == None:
                        sim.hosts = [0] * int(world[1])
                        sim.pids = [0] * int(world[1])
                        sim.cores=int(world[1])
                    nparts=first[1].split('.')
                    sim.hosts[int(rank[1])]=nparts[0]
                    sim.pids[int(rank[1])]=int(pid[1])
        if text=="INITIALIZING":
            self.createDetailTab(sim.cores)    

    @pyqtSlot()
    def on_click(self):
        print("\n")
        for currentQTableWidgetItem in self.tableWidget.selectedItems():
            print(currentQTableWidgetItem.row(), currentQTableWidgetItem.column(), currentQTableWidgetItem.text())



if __name__ == "__main__":
    sim = Sim()
    app = QApplication(sys.argv)
    ex=App()


    sys.exit(app.exec_())

