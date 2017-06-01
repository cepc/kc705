
# from ctypes import windll
# windll.shcore.SetProcessDpiAwareness(False)

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

from PyQt5 import QtGui, QtCore, QtWidgets, uic, Qt
import os, sys

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
from matplotlib.animation import FuncAnimation
import pydaq
import numpy as np

form_class = uic.loadUiType("runcontrol.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        self.dataTaker = pydaq.DataTaker()

        # scale factor for high dpi screens
        sf = self.logicalDpiX() / 96
        self.setupStyle(sf)

        self.dpi = 100 * sf
        self.fig = Figure((3.0, 4.0), dpi=self.dpi)
        self.fig.patch.set_visible(False)

        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self.graph)
        size = self.fig.get_size_inches()*self.fig.dpi
        self.graph.setMinimumSize(*size)

        self.axes = self.fig.add_subplot(111)

        zeros = np.zeros(shape=(48,16))
        self.image = self.axes.imshow(zeros, animated=True, interpolation='nearest')
        self.canvas.show()

        self.mpl_toolbar = NavigationToolbar(self.canvas, self.graph)

        self.btnStartRun.clicked.connect(self.btnStartRun_clicked)
        self.btnStopRun.clicked.connect(self.btnStopRun_clicked)

        self.timer = Qt.QTimer()
        self.timer.timeout.connect(self.update_state)
        self.timer.start(1000)

        self.update_state()

    def setupStyle(self, sf):
        self.setStyleSheet("""
            QToolButton {
                padding: """+str(int(6*sf))+"""px;
                border-radius: """+str(int(2*sf))+"""px;
                border: 1px solid transparent;
            }
            QToolButton::hover {
                background-color: rgba(0, 120, 215, 0.1);
                border: 1px solid #0078D7;
            }        

            QGroupBox {
                /*border: """+str(int(2*sf))+"""px groove #ADADAD;*/
                border: """+str(int(1*sf))+"""px solid #C0C0C0;
                border-radius: """+str(int(4*sf))+"""px;
                margin-top: 0.70em;
            }

            QGroupBox::title {
                subcontrol-origin: margin;
                left: """+str(int(7*sf))+"""px;
                padding: 0 """+str(int(3*sf))+"""px 0 """+str(int(3*sf))+"""px;
            }
        """)

    def btnStartRun_clicked(self, arg):
        self.dataTaker.start_run()
        self.update_state()

    def btnStopRun_clicked(self, arg):
        self.dataTaker.stop_run()
        self.update_state()

    def update_state(self):
        state = self.dataTaker.get_state()
        self.lblState.setText(state)
        self.statusBar().showMessage(state)
        self.btnStartRun.setEnabled(state == pydaq.STATE_STOPPED)
        self.btnStopRun.setEnabled(state == pydaq.STATE_RUNNING)
        nevents = self.dataTaker.get_event_number()
        self.lblEventNumber.setText(str(nevents))
        runNumber = self.dataTaker.get_run_number()
        self.lblRunNumber.setText(str(runNumber))

        if state == pydaq.STATE_STOPPED:
            if self.timer.isActive():
                self.timer.stop()

        elif state == pydaq.STATE_RUNNING:
            if not self.timer.isActive():
                self.timer.start()

        last_events = self.dataTaker.get_accumulated_events()
        if last_events:
            summed = sum(last_events)
            self.image.set_data(summed)
            self.image.autoscale()

            self.axes.draw_artist(self.image)
            self.canvas.update()


def main():
    app = QtWidgets.QApplication(sys.argv)
    win = MainWindow()

    win.show()
    # really quit even if we have running threads
    # this does not e.g. write buffered files to disk,
    # I'm using it for testing only:
    os._exit(app.exec_())
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()