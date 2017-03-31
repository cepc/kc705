#!/usr/bin/env python3
"""
Plot 2D pixel map
"""

__author__ = "Liejian Chen <chenlj@ihep.ac.cn>"


import matplotlib.pyplot as plt
import numpy as np

DEBUG = False
def log(s):
    if DEBUG:
        print(s)

class PixelData():
    def __init__(self):
        self.pixelevent = []

    def convert(self,data):
        # Remove event number and combine event data
        tmp_data = ''.join(data[1:])
        log(tmp_data)
        # Hexadecimal to binary
        tmp_pixeldata = bin(int(tmp_data,16))
        log(type(tmp_pixeldata))
        # Pixel data matrix with binary
        pixelData = list(map(int,tmp_pixeldata[2:]))
        log(pixelData)
        log(type(pixelData))
        # reshape pixe data to 48*16 array
        reData = np.reshape(pixelData, (48, 16))
        return reData

    def read(self,filename):
        with open(filename) as f:
            data = f.readlines()
            f.close()

        eventNumber = []
        nlines = len(data)

        for line in range(nlines):
            value = data[line].split()
            eventNumber.append(value[0])
            pixelData = self.convert(value)
            self.pixelevent.append(pixelData)
            log(pixelData)
            if line % 10 == 0:
                self.plot(pixelData)
        log("eventNumber" + str(eventNumber))
        log("data: " + str(value))

    def plot(self,data):
        plt.imshow(data)
        plt.show()

    def simulate(self,nevent):
        for i in range(nevent):
            tmp_simulatedata = np.random.randint(2,size=(48,16))
            if (i % 10 == 0) and DEBUG:
                self.plot(tmp_simulatedata)
            self.pixelevent.append(tmp_simulatedata)

if __name__ == "__main__":
    filename = "data.txt"
    pixeldata = PixelData()
    #pixeldata.read(filename)
    pixeldata.simulate(2)

