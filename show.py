import os, sys
import logging
import fcntl, select, time
from PyQt5.QtCore import QThread, pyqtSignal


from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow, QDialog, QPlainTextEdit, QPushButton, QVBoxLayout

from PyQt5.QtWidgets import QWidget, QAction, QTabWidget
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

        # Initialize tab screen
        self.tabs = QTabWidget()
        self.tabOVR = QWidget()
        self.tabDTL = QWidget()
        self.tabDGN = QWidget()
        self.tabLOG = QWidget()
        self.tabs.resize(800,600)

        # Add tabs
        self.tabs.addTab(self.tabOVR,"Overview")
        self.tabs.addTab(self.tabDTL,"Detail")
        self.tabs.addTab(self.tabDGN,"Diagnostics")
        self.tabs.addTab(self.tabLOG,"Log")

        # Create Overview tab
        self.tabOVR.layout = QVBoxLayout(self)
        self.pushButton1 = QPushButton("Push Me")
        self.tabOVR.layout.addWidget(self.pushButton1)
        self.tabOVR.setLayout(self.tabOVR.layout)

        # Create Log tab
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


#        self.tabLOG.layout = QVBoxLayout(self)
        self.tabLOG.layout.addWidget(logTextBox.widget)
        self.setLayout(self.tabLOG.layout)

        # Add tabs to widget
        self.layout.addWidget(self.tabs)
        self.setLayout(self.layout)


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
                    sim.cores=int(rank[1])
                    sim.hosts[int(rank[1])]=first[1]
                    sim.pids[int(rank[1])]=pid[1]


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

