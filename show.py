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
        self.resize(400, 200)
        self.centralWidget = QLabel("Gyre")
        self.centralWidget.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.setCentralWidget(self.centralWidget)


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
    def __init__(self, parent=None):
        super().__init__(parent)

        self.stdin_reader = StdinReader()
        self.stdin_reader.inputReceived.connect(self.append)
        self.stdin_reader.start()

        logTextBox = QTextEditLogger(self)
        # You can format what is printed to text box
#        logTextBox.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
        logging.getLogger().addHandler(logTextBox)
        # You can control the logging level
        logging.getLogger().setLevel(logging.DEBUG)

        self._button = QPushButton(self)
        self._button.setText('Test Me')

        layout = QVBoxLayout()
        # Add the new logging box widget to the layout
        layout.addWidget(logTextBox.widget)
        layout.addWidget(self._button)
        self.setLayout(layout)

        # Connect signal to slot
        self._button.clicked.connect(self.test)

    def append(self, text):
        logging.debug(text)

    def test(self):
        logging.debug('damn, a bug')
        logging.info('something to remember')
        logging.warning('that\'s not right')
        logging.error('foobar')

if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = Window()
    win.show()
    dlg = MyDialog()
    dlg.show()
    dlg.raise_()


    sys.exit(app.exec_())

