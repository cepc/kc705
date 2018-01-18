
from PyQt5 import QtGui, QtCore, QtWidgets, uic, Qt
import os, sys

import numpy as np

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure


form_class = uic.loadUiType("JadePixel.ui")[0]

class MainWindow(QtWidgets.QMainWindow, form_class):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        
        self.Draw_Pixel_Image()
    
        self.Btn_StartRun.clicked.connect(self.Btn_StartRun_Clicked)
        self.Btn_StopRun.clicked.connect(self.Btn_StopRun_Clicked)
      
        self.Action_Open.triggered.connect(self.Action_Open_Clicked)
        self.Action_Save.triggered.connect(self.Action_Save_Clicked)
        self.Action_Exit.triggered.connect(self.Action_Exit_Clicked)
        
        self.RadioButton_Single.toggled.connect(self.Mode_Update)
        self.SpinBox_MinRow.valueChanged.connect(self.Mode_Update)
        self.SpinBox_MinCol.valueChanged.connect(self.Mode_Update)
        
        self.update()

#####################################################################

    def Draw_Pixel_Image(self):
     
        fig = Figure((3.0, 4.0))
        fig.patch.set_visible(False)
        self.Pixel_Canvas = FigureCanvas(fig)
        self.Pixel_Canvas.setParent(self.Pixel_Graph)
        size = fig.get_size_inches()*fig.dpi
        self.Pixel_Graph.setMinimumSize(*size)
        self.Pixel_Axes = fig.add_subplot(111)

        zeros = np.zeros(shape = (48,16))
        self.Pixel_Image = self.Pixel_Axes.imshow(zeros, animated = True, interpolation = 'nearest')
        self.Pixel_Canvas.draw()
        Pixel_mpl_toolbar = NavigationToolbar(self.Pixel_Canvas, self.Pixel_Graph)
        

    def Btn_StartRun_Clicked(self, arg):
        #self.BtnStartRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)
        self.update()
        return 0

    def Btn_StopRun_Clicked(self):
        #self.BtnStopRun.setAttribute(QtCore.Qt.WA_UnderMouse, False)
        return 0
        
    def Action_Open_Clicked(self):
        open = QtWidgets.QFileDialog()
        open.setWindowModality(QtCore.Qt.WindowModal)
        files=open.getOpenFileNames(self, 'Open data files ', os.getcwd())
        self.LineEdit_CurrentData.setText(self.ComboBox_Data.currentText())
        print(files)
        self.ComboBox_Data.addItems(files[0])
        
        
    def Action_Save_Clicked(self):
        save = QtWidgets.QFileDialog()
        save.setWindowModality(QtCore.Qt.WindowModal)
        file_path=save.getExistingDirectory(self, 'Choose the directory to save data',os.getcwd())
        self.LineEdit_CurrentData.setText(file_path)
        
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
            
        
    def update(self):
        self.Mode_Update()
    

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
