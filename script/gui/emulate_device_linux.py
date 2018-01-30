"""
Sends fake data to the C++ DAQ module.
"""

import numpy as np
import signal
import os
import time

# Lets us exit with CTRL+C
signal.signal(signal.SIGINT, signal.SIG_DFL)

# Set up a named pipe
pipe_name = '/tmp/test_pipe'

while True:
    if not os.path.exists(pipe_name):
        os.mkfifo(pipe_name)
    
    print ("Opening pipe")
    
    p = os.open(pipe_name, os.O_WRONLY)
    counter = 0
   
    try:
        i = -1
        while True:
            i += 1
            if i % 100 == 0:
                print('sending event', i)
            os.write(p, b'\xAA\xAA\xAA\xAA')
            for i in range(1,49):
                os.write(p, b'\x57\x53\xFC\xFE')
                os.write(p, b'\x00\x00\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x00\x0B\x00\x0C\x00\x0D\x00\x0E\x00\x0F')
                os.write(p, b'\x97\x98\xFB\xFD')
            os.write(p, b'\xF0\xF0\xF0\xF0')
    except IOError as e:
        print("Connection closed, restarting")
        continue
        
    finally:
        print("Closing pipe")
        os.close(p)

    
