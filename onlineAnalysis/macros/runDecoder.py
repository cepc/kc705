#/usr/bin/env python

import jadepix as jp
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np

def drawHitMaps(j, data):
    fig = plt.figure(j/8,figsize=(12,12))
    gs = gridspec.GridSpec(4, 2, height_ratios=[1, 1, 1, 1])
    ax = plt.subplot(gs[j%8])
    cax = ax.imshow(data)
    #cbar = fig.colorbar(cax, orientation='horizontal', format='%.0e')
    cbar = fig.colorbar(cax, orientation='vertical', format='%.0e')


def readRawFile():
    dataFile = "..\\data\\Test201712231.dat" 
    dataStructure = 1
    maxDecodeFrame = 10
    decode = jp.JadepixDecoder.Instance()
    decode.Decode(dataFile, dataStructure, maxDecodeFrame) 
    
    frameNumbers=decode.NoOfFrame()
    print("Frame numbers per event: ", frameNumbers)

    for j in range(0, frameNumbers):
        frame=decode.GetFrame(j)
        print("Digi numbers per frame: ", frame.NoOfDigi())

        digi=frame.GetDigi(1)
        print("Digi ADC: ", digi.GetADC())

        frameMap=[]
        for i in range(0,768):
            digi=frame.GetDigi(i)
            frameMap.append(digi.GetADC())

        hitMaps = np.reshape(frameMap,(16,48))
        drawHitMaps(j, hitMaps)

    fPdf = PdfPages('Hit_Map.pdf')
    for n in range(0, int(frameNumbers/8)):
        fig=plt.figure(n,figsize=(12,12))
        fPdf.savefig()

    fPdf.close()
    input("Please press enter to exit")

if __name__ == "__main__":
    readRawFile()