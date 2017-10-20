"""
Tests the swig DAQ module.
Usage: call python emulate_device.py in one window, and python test_daq_module.py in another.
"""

import signal
import time
import win32con
import win32file

import daq2 as daq

signal.signal(signal.SIGINT, signal.SIG_DFL)

# make named pipe available at //./xillybus_read_32
win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')

class MyListener(daq.EventListener):
    def logMessage(self, level, string):
        print ("Message:", string)

l = MyListener()
d = daq.DataTaker(l)
d.start_run()
time.sleep(0.5)
d.stop_run()
event = d.getRecentEvent()
print(repr(event))

import os
os._exit(0)