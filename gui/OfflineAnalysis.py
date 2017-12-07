import sys
import numpy as np
import struct
import matplotlib.pyplot as plt
from PyQt5.QtCore import QThread, pyqtSignal

EVENTHEADER = "aaaaaaaa"
EVENTTRILER = "f0f0"
EVENTLENGTH = 3840
ROWHEADER_AHEAD = "5753"
ROWTRAILER_AHEAD = "002f"
CONTROLVIEWNUMBER = 1000

VERBOSE = 1
class OfflineThread(QThread):
    
    SendFormat=pyqtSignal(str)
    SendCount=pyqtSignal(str)
    SendTotal=pyqtSignal(str)
    SendLog=pyqtSignal(str)
    
    def __init__(self, parent=None):
        super(OfflineThread, self).__init__(parent)
        self.working=True
        
    def __del__(self):
        self.working=False
        self.wait()

    def Hex2Byte(self, hexStr):
        bytes=[]
        hexStr=''.join(hexStr.split(" "))
        for i in range(0, len(hexStr), 2):
            bytes.append(chr(int(hexStr[i,i+2],16)))
    
        return ''.join(bytes)
    
    def Byte2Hex(self, byteStr):
        return ''.join("%02x" % x for x in byteStr)
    
    def Byte2Int(self, byteStr, width=32):
        val = sum(ord(b) << 8*n for (n,b) in enumerate(reversed(byteStr)))
        if val >= (1 << (width - 1)):
            val = val-(1 << width)
    
        return val
    
    def Byte2Text(self, binStr):
        return binStr.decode("utf-8")
    
    def logPrintLevel0(self, s):
        if(VERBOSE>0):
            print(s)
            self.SendLog.emit(s+'\n')
    
    def logPrintLevel1(self, s):
        if(VERBOSE>1):
            print(s)
            self.SendLog.emit(s+'\n')
    
    def JadiPixRawDataFile(self, fname):
        f = open(fname, "rb")
    
        all = f.read()
        timeStamp = all[:35]
        print("Jadipix time stamp")
        self.SendLog.emit("Jadipix time stamp"+'\n')
        
        print("Binary format: ", timeStamp)
        self.SendFormat.emit(self.Byte2Text(all[:32]))
        #self.SendLog.emit("Binary format: "+self.Byte2Text(timeStamp)+'\n')
        
        print("Hex format: ", self.Byte2Hex(timeStamp))
        #self.SendLog.emit("Hex format: "+self.Byte2Hex(timeStamp)+'\n')
        
        print("String format: ",  self.Byte2Text(timeStamp))
        self.SendLog.emit("Format: "+self.Byte2Text(timeStamp)+'\n')
        
        allHex = self.Byte2Hex(all[35:])
        EventCount=0
        pixelData=np.zeros((48,16))
        while(len(allHex)>EVENTLENGTH):
            if(allHex.find(EVENTHEADER)):
                
                print("Find event header.")
                self.SendLog.emit("Find event header."+'\n')
                
                EventCount += 1
                
                print("Event number: ", EventCount)
                self.SendCount.emit(str(EventCount))
                self.SendLog.emit("Event number: "+str(EventCount)+'\n')
                
                if(EventCount > CONTROLVIEWNUMBER):
                    break
                indexEvent = allHex.index(EVENTHEADER)
                self.logPrintLevel0("indexEvent: " + str(indexEvent))
                subHex=allHex[indexEvent:]
                EventTrailer = subHex[EVENTLENGTH+8:EVENTLENGTH+12]
    
                if(EventTrailer != EVENTTRILER):
                    print("This event is broken")
                    self.SendLog.emit("This event is broken"+'\n')
                    allHex=subHex[EVENTLENGTH:]
                    continue
    
                self.logPrintLevel1(subHex[8:EVENTLENGTH+8])
                self.logPrintLevel1("Type of data: " + str(type(subHex)))
                frameDataHex=np.reshape(list(subHex[8:EVENTLENGTH+8]), (48,80))
                self.logPrintLevel0("Row trailer: " + str(frameDataHex[0,0:4]))
                self.logPrintLevel0("col 01, row 01" + str(frameDataHex[0,8:12]))
                frameData=[]
                for i in range(0,48):
                    colData=[]
                    for j in range(2,18):
                        self.logPrintLevel1(frameDataHex[i,j*4:(j+1)*4])
                        colData.append(int(''.join(frameDataHex[i,j*4:(j+1)*4]),16))
                    frameData.append(colData)
                    self.logPrintLevel1(frameData)
                pixelData += np.array(frameData)
    
            else:
                allHex=subHex[EVENTLENGTH:]
                print("There are no events for this raw data.")
                self.SendLog.emit("There are no events for this raw data."+'\n')
                continue
    
        print("Event Total: ", EventCount)
        self.SendTotal.emit(str(EventCount))
        self.SendLog.emit("Event Total: "+str(EventCount))
        
#        fig=plt.figure(1,figsize=(6,6))
#        plt.imshow(pixelData)
#        plt.colorbar()
    
#        fig.show()
        #input("Please press enter to exit")
    
    def run(self):
        while self.working==True:
            fname="../test/dat/RawEventData_0000.dat"
            self.JadiPixRawDataFile(fname)
            self.working=False

if __name__ == "__main__":
    Thr=OfflineThread()
    #Thr.start()
    Thr.run()
    if Thr.working==False:
        sys.exit()
    
