
#from ctypes import windll
#windll.shcore.SetProcessDpiAwareness(True)

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

from PyQt5 import QtGui, QtCore, QtWidgets, uic, Qt
import os, sys
from cgi import escape
import threading

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
from matplotlib.animation import FuncAnimation


#import win32file, win32con
#win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')


import daq 

from qtthreadutils import invoke_in_main_thread
import numpy as np

form_class = uic.loadUiType("runcontrol.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        # keep this object alive by saving a reference
        self.events = MyEventListener(self)
        self.dataTaker = daq.DataTaker(self.events)

        # scale factor for high dpi screens
        sf = self.logicalDpiX() / 96
        self.setupStyle(sf)

        self.dpi = 100 * sf
        self.fig = Figure((3.0, 4.0), dpi=self.dpi)
        self.fig.patch.set_visible(False)

        self.setWindowIcon(QtGui.QIcon('iheplogo.gif'))
        self.setWindowTitle('DAQ-UI')

        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self.graph)
        size = self.fig.get_size_inches()*self.fig.dpi
        self.graph.setMinimumSize(*size)

        self.axes = self.fig.add_subplot(111)

        zeros = np.zeros(shape=(48,16))
        self.image = self.axes.imshow(zeros, animated=True, interpolation='nearest')
        self.canvas.draw()

        self.mpl_toolbar = NavigationToolbar(self.canvas, self.graph)

        self.btnStartRun.clicked.connect(self.btnStartRun_clicked)
        self.btnStopRun.clicked.connect(self.btnStopRun_clicked)

        self.timer = Qt.QTimer()
        self.timer.timeout.connect(self.update_state)
        self.timer.start(1000)

        # logging
        self.textEdit.document().setDefaultStyleSheet("""
        .error {color: red}
        .warning {color: orange}
        .debug {color: grey}
        .info {color: black}
        """)
        self.textEdit.setReadOnly(True)
        self.textEdit.setUndoRedoEnabled(False)

        self.lblStatus = QtWidgets.QLabel()
        self.lblStatus.setMinimumSize(int(sf*200), 0)
        self.statusBar().addWidget(self.lblStatus)

        self.update_state()



    def setupStyle(self, sf):
        def px(x):
            return str(int(x*sf))+"px"

        self.setStyleSheet("""
            QToolButton {
                padding: """+px(6)+""";
                border-radius: """+px(2)+""";
                border: 1px solid transparent;
            }
            QToolButton::hover {
                background-color: rgba(0, 120, 215, 0.1);
                border: 1px solid #0078D7;
            }        

            QGroupBox {
                /*border: """+px(2)+""" groove #ADADAD;*/
                border: """+px(1)+""" solid #C0C0C0;
                border-radius: """+px(4)+""";
                margin-top: 0.70em;
            }

            QGroupBox::title {
                subcontrol-origin: margin;
                left: """+px(7)+""";
                padding: 0 """+px(3)+""" 0 """+px(3)+""";
            }

            QMenuBar {
                border-bottom: """+px(2)+""" groove #C0C0C0;
                background-color: white;
            }

            QMenuBar::item {
                spacing: """+px(3)+"""; /* spacing between menu bar items */
                padding: """+px(3)+""" """+px(6)+""";
                margin-bottom: """+px(1)+""";
                background: transparent;
                border: """+px(1)+""" solid transparent;
            }

            QMenuBar::item:selected {
                background-color: rgba(0, 120, 215, 0.1);
                border: """+px(1)+""" solid #0078D7;
            }

            QPushButton {
                padding: """+px(4)+""" """+px(8)+""";
            }

            QStatusBar {
                border-top: """+px(2)+""" groove #C0C0C0;
            }

            QStatusBar::item {
                /*border-right: """+px(2)+""" groove #C0C0C0;*/
                border-width: 0px;
            }

            QStatusBar QLabel {
                padding: """+px(3)+""" """+px(6)+""";
            }

            QSizeGrip {
                width: """+px(16)+""";
                height: """+px(16)+""";
            }

        """)

    def btnStartRun_clicked(self, arg):
        self.dataTaker.set_filename("C:\\Users\\daq\\Desktop\\data\\test.dat")  ## set the filename. test input 
        self.dataTaker.start_run()
        self.update_state()
        # Bugfix: Without this, the button appears still "hovered"
        # if disabled
        self.btnStartRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)

    def btnStopRun_clicked(self, arg):
        self.dataTaker.stop_run()
        self.update_state()
        self.btnStopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)

    def logMessage(self, level, thread_name, string):
        print(string)

        prev_cursor = self.textEdit.textCursor()
        self.textEdit.moveCursor(Qt.QTextCursor.End)

        #self.textEdit.insertPlainText(thread_name + "\t" + string + "\n")
        message = escape(string, True) + "\n"
        classNames = {
            daq.LOG_DEBUG: 'debug',
            daq.LOG_INFO: 'info',
            daq.LOG_WARNING: 'warning',
            daq.LOG_ERROR: 'error',
        }
        className = classNames[level]
        self.textEdit.insertPlainText(thread_name + "\t")
        self.textEdit.insertHtml('<div class="%s">%s</div><br>' % (className, message))
        self.textEdit.setTextCursor(prev_cursor)

    def update_state(self):
        print ("In update_state")
        state_names = {
            daq.STATE_STOPPED: "stopped",
            daq.STATE_RUNNING: "running",
            daq.STATE_STOPPING: "stopping...",
        }

        state = self.dataTaker.get_state()
        self.lblState.setText(state_names[state])
        self.lblStatus.setText(state_names[state])
        self.btnStartRun.setEnabled(state == daq.STATE_STOPPED)
        self.btnStopRun.setEnabled(state == daq.STATE_RUNNING)
        nevents = self.dataTaker.get_event_number()
        self.lblEventNumber.setText(str(nevents))
        runNumber = self.dataTaker.get_run_number()
        self.lblRunNumber.setText(str(runNumber))

        # print ("here")

        if state == daq.STATE_STOPPED:
            if self.timer.isActive():
                self.timer.stop()

        elif state == daq.STATE_RUNNING:
            if not self.timer.isActive():
                self.timer.start()

        # last_events = self.dataTaker.get_accumulated_events()
        # last_events = None
        thebytes = self.dataTaker.getRecentEvent()
        frame = np.frombuffer(thebytes, dtype=np.uint8, count=96)
        hits = np.reshape(np.unpackbits(frame), newshape=(48, 16))
        last_events = [hits]

        if last_events:
            summed = sum(last_events)
            self.image.set_data(summed)
            self.image.autoscale()

            self.axes.draw_artist(self.image)
            self.canvas.update()


class MyEventListener(daq.EventListener):
    def __init__(self, window):
        super().__init__()
        self._window = window

    def logMessage(self, level, string):
        print("In logMessage")
        thread = threading.current_thread()
        # invoke_in_main_thread(self._window.logMessage, level, thread.name, string)
        invoke_in_main_thread(self._window.logMessage, level, thread.name, string)


def main():
    app = QtWidgets.QApplication(sys.argv)
    # app.setStyle(QtWidgets.QStyleFactory.create("Fusion"));
    splash = QtWidgets.QSplashScreen(QtGui.QPixmap('splash.png'))

    splash.show()
    splash.showMessage(u'loading...', QtCore.Qt.AlignBottom)
    app.processEvents()

    
    win = MainWindow()

    win.show()
    splash.finish(win)
    # really quit even if we have running threads
    # this does not e.g. write buffered files to disk,
    # I'm using it for testing only:
    os._exit(app.exec_())
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
