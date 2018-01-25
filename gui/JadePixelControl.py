
from PyQt5 import QtGui, QtCore, QtWidgets, uic, Qt
from PyQt5.QtCore import pyqtSignal
import os, sys

import numpy as np

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
from matplotlib.animation import FuncAnimation

import OfflineAnalysis

import signal
import threading
import time
from cgi import escape
import daq
import win32con
import win32file
from qtthreadutils import invoke_in_main_thread
signal.signal(signal.SIGINT, signal.SIG_DFL)
win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')

form_class = uic.loadUiType("JadePixel.ui")[0]


class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)

        #Offline Init#
        self.OfflineThread=OfflineAnalysis.OfflineThread()
        
        self.Draw_Pixel_Offline_Image()

    
        self.Btn_Offline_StartRun.clicked.connect(self.Btn_Offline_StartRun_Clicked)
        #self.Btn_Offline_StopRun.clicked.connect(self.Btn_Offline_StopRun_Clicked)
      
        self.Action_Open.triggered.connect(self.Action_Open_Clicked)
        self.Action_Save.triggered.connect(self.Action_Save_Clicked)
        self.Action_Exit.triggered.connect(self.Action_Exit_Clicked)
        
        self.RadioButton_Offline_Single.toggled.connect(self.Mode_Update)
        self.SpinBox_Offline_MinRow.valueChanged.connect(self.Mode_Update)
        self.SpinBox_Offline_MinCol.valueChanged.connect(self.Mode_Update)
        
        self.LineEdit_Offline_CurrentData.setText('???')
        
        self.ComboBox_Data.activated.connect(self.LineEdit_Offline_CurrentData_Update)

        self.Mode_Update()

        self.Offline_Update()

        #Online Init

        file_name=time.strftime('%Y-%m-%d',time.localtime(time.time()))
        file_path= os.getcwd()

        self.events = MyEventListener(self)
        self.dataTaker = daq.DataTaker(self.events)
        self.dataTaker.set_max_event_number(self.SpinBox_Online_MaxEvents.value())
        
        # scale factor for high dpi screens
        self.LineEdit_Online_FilePath.setText(file_path)
        self.LineEdit_Online_FileName.setText(file_name)
        
        self.Draw_Online_Image()

        self.Btn_Online_StartRun.clicked.connect(self.Btn_Online_StartRun_clicked)
        self.Btn_Online_SimStart.clicked.connect(self.Btn_Online_SimStart_clicked)
        self.Btn_Online_StopRun.clicked.connect(self.Btn_Online_StopRun_clicked)
        self.Btn_Online_Chose.clicked.connect(self.Btn_Online_Chose_clicked)

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

