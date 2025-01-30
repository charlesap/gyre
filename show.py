import os, sys
import logging
import fcntl, select, time
from PyQt5.QtCore import QThread, pyqtSignal


from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow, QDialog, QPlainTextEdit, QPushButton, QVBoxLayout

class Window(QMainWindow):
    """Main Window."""
    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        self.setWindowTitle("Gyre Simulation")
        self.resize(600, 200)
        self.centralWidget = QLabel("Gyre")
        self.centralWidget.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.setCentralWidget(self.centralWidget)

    def update(self,text):
        self.centralWidget = QLabel(text)

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



class MyDialog(QDialog, QPlainTextEdit):
    def __init__(self, win, parent=None):
        super().__init__(parent)

        self.topwin = win
        self.resize(800, 400)
        self.setWindowTitle("Gyre Log")

        self.stdin_reader = StdinReader()
        self.stdin_reader.inputReceived.connect(self.append)
        self.stdin_reader.start()

        logTextBox = QTextEditLogger(self)
#        logTextBox.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
        logging.getLogger().addHandler(logTextBox)
        logging.getLogger().setLevel(logging.DEBUG)

#        self._button = QPushButton(self)
#        self._button.setText('Test Me')

        layout = QVBoxLayout()
        # Add the new logging box widget to the layout
        layout.addWidget(logTextBox.widget)
#        layout.addWidget(self._button)
        self.setLayout(layout)

        # Connect signal to slot
#        self._button.clicked.connect(self.test)

    def append(self, text):
        logging.debug(text)
        parts = text.split(',')
        if len(parts)>3:
            first = parts[0].split(':')
            if len(first)>1:
                if first[0]=="hostname":
                    pid = parts[1].split(':')
                    rank = parts[2].split(':')
                    world = parts[3].split(':')
                    win.setWindowTitle(first[1])
                    try:
                        hosts
                    except NameError:
                        hosts = [0] * int(world[1])
                        pids = [0] * int(world[1])
                    hosts[int(rank[1])]=first[1]
                    pids[int(rank[1])]=pid[1]

#    def test(self):
#        logging.debug('damn, a bug')
#        logging.info('something to remember')
#        logging.warning('that\'s not right')
#        logging.error('foobar')

if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = Window()
    dlg = MyDialog(win)
    dlg.show()
    dlg.raise_()
    win.show()


    sys.exit(app.exec_())

