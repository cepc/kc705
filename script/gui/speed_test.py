"""
Tests the swig DAQ module.
Usage: call python emulate_device.py in one window, and python test_daq_module.py in another.
"""

import signal
import os
import time
import win32con
import win32file

import daq2 as daq

signal.signal(signal.SIGINT, signal.SIG_DFL)

# https://stackoverflow.com/a/1094933/143091
def sizeof_fmt(num, suffix='B'):
    for unit in ['','K','M','G']:
        if abs(num) < 1024.0:
            return "%3.1f %s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f %s%s" % (num, 'T', suffix)

# # make named pipe available at //./xillybus_read_32
# win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')

class MyListener(daq.EventListener):
    def logMessage(self, level, string):
        pass
        # print ("Message:", string)

l = MyListener()
d = daq.DataTaker(l)

start = time.time()
d.start_run()
time.sleep(5)
d.stop_run_and_join()
end = time.time()

elapsed = end - start
bytes_read = d.get_bytes_read()
print("Time elapsed:", elapsed, "s")
print("Bytes read:", sizeof_fmt(bytes_read))
print("Rate:", sizeof_fmt(bytes_read/elapsed) + "/s")

# event = d.getRecentEvent()
# print(repr(event))

os._exit(0)
