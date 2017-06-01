"""
Sends fake data to the C++ DAQ module.
"""

import signal
import win32pipe, win32file, win32con
import numpy as np

# Lets us exit with CTRL+C
signal.signal(signal.SIGINT, signal.SIG_DFL)

# Set up a named pipe
p = win32pipe.CreateNamedPipe(r'\\.\pipe\test_pipe',
    win32pipe.PIPE_ACCESS_DUPLEX,
    win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT,
    1, 65536, 65536, 300, None)

win32pipe.ConnectNamedPipe(p, None)

# create fake data and send it to the pipe
buf = np.zeros(100, dtype=np.int8)
buf[0] = 0xF0
buf[1:-1:1] = 0xAA
print(buf.tobytes())

win32file.WriteFile(p, buf.tobytes())