#/usr/bin/env python

import jadepix as jp
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import time

from PyQt5.QtCore import pyqtSignal,  QObject

class DataSignal(QObject) :
    FrameSignal=pyqtSignal(list)
    def __init__(self, parent=None):
        super(DataSignal, self).__init__(parent)

Signal=DataSignal()

def drawHitMaps(j, data):
    fig = plt.figure(j/8,figsize=(12,12))
    gs = gridspec.GridSpec(4, 2, height_ratios=[1, 1, 1, 1])
    ax = plt.subplot(gs[j%8])
    cax = ax.imshow(data)
    #cbar = fig.colorbar(cax, orientation='horizontal', format='%.0e')
    cbar = fig.colorbar(cax, orientation='vertical', format='%.0e')


def readRawFile(filestr, struc=2, maxf=5):
    dataFile = filestr
    dataStructure = struc
    maxDecodeFrame = maxf
    decode = jp.JadepixDecoder.Instance()
    decode.Decode(dataFile, dataStructure, maxDecodeFrame) 
    
    frameNumbers=decode.NoOfFrame()
    #LogSignal.mes.emit("Frame numbers per event: ", frameNumbers)

    for j in range(0, frameNumbers):
        frame=decode.GetFrame(j)
        #Logl.mesmes.emit("Digi numbers per frame: ", frame.NoOfDigi())

        #digi=frame.GetDigi(1)
        #LogSignal.mes.emit("Digi ADC: ", digi.GetADC())

        frameMap=[]
        for i in range(0,768):
            digi=frame.GetDigi(i)
            frameMap.append(digi.GetADC())

        #hitMaps = np.reshape(frameMap,(16,48))
        #print("------------->", frameMap)
        #print("=========>", type(frameMap))
        Signal.FrameSignal.emit(list(frameMap))#
        #time.sleep(1)
        #drawHitMaps(j, hitMaps)
    
    #    fPdf = PdfPages('Hit_Map.pdf')
    #    for n in range(0, int(frameNumbers/8)):
    #        fig=plt.figure(n,figsize=(12,12))
    #        fPdf.savefig()
    #
    #    fPdf.close()
    #input("Please press enter to exit")

if __name__ == "__main__":
    filestr = "C:/Users/Lenovo/Desktop/JadePixelAnalysis/kc705/test/dat/TestFile20180105-000.dat"
    readRawFile(filestr, struc=2, maxf=10)
    
    
