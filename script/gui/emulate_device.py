"""
Sends fake data to the C++ DAQ module.
"""

import numpy as np
import signal
import win32con
import win32file
import win32pipe

# Lets us exit with CTRL+C
signal.signal(signal.SIGINT, signal.SIG_DFL)

while True:

    # Set up a named pipe
    print ("Opening pipe")
    p = win32pipe.CreateNamedPipe(r'\\.\pipe\test_pipe',
        win32pipe.PIPE_ACCESS_DUPLEX,
        win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT,
        1, 65536, 65536, 300, None)

    win32pipe.ConnectNamedPipe(p, None)

    try:
        i = -1
        while True:
            i += 1
            if i % 100 == 0:
                print('sending event', i)
            win32file.WriteFile(p, b'\xAA\xAA\xAA\xAA')
            for i in range(1, 49):
                win32file.WriteFile(p, b'\x57\x53\xFC\xFE')
                win32file.WriteFile(p, b'\x00\x00\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x00\x0B\x00\x0C\x00\x0D\x00\x0E\x00\x0F')
                win32file.WriteFile(p, b'\x97\x98\xFB\xFD')
            #win32file.WriteFile(p, bits.tobytes())
            win32file.WriteFile(p, b'\xF0\xF0\xF0\xF0')
    except win32file.error as exc:
        if exc.winerror == 232:
            # pipe has been closed
            print("Connection closed, restarting")
            continue
        # different error, don't handle
        raise
    finally:
        print("Closing pipe")
        win32file.CloseHandle(p)

