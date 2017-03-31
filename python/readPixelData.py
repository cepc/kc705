#!/usr/bin/env python3
"""
Plot 2D pixel map
"""

__author__ = "Liejian Chen <chenlj@ihep.ac.cn>"


import matplotlib.pyplot as plt
import numpy as np

def convertData(data):
        tmp_data = ''.join(data[1:]) # Remove event number
        print(tmp_data)
        tmp_pixeldata = bin(int(tmp_data,16))
        print(type(tmp_pixeldata))
        pixelData = list(map(int,tmp_pixeldata[2:]))
        print(pixelData)
        print(type(pixelData))
        return pixelData

def readPixelData(filename):
    with open(filename) as f:
        data = f.readlines()
        f.close()

    eventNumber = []
    nlines = len(data)

    for line in range(nlines):
         value = data[line].split()
         eventNumber.append(value[0])
         pixelData = convertData(value)
         repixelData = np.reshape(pixelData, (48, 16))
         print(repixelData)

         plt.imshow(repixelData)
         plt.show()

    print("eventNumber" + str(eventNumber))
    print("data: " + str(value))
    

if __name__ == "__main__":
    filename = "data.txt"
    readPixelData(filename)

