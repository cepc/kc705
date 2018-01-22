#from ctypes import windll
#windll.shcore.SetProcessDpiAwareness(True)

import os
import signal
import sys
import threading
import time
from cgi import escape

import daq
import matplotlib
import numpy as np
import win32con
from matplotlib.animation import FuncAnimation
from matplotlib.backends.backend_qt5 import \
    NavigationToolbar2QT as NavigationToolbar
from matplotlib.backends.backend_qt5agg import \
    FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5 import Qt, QtCore, QtGui, QtWidgets, uic

import win32file
from qtthreadutils import invoke_in_main_thread

signal.signal(signal.SIGINT, signal.SIG_DFL)


matplotlib.use('Qt5Agg')


win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')#tao

import OnlineDataRebuild




form_class = uic.loadUiType("JadePixel.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)

        self.events = MyEventListener(self)
        self.dataTaker = daq.DataTaker(self.events)
        self.file_name='C:/Users/Lenovo/Desktop/kc705/test/sim/test.bat'
        
        # scale factor for high dpi screens

        
        self.Draw_Online_Image()

        self.Btn_Online_StartRun.clicked.connect(self.Btn_Online_StartRun_clicked)
        self.Btn_Online_StopRun.clicked.connect(self.Btn_Online_StopRun_clicked)

        self.timer = Qt.QTimer()
        self.timer.timeout.connect(self.Online_Update)
        self.timer.start(1000)

        # logging
        self.TextEdit_Online_Log.document().setDefaultStyleSheet("""
        .error {color: red}
        .warning {color: orange}
        .debug {color: grey}
        .info {color: black}
        """)
        self.TextEdit_Online_Log.setReadOnly(True)
        self.TextEdit_Online_Log.setUndoRedoEnabled(False)

        self.lblStatus = QtWidgets.QLabel()
        #self.lblStatus.setMinimumSize(int(sf*200), 0)
        self.statusBar().addWidget(self.lblStatus)

        self.Online_Update()
        
    

    def Draw_Online_Image(self):
     
        fig = Figure((4.0, 8.0))
        fig.patch.set_visible(False)
        self.Pixel_Online_Canvas = FigureCanvas(fig)
        self.Pixel_Online_Canvas.setParent(self.Pixel_Online_Graph)
        size = fig.get_size_inches()*fig.dpi
        self.Pixel_Online_Graph.setMinimumSize(*size)
        self.Pixel_Online_Axes = fig.add_subplot(211)
        zeros = np.zeros(shape = (16,48))
        self.Pixel_Online_Image = self.Pixel_Online_Axes.imshow(zeros, animated = True, interpolation = 'nearest')
        
        self.Pixel_Online_Axes_hist = fig.add_subplot(212)
        zeros_hist = np.zeros(shape=(1,10))
        #self.Pixel_Online_Hist_Image= self.Pixel_Online_Axes_hist.hist(zeros_hist, 20, animated = True, histtype='stepfilled')

        self.Pixel_Online_Canvas.draw()
        Pixel_Online_mpl_toolbar = NavigationToolbar(self.Pixel_Online_Canvas, self.Pixel_Online_Graph)
        

    def Btn_Online_StartRun_clicked(self, arg):
        self.dataTaker.set_filename(self.file_name)  ## set the filename. test input 
        self.dataTaker.set_simulate_state(1)
        self.dataTaker.start_run()
        self.Online_Update()
        # Bugfix: Without this, the button appears still "hovered"
        # if disabled
        self.Btn_Online_StartRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)



    def Btn_Online_StopRun_clicked(self, arg):
        self.dataTaker.stop_run()
        self.Online_Update()
        self.Btn_Online_StopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)

    def logMessage(self, level, thread_name, string):
        print(string)

        prev_cursor = self.TextEdit_Online_Log.textCursor()
        self.TextEdit_Online_Log.moveCursor(Qt.QTextCursor.End)

        #self.TextEdit_Online_Log.insertPlainText(thread_name + "\t" + string + "\n")
        message = escape(string, True) + "\n"
        classNames = {
            daq.LOG_DEBUG: 'debug',
            daq.LOG_INFO: 'info',
            daq.LOG_WARNING: 'warning',
            daq.LOG_ERROR: 'error',
        }
        className = classNames[level]
        self.TextEdit_Online_Log.insertPlainText(thread_name + "\t")
        self.TextEdit_Online_Log.insertHtml('<div class="%s">%s</div><br>' % (className, message))
        self.TextEdit_Online_Log.setTextCursor(prev_cursor)

    def Online_Update(self):
        print ("In Online_Update")
        state_names = {
            daq.STATE_STOPPED: "stopped",
            daq.STATE_RUNNING: "running",
            daq.STATE_STOPPING: "stopping...",
        }

        state = self.dataTaker.get_state()
        self.Label_Online_State.setText(state_names[state])
        self.lblStatus.setText(state_names[state])
        self.Btn_Online_StartRun.setEnabled(state == daq.STATE_STOPPED)
        self.Btn_Online_StopRun.setEnabled(state == daq.STATE_RUNNING)
        #nevents = self.dataTaker.get_event_number()
        #self.lblEventNumber.addsetText(str(nevents))
        #runNumber = self.dataTaker.get_run_number()
        #self.lblRunNumber.setText(str(runNumber))

        # print ("here")

        if state == daq.STATE_STOPPED:
            if self.timer.isActive():
                self.timer.stop()

        elif state == daq.STATE_RUNNING:
            if not self.timer.isActive():
                self.timer.start()

        # last_events = self.dataTaker.get_accumulated_events()
        # last_events = None
        thebytes = self.dataTaker.get_recent_event()
        #print(thebytes)
        
        '''
        frame=self.rebuild_data(thebytes)#rebuild data
        last_events=[frame]
        
        if last_events:
            summed = sum(last_events)
            self.image.set_data(summed)
            self.image.autoscale()
            
            self.axes.draw_artist(self.image)
            self.canvas.update()
        '''


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
    
    splash.finish(win)

    win.show()

       
    # really quit even if we have running threads
    # this does not e.g. write buffered files to disk,
    # I'm using it for testing only:
    os._exit(app.exec_())
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
