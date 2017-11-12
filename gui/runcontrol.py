
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


##RK import win32file, win32con 
##RK win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')#tao


import daq 

from qtthreadutils import invoke_in_main_thread
import numpy as np

import time

form_class = uic.loadUiType("runcontrol.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        # keep this object alive by saving a reference
        self.select_dialog=Directory_Select_Dialog(self)
        self.file_name=os.getcwd()
#        print(self.select_dialog.get_save_path_and_name())

        self.events = MyEventListener(self)
        self.dataTaker = daq.DataTaker(self.events)
        
        # scale factor for high dpi screens
        sf = self.logicalDpiX() / 96
        #self.setupStyle(sf)
        self.dpi = 100 * sf
        
        self.draw_2D_image()

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
        
    

    def draw_2D_image(self):
     
        
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
        

    def btnStartRun_clicked(self, arg):
        self.dataTaker.set_filename(self.file_name)  ## set the filename. test input 
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
        ##RK print(thebytes)
        
        frame=self.rebuild_data(thebytes)#rebuild data
        last_events=[frame]
        
        if last_events:
            summed = sum(last_events)
            self.image.set_data(summed)
            self.image.autoscale()
            
            self.axes.draw_artist(self.image)
            self.canvas.update()

    def rebuild_data(self, thebytes):
        event_data=np.frombuffer(thebytes, dtype=np.uint8, count=1928)
        
        event_data_list=event_data.tolist()
        
        frame_list=event_data_list
        
        #delete  event_header and event_footer
        for j in range(0, 4):
            frame_list.remove(frame_list[0])
            frame_list.pop()
        
        mark=0
        tmp_list=np.zeros(1536, dtype=int)
        x=0
        
        for k in range(0, 1920):
            if k%40==0 or  k%40==1 or k%40==2 or k%40==3 or k%40==36 or k%40==37 or k%40==38 or k%40==39:
                x=1
            if x==0:
                tmp_list[mark]=frame_list[k]
                mark=mark+1
            x=0
            
        frame_list=tmp_list
        #length=len(frame_list)
        #print(length)
        
        frame_array=np.reshape(frame_list, newshape=(48, 16, 2))
        
        frame=np.zeros((48, 16), dtype=int)
        for i in range(0, 48):
            for j in range(0, 16):
                tmp=frame_array[i, j, 0]*256+frame_array[i, j, 1]
                frame[i, j]=tmp
        return frame        

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
class Directory_Select_Dialog(QtWidgets.QDialog):
    def __init__(self, parent=None):
        super(Directory_Select_Dialog, self).__init__(parent)
        
        self.save_path= os.getcwd()#this is the default path
        #print('default path :', self.save_path)
        
        self.save_name=time.strftime('%Y-%m-%d',time.localtime(time.time())) #this is the default filename
        #print('default name :', self.save_name)
        
        self.initUI()
        self.setWindowTitle("Chose the directory to save data ")
        self.setWindowModality(QtCore.Qt.WindowModal)
        self.resize(240, 100)
        
    def initUI(self):
        grid = QtWidgets.QGridLayout()
        
        grid.addWidget(QtWidgets.QLabel('Path：'), 0, 0)
        self.pathLineEdit = QtWidgets.QLineEdit()
        self.pathLineEdit.setReadOnly(True)
        self.pathLineEdit.setFixedWidth(200)
        self.pathLineEdit.setText(self.save_path)
        grid.addWidget(self.pathLineEdit, 0, 1)
        self.chose_button = QtWidgets.QPushButton("Chose..")
        self.chose_button.clicked.connect(self.changePath)
        grid.addWidget(self.chose_button, 0, 2)
        
        grid.addWidget(QtWidgets.QLabel('File name：'), 1, 0)
        self.filenamelineEdit=QtWidgets.QLineEdit()
        self.filenamelineEdit.setFixedWidth(200)
        self.filenamelineEdit.setText(self.save_name)
        self.filenamelineEdit.textChanged.connect(self.changename)
        grid.addWidget(self.filenamelineEdit,1, 1 )
        self.comboBox=QtWidgets.QComboBox()
        self.comboBox.addItem('.dat')
        grid.addWidget(self.comboBox, 1, 2)
        
        
        self.buttonBox = QtWidgets.QDialogButtonBox()
        #buttonBox.setOrientation())# 设置为水平方向
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Ok|QtWidgets.QDialogButtonBox.Cancel)
        self.buttonBox.accepted.connect(self.accept)  # 确定
        self.buttonBox.rejected.connect(self.reject) 
        
        grid.addWidget(self.buttonBox, 2, 1)
        self.setLayout(grid)
        
    def changePath(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        self.save_path=open.getExistingDirectory(self, 'Chose the directory to save data',self.save_path)
        #print(self.path)
        self.pathLineEdit.setText(self.save_path)
        
    def changename(self):
        self.save_name=self.filenamelineEdit.text()
        
    def get_save_path_and_name(self):
        fpath=self.save_path
        fname=self.save_name+self.comboBox.currentText()
        tmp=fpath+'/'+fname
        return tmp
        
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
    win.select_dialog.show()
    
    if win.select_dialog.exec_() or win.select_dialog.buttonBox.rejected:
       win.file_name=win.select_dialog.get_save_path_and_name()
       
    # really quit even if we have running threads
    # this does not e.g. write buffered files to disk,
    # I'm using it for testing only:
    os._exit(app.exec_())
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
