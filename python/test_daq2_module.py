"""
Tests the swig DAQ module.
Usage: call python emulate_device.py in one window, and python test_daq_module.py in another.
"""

import signal
import win32con
import win32file

import daq2 as daq

signal.signal(signal.SIGINT, signal.SIG_DFL)

# make named pipe available at //./xillybus_read_32
win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')

class MyListener(daq.EventListener):
    def logMessage(self, level, string):
        pass

l = MyListener()
d = daq.DataTaker(l)
event = d.getRecentEvent()
print(repr(event))
