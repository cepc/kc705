
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


form_class = uic.loadUiType("JadePixel.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)

        self.OfflineThread=OfflineAnalysis.OfflineThread()
        
        self.Draw_Pixel_Image()
    
        self.Btn_StartRun.clicked.connect(self.Btn_StartRun_Clicked)
        self.Btn_StopRun.clicked.connect(self.Btn_StopRun_Clicked)
      
        self.Action_Open.triggered.connect(self.Action_Open_Clicked)
        self.Action_Save.triggered.connect(self.Action_Save_Clicked)
        self.Action_Exit.triggered.connect(self.Action_Exit_Clicked)
        
        self.RadioButton_Single.toggled.connect(self.Mode_Update)
        self.SpinBox_MinRow.valueChanged.connect(self.Mode_Update)
        self.SpinBox_MinCol.valueChanged.connect(self.Mode_Update)
        
        self.LineEdit_CurrentData.setText('???')
        
        self.ComboBox_Data.activated.connect(self.LineEdit_CurrentData_Update)

        self.Offline_Update()
        
        
#####################################################################

    def Draw_Pixel_Image(self):
     
        fig = Figure((4.0, 8.0))
        fig.patch.set_visible(False)
        self.Pixel_Canvas = FigureCanvas(fig)
        self.Pixel_Canvas.setParent(self.Pixel_Graph)
        size = fig.get_size_inches()*fig.dpi
        self.Pixel_Graph.setMinimumSize(*size)
        self.Pixel_Axes = fig.add_subplot(211)
        zeros = np.zeros(shape = (16,48))
        self.Pixel_Image = self.Pixel_Axes.imshow(zeros, animated = True, interpolation = 'nearest')
        
        self.Pixel_Axes_hist = fig.add_subplot(212)
        zeros_hist = np.zeros(shape=(1,10))
        #self.Pixel_Hist_Image= self.Pixel_Axes_hist.hist(zeros_hist, 20, animated = True, histtype='stepfilled')

        self.Pixel_Canvas.draw()
        Pixel_mpl_toolbar = NavigationToolbar(self.Pixel_Canvas, self.Pixel_Graph)
        

    def Btn_StartRun_Clicked(self, arg):
        self.Btn_StartRun.setEnabled(False)
        self.ComboBox_Data.setEnabled(False)


        self.OfflineThread.fname=self.LineEdit_CurrentData.text()
        self.OfflineThread.struceture=self.SpinBox_DataStructure.value()
        self.OfflineThread.maxframe=self.SpinBox_MaxFrame.value()

        frame = np.zeros(shape=(16,48))
        self.Pixel_Image.set_data(frame)
        self.Pixel_Image.autoscale()
        self.Pixel_Axes.draw_artist(self.Pixel_Image)
        self.Pixel_Canvas.update()

        self.OfflineThread.start()


        

    def Btn_StopRun_Clicked(self):
        #self.BtnStopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)
        return 0
        
    def Action_Open_Clicked(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        files=open.getOpenFileNames(self, 'Open data files ', os.getcwd())
        self.LineEdit_CurrentData.setText(self.ComboBox_Data.currentText())
        #print(files)
        self.ComboBox_Data.addItems(files[0])
        
        
    def Action_Save_Clicked(self):
        save = QtWidgets.QFileDialog()
        save.setWindowModality(QtCore.Qt.WindowModal)
        #file_path=save.getExistingDirectory(self, 'Choose the directory to save data',os.getcwd())
        
        
    def Action_Exit_Clicked(self):
        self.close()
        
    def Mode_Update(self):
        if self.RadioButton_Multi.isChecked():
            self.SpinBox_MaxRow.setMinimum(self.SpinBox_MinRow.value())
            self.SpinBox_MaxCol.setMinimum(self.SpinBox_MinCol.value())
            #when max=min needs to add a dialog
            self.SpinBox_MaxRow.setEnabled(True)
            self.SpinBox_MaxCol.setEnabled(True)
            
        if self.RadioButton_Single.isChecked():
            self.SpinBox_MaxRow.setValue(self.SpinBox_MinRow.value())
            self.SpinBox_MaxCol.setValue(self.SpinBox_MinCol.value())
            self.SpinBox_MaxRow.setEnabled(False)
            self.SpinBox_MaxCol.setEnabled(False)
            
    def LineEdit_CurrentData_Update(self):
        self.LineEdit_CurrentData.setText(self.ComboBox_Data.currentText())

    def Offline_Update(self):
        self.OfflineThread.SendOver.connect(self.Offline_Btn_Update)
        self.OfflineThread.SendOneFrame.connect(self.Frame_Update)


    def Offline_Btn_Update(self):
        self.Btn_StartRun.setEnabled(True)
        self.ComboBox_Data.setEnabled(True)



    def Offline_Image_Update(self):
        #print(np.reshape(data,  (48, 16)))
        self.Pixel_Image.set_data(np.reshape(self.OfflineThread.frame, (16, 48))) 
        print(self.OfflineThread.PixelADC)
        #self.Pixel_Hist_Image= self.Pixel_Axes_hist.hist(self.OfflineThread.PixelADC, 20, animated = True, histtype='stepfilled')
        #print(self.OfflineThread.frame)     
        self.Pixel_Image.autoscale()          
        self.Pixel_Axes.draw_artist(self.Pixel_Image)
        self.Pixel_Canvas.update()

    def Lable_Update(self):
        self.Label_EventNumber.setText(str(self.OfflineThread.frameId))

    def Frame_Update(self):
        self.Offline_Image_Update()
        self.Lable_Update()
        
    

#############################################################################

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
