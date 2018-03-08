import subprocess
import time
import platform
import logging
import sys
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

def set_logger(logfile):
    logger = logging.getLogger("SourceTest")
    formatter = logging.Formatter('%(name)-12s %(asctime)s %(levelname)-8s %(message)s','%a, %d %b %Y %H:%M%S',)
    file_handler = logging.FileHandler(logfile)  
    file_handler.setFormatter(formatter)  
    stream_handler = logging.StreamHandler(sys.stderr)
    logger.addHandler(file_handler)  
    logger.addHandler(stream_handler)  
    logger.setLevel(logging.INFO)  

    return logger

def subrun(infile, outfile, regfile, run_time, evPrints):
    if(platform.system() == "Windows"):
        exefile = "D:/daq_develop/kc705/bin/ManagerTest.exe"
    else:
        exefile = "/home/chenlj/Documents/Code/kc705/bin/ManagerTest"
    
    listcmd = [exefile,"-r", regfile, "-i", infile,"-o",outfile,"-s",str(run_time),"-p",str(evPrints)]
    byte_return = subprocess.run(listcmd)
    return byte_return

def sim_take(run_time, n, evPrints):
    logger =  set_logger("test.log")
    if(platform.system() == "Windows"):
        import win32con
        import win32file
        win32file.DefineDosDevice(win32con.DDD_RAW_TARGET_PATH, r'xillybus_read_32', r'\??\GLOBAL\pipe\test_pipe')
        infile = "//./xillybus_read_32" 
        outfile = "E:/data/tmp/output/test" 
        regfile = "E:/data/tmp/test_reg"
    else:
        infile = "/tmp/test_pipe" 
        outfile = "/home/chenlj/Documents/Code/kc705/data/sim/test" 
        regfile = "/tmp/test_reg"
    run_time = run_time * 1000 #s
    
    for i in range(0, n):
        logger.info("----------\n")
        logger.info("Start run: " + str(i))
        time1 = time.time()
        byte_return = subrun(infile, outfile, regfile, run_time, evPrints)
        logger.info("ManagerTest return: " + str(byte_return))
        time2 = time.time()
        sleep_time = (time2-time1)/10.0
        logger.info("time used: " + str(time2-time1))
        time.sleep(sleep_time)

def win_take(run_time, n, evPrints):
    logger =  set_logger("test.log")
    infile = "//./xillybus_read_32" 
    outfile = "F:\\Ryuta\\data\\test" 
    regfile = "//./xillybus_mem_8"
    run_time = run_time * 1000 #s
    for i in range(0, n):
        logger.info("----------\n")
        logger.info("Start run: " + str(i))
        time1 = time.time()
        byte_return = subrun(infile, outfile, regfile, run_time, evPrints)
        logger.info("ManagerTest return: " + str(byte_return))
        time2 = time.time()
        sleep_time = (time2-time1)/10.0
        logger.info("time used: " + str(time2-time1))
        time.sleep(sleep_time)

if __name__ == "__main__":
    sim_take(120, 2, 100000)