#####################################################################

    def Draw_Pixel_Offline_Image(self):
     
        fig = Figure((4.0, 8.0))
        fig.patch.set_visible(False)
        self.Pixel_Offline_Canvas = FigureCanvas(fig)
        self.Pixel_Offline_Canvas.setParent(self.Pixel_Offline_Graph)
        size = fig.get_size_inches()*fig.dpi
        self.Pixel_Offline_Graph.setMinimumSize(*size)
        self.Pixel_Offline_Axes = fig.add_subplot(211)
        self.Pixel_Offline_Axes.hold(False)
        zeros = np.zeros(shape = (16,48))
        self.Pixel_Offline_Image = self.Pixel_Offline_Axes.imshow(zeros, animated = True, interpolation = 'nearest')
        
        self.Pixel_Offline_Axes_hist = fig.add_subplot(212)
        self.Pixel_Offline_Axes_hist.hold(False)
        zeros_hist = np.zeros(shape=(1,10))
        self.Pixel_Offline_Hist_Image= self.Pixel_Offline_Axes_hist.hist(zeros_hist, 20, histtype='stepfilled')

        self.Pixel_Offline_Canvas.draw()
        Pixel_Offline_mpl_toolbar = NavigationToolbar(self.Pixel_Offline_Canvas, self.Pixel_Offline_Graph)
        
    def Btn_Offline_StartRun_Clicked(self, arg):
        self.Btn_Offline_StartRun.setEnabled(False)
        self.ComboBox_Data.setEnabled(False)


        self.OfflineThread.fname=self.LineEdit_Offline_CurrentData.text()
        self.OfflineThread.struceture=self.SpinBox_Offline_DataStructure.value()
        self.OfflineThread.maxframe=self.SpinBox_Offline_MaxFrame.value()

        frame = np.zeros(shape=(16,48))
        self.Pixel_Offline_Image.set_data(frame)
        self.Pixel_Offline_Image.autoscale()
        self.Pixel_Offline_Axes.draw_artist(self.Pixel_Offline_Image)
        self.Pixel_Offline_Canvas.update()

        self.OfflineThread.start()
     
      
    def SpinBox_Offline_StopRun_Clicked(self):
        #self.BtnStopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)
        return 0
        
    def Action_Open_Clicked(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        files=open.getOpenFileNames(self, 'Open data files ', os.getcwd())
        self.LineEdit_Offline_CurrentData.setText(self.ComboBox_Data.currentText())
        #print(files)
        self.ComboBox_Data.addItems(files[0])
        
        
    def Action_Save_Clicked(self):
        save = QtWidgets.QFileDialog()
        save.setWindowModality(QtCore.Qt.WindowModal)
        #file_path=save.getExistingDirectory(self, 'Choose the directory to save data',os.getcwd())
        
        
    def Action_Exit_Clicked(self):
        self.close()
        
    def Mode_Update(self):
        if self.RadioButton_Offline_Multi.isChecked():
            self.SpinBox_Offline_MaxRow.setMinimum(self.SpinBox_Offline_MinRow.value())
            self.SpinBox_Offline_MaxCol.setMinimum(self.SpinBox_Offline_MinCol.value())
            #when max=min needs to add a dialog
            self.SpinBox_Offline_MaxRow.setEnabled(True)
            self.SpinBox_Offline_MaxCol.setEnabled(True)
            
        if self.RadioButton_Offline_Single.isChecked():
            self.SpinBox_Offline_MaxRow.setValue(self.SpinBox_Offline_MinRow.value())
            self.SpinBox_Offline_MaxCol.setValue(self.SpinBox_Offline_MinCol.value())
            self.SpinBox_Offline_MaxRow.setEnabled(False)
            self.SpinBox_Offline_MaxCol.setEnabled(False)
            
    def LineEdit_Offline_CurrentData_Update(self):
        self.LineEdit_Offline_CurrentData.setText(self.ComboBox_Data.currentText())




    def Offline_Update(self):

        self.OfflineThread.SendOneFrame.connect(self.Frame_Update)
        self.OfflineThread.SendOver.connect(self.OfflineThread_Update)



    def OfflineThread_Update(self):
        self.Offline_Btn_Update()
        self.OfflineThread.quit()

    def Offline_Btn_Update(self):
        self.Btn_Offline_StartRun.setEnabled(True)
        self.ComboBox_Data.setEnabled(True)



    def Offline_Image_Update(self):
        #print(np.reshape(data,  (48, 16)))
        self.Pixel_Offline_Image.set_data(np.reshape(self.OfflineThread.frame, (16, 48))) 

        #print('Current ADC : ',self.OfflineThread.PixelADC)
        self.Pixel_Offline_Hist_Image= self.Pixel_Offline_Axes_hist.hist(self.OfflineThread.PixelADC, 20, histtype='stepfilled')

        #print(self.OfflineThread.frame)
        self.Pixel_Offline_Image.autoscale()
        #self.Pixel_Offline_Hist_Image.autoscale()     
        self.Pixel_Offline_Axes.draw_artist(self.Pixel_Offline_Image)
        self.Pixel_Offline_Canvas.draw()

    def Label_Update(self):
        self.Label_Offline_FrameNumber.setText(str(self.OfflineThread.frameId))

    def Frame_Update(self):
        self.Offline_Image_Update()
        self.Label_Update()

#######################################################
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


    def Btn_Online_Chose_clicked(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        file_path=open.getExistingDirectory(self, 'Chose the directory to save data',os.getcwd())
        self.LineEdit_Online_FilePath.setText(file_path)

    def GetPathAndName(self):
        tmp=os.getcwd()+'/'+time.strftime('%Y-%m-%d',time.localtime(time.time()))+'.dat'
        file_path=self.LineEdit_Online_FilePath.text()
        file_name=self.LineEdit_Online_FileName.text()
        if file_path[-1]=='/':
            file_path=file_path.rstrip('/')
            
        name=file_name+self.ComboBox_Data_Type.currentText()
        tmp=file_path+'/'+name
        
        return tmp


    def Btn_Online_StartRun_clicked(self, arg):
        self.dataTaker.set_filename(self.GetPathAndName())  ## set the filename. test input 
        self.dataTaker.set_simulate_state(0)
        self.dataTaker.start_run()
        self.Online_Update()
        # Bugfix: Without this, the button appears still "hovered"
        # if disabled
        self.Btn_Online_StartRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)


    def Btn_Online_SimStart_clicked(self):
        self.dataTaker.set_filename(self.GetPathAndName())  ## set the filename. test input 
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

        nevents = self.dataTaker.get_event_number()
        self.Label_Online_FrameNumber.setText(str(nevents))
        #runNumber = self.dataTaker.get_run_number()
        #self.lblRunNumber.setText(str(runNumber))

        # print ("here")

        if state == daq.STATE_STOPPED:
            if self.timer.isActive():
                self.timer.stop()

        elif state == daq.STATE_RUNNING:
            if not self.timer.isActive():
                self.timer.start()

    
        thebytes = self.dataTaker.get_recent_event()
        print(thebytes)
        
        frame=self.rebuild_data(thebytes)#rebuild data
        last_events=[frame]
        print(last_events)

        if last_events:
            summed = sum(last_events)
            self.Pixel_Online_Image.set_data(summed)
            self.Pixel_Online_Image.autoscale()
            
            self.Pixel_Online_Axes.draw_artist(self.Pixel_Online_Image)
            self.Pixel_Online_Canvas.update()

    def rebuild_data(self, thebytes):
        event_data=np.frombuffer(thebytes, dtype=np.uint8, count=1920+8)
        print(event_data)
        
        event_data_list=event_data.tolist()
        
        frame_list=event_data_list
        
        #delete  event_header and event_footer
        frame_list = frame_list[4:-4]
        tmp_array = np.reshape(frame_list, newshape=(48,40))
        frame_array = tmp_array[:,4:36]

        frame_array=np.reshape(frame_array, newshape=(48, 16, 2))
        
        frame=np.zeros((48, 16), dtype=int)
        for i in range(0, 48):
            for j in range(0, 16):
                tmp=frame_array[i, j, 0]*256+frame_array[i, j, 1]
                frame[i, j]=tmp
        return frame.T     


class MyEventListener(daq.EventListener):
    def __init__(self, window):
        super().__init__()
        self._window = window

    def logMessage(self, level, string):
        print("In logMessage")
        thread = threading.current_thread()
        invoke_in_main_thread(self._window.logMessage, level, thread.name, string)





###############################################################
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
    os._exit(app.exec_())
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
