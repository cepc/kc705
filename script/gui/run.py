import os
import time

def run(infile, outfile, regfile, run_time):
    
    command1 = "./bin/ManagerTest "
    command2 = "-r " + regfile + "  "
    command3 = "-i " + infile  + "  "
    command4 = "-o " + outfile + "  "
    command5 = "-s " + str(run_time)

    command = command1 + command2 + command3 + command4 + command5

    os.system(command)


def sim_take():
    infile = "/tmp/test_pipe" 
    outfile = "/tmp/test_out" 
    regfile = "/tmp/test_reg"
    run_time = 10000 
    for i in range(0,10):
        run(infile, outfile, regfile, run_time)
        time.sleep(1)

def win_take(run_time, n):
    infile = "//./xillybus_read_32" 
    outfile = "output" 
    regfile = "//./xillybus_mem_8"
    run_time = run_time * 1000 #s
    for i in range(0,n):
        run(infile, outfile, regfile, run_time)
        time.sleep(1)

if __name__ == "__main__":
    win_take(100,10)
