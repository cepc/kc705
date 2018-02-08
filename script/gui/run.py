import os
import time
import platform
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

def run(infile, outfile, regfile, run_time):
    
    if(platform.system() == "Windows"):
        command1 = "D:/code/JadePixDAQ/kc705/bin/ManagerTest.exe "
    else:
        command1 = "/home/chenlj/Documents/Code/kc705/bin/ManagerTest "
    command2 = "-r " + regfile + "  "
    command3 = "-i " + infile  + "  "
    command4 = "-o " + outfile + "  "
    command5 = "-s " + str(run_time) + " "
    command6 = "-p " + str(10000) + " "

    command = command1 + command2 + command3 + command4 + command5 + command6

    os.system(command)


def sim_take(run_time, sleep_time, n):
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
        time1 = time.time()
        run(infile, outfile, regfile, run_time)
        time2 = time.time()
        print("time used: ", time2-time1)
        time.sleep(sleep_time)

def win_take(run_time, sleep_time, n):
    infile = "//./xillybus_read_32" 
    outfile = "output" 
    regfile = "//./xillybus_mem_8"
    run_time = run_time * 1000 #s
    for i in range(0, n):
        run(infile, outfile, regfile, run_time)
        time.sleep(sleep_time)

if __name__ == "__main__":
    sim_take(1, 10, 10)
