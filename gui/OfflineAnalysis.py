#/usr/bin/env python

import jadepix as jp
from matplotlib import gridspec
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import time

from PyQt5.QtCore import QThread, pyqtSignal


class OfflineThread(QThread):

    SendOneFrame=pyqtSignal(str)
    SendOver=pyqtSignal(str)


    frame=np.zeros((16, 48), dtype=int)
    frameId=0
    fname='D:code\\kc705\\onlineAnaysis\\data\\TestFile20180105-000.dat'
    struceture=2
    maxframe=5

    PixelADC = []
    col = 7
    row = 21
    
    def __init__(self, parent=None):
        super(OfflineThread, self).__init__(parent)
        self.working=True
        
    def __del__(self):
        self.working=False
        self.wait()
 
    def readRawFile(self,filestr, struc, maxf):
        dataFile = filestr
        dataStructure = struc
        maxDecodeFrame = maxf
        print("maxDecodeFrame: ", maxf)
        decode = jp.JadepixDecoder.Instance()
        decode.Decode(dataFile, dataStructure, maxDecodeFrame) 
        
        frameNumbers=decode.NoOfFrame()
        print('*******************',frameNumbers)

        tmpPixelADC = []
        for j in range(0, frameNumbers):
            frame=decode.GetFrame(j)
            if j%100 == 0:
                print("Frame: ", j)
            #print("Digi numbers per frame: ", frame.NoOfDigi())

            digi=frame.GetDigi(self.col*48+self.row)
            #print("Digi Row: ", digi.GetRowId())
            #print("Digi Col: ", digi.GetColId())
            #print("Digi ADC: ", digi.GetADC())

            tmpPixelADC.append(digi.GetADC())

            frameMap=[]
            for i in range(0,768):
                digi=frame.GetDigi(i)
                frameMap.append(digi.GetADC())

            hitMaps = np.reshape(frameMap,(16,48))

            self.frameId=frame.GetFrameId()
            self.frame=frameMap#
            #print(frameMap)
            self.SendOneFrame.emit('Receive one Frame')
                
            time.sleep(1)     

            self.PixelADC = tmpPixelADC##################
        decode.ReSet()
        self.SendOver.emit('Finish Read')
        
    def run(self):
            if self.working==True:
                self.readRawFile(self.fname,self.struceture,self.maxframe)


if __name__ == "__main__":
    Thr=OfflineThread()
    Thr.run()
    
    
