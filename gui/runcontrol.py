from ctypes import windll
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


import win32file, win32con 
win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')#tao


import daq 
from qtthreadutils import invoke_in_main_thread
import OnlineAnalysis
import OfflineAnalysis
import numpy as np
import time

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
        #self.setupStyle(sf)
        self.dpi = 100 * sf
        
        ##########OnlineAnalysis init#########
        file_name=time.strftime('%Y-%m-%d',time.localtime(time.time()))
        file_path= os.getcwd()
        
        self.Draw_online_image()
        
        self.BtnChoose.clicked.connect(self.ChangePath)

        self.BtnStartRun.clicked.connect(self.BtnStartRun_clicked)
        self.BtnStopRun.clicked.connect(self.BtnStopRun_clicked)
        self.FilePathEdit.setText(file_path)
        self.FileNameEdit.setText(file_name)

        self.timer = Qt.QTimer()
        self.timer.timeout.connect(self.update_state)
        self.timer.start(1000)

        # logging
        self.OnlineTextEdit.document().setDefaultStyleSheet("""
        .error {color: red}
        .warning {color: orange}
        .debug {color: grey}
        .info {color: black}
        """)
        self.OnlineTextEdit.setReadOnly(True)
        self.OnlineTextEdit.setUndoRedoEnabled(False)

        self.LabelStatus = QtWidgets.QLabel()
        self.LabelStatus.setMinimumSize(int(sf*200), 0)
        self.statusBar().addWidget(self.LabelStatus)

        self.update_state()
        
        ##########OfflineAnalysis init##########
        self.OfflineThread=OfflineAnalysis.OfflineThread()
        self.BtnSartRead.clicked.connect(self.BtnSartRead_clicked)
        #self.DataEdit
        self.OfflineUpdate()
 
 
     ##########OnlineAnalysis Fun#########
    def Draw_online_image(self):
     
        fig = Figure((3.0, 4.0), dpi=self.dpi)
        fig.patch.set_visible(False)
        self.online_canvas = FigureCanvas(fig)
        self.online_canvas.setParent(self.OnlineGraph)
        size = fig.get_size_inches()*fig.dpi
        self.OnlineGraph.setMinimumSize(*size)
        self.online_axes = fig.add_subplot(111)

        zeros = np.zeros(shape=(48,16))
        self.online_image = self.online_axes.imshow(zeros, animated=True, interpolation='nearest')
        self.online_canvas.draw()

        online_mpl_toolbar = NavigationToolbar(self.online_canvas, self.OnlineGraph)

    def BtnStartRun_clicked(self, arg):
        self.dataTaker.set_filename(self.GetPathAndName())  
        self.dataTaker.start_run()
        self.update_state()
        # Bugfix: Without this, the button appears still "hovered"
        # if disabled
        self.BtnStartRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)

    def BtnStopRun_clicked(self, arg):
        self.dataTaker.stop_run()
        self.update_state()
        self.BtnStopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)

    def ChangePath(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        file_path=open.getExistingDirectory(self, 'Chose the directory to save data',os.getcwd())
        #print(self.path)
        self.FilePathEdit.setText(file_path)
        
        
    def GetPathAndName(self):
        tmp=os.getcwd()+'/'+time.strftime('%Y-%m-%d',time.localtime(time.time()))+'.dat'
        file_path=self.FilePathEdit.text()
        file_name=self.FileNameEdit.text()
        if file_path[-1]=='/':
            file_path=file_path.rstrip('/')
            
        name=file_name+self.DataCBox.currentText()
        tmp=file_path+'/'+name
        
        return tmp


    def logMessage(self, level, thread_name, string):
        print(string)

        prev_cursor = self.OnlineTextEdit.textCursor()
        self.OnlineTextEdit.moveCursor(Qt.QTextCursor.End)

        #self.OnlineTextEdit.insertPlainText(thread_name + "\t" + string + "\n")
        message = escape(string, True) + "\n"
        classNames = {
            daq.LOG_DEBUG: 'debug',
            daq.LOG_INFO: 'info',
            daq.LOG_WARNING: 'warning',
            daq.LOG_ERROR: 'error',
        }
        className = classNames[level]
        self.OnlineTextEdit.insertPlainText(thread_name + "\t")
        self.OnlineTextEdit.insertHtml('<div class="%s">%s</div><br>' % (className, message))
        self.OnlineTextEdit.setTextCursor(prev_cursor)

    def update_state(self):
        print ("In update_state")
        state_names = {
            daq.STATE_STOPPED: "stopped",
            daq.STATE_RUNNING: "running",
            daq.STATE_STOPPING: "stopping...",
        }

        state = self.dataTaker.get_state()
        self.LabelState.setText(state_names[state])
        self.LabelStatus.setText(state_names[state])
        self.BtnStartRun.setEnabled(state == daq.STATE_STOPPED)
        self.BtnStopRun.setEnabled(state == daq.STATE_RUNNING)
        self.BtnChoose.setEnabled(state == daq.STATE_STOPPED)
        self.DataCBox.setEnabled(state == daq.STATE_STOPPED)
        nevents = self.dataTaker.get_event_number()
        self.LableEventNumber.setText(str(nevents))
        runNumber = self.dataTaker.get_run_number()
        self.LableRunNumber.setText(str(runNumber))

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
        #print(thebytes)
        
        frame=OnlineAnalysis.rebuild_data(thebytes)#rebuild data
        last_events=[frame]
        
        if last_events:
            summed = sum(last_events)
            self.online_image.set_data(summed)
            self.online_image.autoscale()
            
            self.online_axes.draw_artist(self.online_image)
            self.online_canvas.update()

    ##########OnlineAnalysis Fun#########
    def BtnSartRead_clicked(self):
        self.BtnSartRead.setEnabled(False)
        self.OfflineThread.start()
        
    def OfflineUpdate(self):
        self.OfflineThread.SendCount.connect(self.LabelReadNumber.setText)
        self.OfflineThread.SendFormat.connect(self.LabelFormat.setText)
        self.OfflineThread.SendTotal.connect(self.LabelEventTotal.setText)
        self.OfflineThread.SendLog.connect(self.OfflineTextEdit.insertPlainText)

#############################################################################
#############################################################################

class MyEventListener(daq.EventListener):
    def __init__(self, window):
        super().__init__()
        self._window = window

    def logMessage(self, level, string):
        print("In logMessage")
        thread = threading.current_thread()
        # invoke_in_main_thread(self._window.logMessage, level, thread.name, string)
        invoke_in_main_thread(self._window.logMessage, level, thread.name, string)


##############################################################################
##############################################################################

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
