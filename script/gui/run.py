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

def subrun(infile, outfile, regfile, run_time):
    listcmd = ["/home/chenlj/Documents/Code/kc705/bin/ManagerTest","-r", regfile, "-i", infile,"-o",outfile,"-s",str(run_time)]
    byte_return = subprocess.run(listcmd)
    return byte_return

def sim_take(run_time, n):
    logger =  set_logger("test.log")
    if(platform.system() == "Windows"):
        infile = "D:/code/JadePixDAQ/kc705/data/2017-12-23-1.dat" 
        outfile = "D:/code/JadePixDAQ/kc705/data/tmp/output/test" 
        regfile = "D:/code/JadePixDAQ/kc705/data/tmp/test_reg"
    else:
        infile = "/tmp/test_pipe" 
        outfile = "/home/chenlj/Documents/Code/kc705/data/sim/test" 
        regfile = "/tmp/test_reg"
    run_time = run_time * 1000 #s
    
    for i in range(0, n):
        logger.info("----------\n")
        logger.info("Start run: " + str(i))
        time1 = time.time()
        byte_return = subrun(infile, outfile, regfile, run_time)
        logger.info("ManagerTest return: " + str(byte_return))
        time2 = time.time()
        sleep_time = time2-time1
        logger.info("time used: " + str(time2-time1))
        time.sleep(sleep_time)

def win_take(run_time, n):
    infile = "//./xillybus_read_32" 
    outfile = "output" 
    regfile = "//./xillybus_mem_8"
    run_time = run_time * 1000 #s
    for i in range(0, n):
        logger.info("----------\n")
        logger.info("Start run: " + str(i))
        time1 = time.time()
        byte_return = subrun(infile, outfile, regfile, run_time)
        logger.info("ManagerTest return: " + str(byte_return))
        time2 = time.time()
        sleep_time = time2-time1
        logger.info("time used: " + str(time2-time1))
        time.sleep(sleep_time)

if __name__ == "__main__":
    sim_take(5, 10)
