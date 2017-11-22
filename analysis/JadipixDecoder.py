import sys
import numpy as np
import struct
import matplotlib.pyplot as plt

EVENTHEADER = "aaaaaaaa"
EVENTTRILER = "f0f0"
EVENTLENGTH = 3840
ROWHEADER_AHEAD = "5753"
ROWTRAILER_AHEAD = "002f"
CONTROLVIEWNUMBER = 1000

VERBOSE = 1

def Hex2Byte(hexStr):
    bytes=[]
    hexStr=''.join(hexStr.split(" "))
    for i in range(0, len(hexStr), 2):
        bytes.append(chr(int(hexStr[i,i+2],16)))

    return ''.join(bytes)

def Byte2Hex(byteStr):
    return ''.join("%02x" % x for x in byteStr)

def Byte2Int(byteStr, width=32):
    val = sum(ord(b) << 8*n for (n,b) in enumerate(reversed(byteStr)))
    if val >= (1 << (width - 1)):
        val = val-(1 << width)

    return val

def Byte2Text(binStr):
    return binStr.decode("utf-8")

def logPrintLevel0(s):
    if(VERBOSE>0):
        print(s)

def logPrintLevel1(s):
    if(VERBOSE>1):
        print(s)

def JadiPixRawDataFile(fname):
    f = open(fname, "rb")

    all = f.read()
    timeStamp = all[:35]
    print("Jadipix time stamp")
    print("Binary format: ", timeStamp)
    print("Hex format: ", Byte2Hex(timeStamp))
    print("String format: ", Byte2Text(timeStamp))
    allHex = Byte2Hex(all[35:])
    EventCount=0
    pixelData=np.zeros((48,16))
    while(len(allHex)>EVENTLENGTH):
        if(allHex.find(EVENTHEADER)):
            print("Find event header.")
            EventCount += 1
            print("Event number: ", EventCount)
            if(EventCount > CONTROLVIEWNUMBER):
                break
            indexEvent = allHex.index(EVENTHEADER)
            logPrintLevel0("indexEvent: " + str(indexEvent))
            subHex=allHex[indexEvent:]
            EventTrailer = subHex[EVENTLENGTH+8:EVENTLENGTH+12]

            if(EventTrailer != EVENTTRILER):
                print("This event is broken")
                allHex=subHex[EVENTLENGTH:]
                continue

            logPrintLevel1(subHex[8:EVENTLENGTH+8])
            logPrintLevel1("Type of data: " + str(type(subHex)))
            frameDataHex=np.reshape(list(subHex[8:EVENTLENGTH+8]), (48,80))
            logPrintLevel0("Row trailer: " + str(frameDataHex[0,0:4]))
            logPrintLevel0("col 01, row 01" + str(frameDataHex[0,8:12]))
            frameData=[]
            for i in range(0,48):
                colData=[]
                for j in range(2,18):
                    logPrintLevel1(frameDataHex[i,j*4:(j+1)*4])
                    colData.append(int(''.join(frameDataHex[i,j*4:(j+1)*4]),16))
                frameData.append(colData)
                logPrintLevel1(frameData)
            pixelData += np.array(frameData)

        else:
            allHex=subHex[EVENTLENGTH:]
            print("There are no events for this raw data.")
            continue

    print("Event Total: ", EventCount)
    fig=plt.figure(1,figsize=(6,6))
    plt.imshow(pixelData)
    plt.colorbar()

    fig.show()
    input("Please press enter to exit")

def main():
    fname="../test/dat/RawEventData_0000.dat"
    JadiPixRawDataFile(fname)

if __name__ == "__main__":
    main()
