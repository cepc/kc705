import os
import time
import signal
import platform

def display(command, path, end_time):
    start_time = time.time()
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    while(True):
        os.system(command)
        run_time = time.time()
        if (run_time-start_time) > end_time:
            print("Monitor finish!!")
            break
        elif (run_time-start_time) > 10000:
            print("Monitor overtime!!")
            break
        time.sleep(5)

def monitor(path, end_time):
    sysstr = platform.system()
    if(sysstr == "Windows"):
        command = "dir /a /s " + path
    else:
        command = "ls -lh " + path
    display(command, path, end_time)

if __name__ == "__main__":
    monitor("/home/chenlj/Documents/Code/kc705/data/sim", 1000)


