# KC705 Firmware Information
Kintex\-7 FPGA KC705

### Version 1.3: Last update ( 04/08/2018 )

**Change from v1.2**  
1. FIFO Non-block mode. 
To be more concrete, if the number of words in the FIFO reaches 
a certain threshold (programmable full), the writing to the FIFO will be
stopped from the next frame, and it waits until the number of words in the
FIFO becomes less than another threshold by reading from PC side.

2. Only complete frame can be written into the FIFO. The above function 
guarantees this functionality.  

3. Change the logic to handle the SR_OUT signal, so that the system works, when the SR_OUT is  always HIGH. (It happens running the firmware 
without the sensor, but does not it the sensor is really connected. ) 

4. Modify running status part, and add run status/fifo status into the 
register.

5. Move back the functionality of row/column selector, together with 
re-arrangement of data sample (CNV clock) timing adjustment.  
 

* Command Line access (using xillybus executable)
> hexdump.exe \-C \-v \-n 32 \\\\.\\xillybus_mem_8

* Write register:
> memwrite.exe  \\\\.\\xillybus_mem_8  [address]  [value]

* Read register:
> memread.exe  \\\\.\\xillybus_mem_8  [address]

  
\[  **Register Table**  \]
  
| Address | Value | Direction | Description |
|--:|:--:|:--:|:--|
| 1       |  15   | Write only |  Reset      |
| 2       |  10   | Write only |  Set        |
| 3       |  15   | Write only |  Start      |
| 4       |  15   | Write only |  Stop       |
| 5       |       | | (reserved)  |
| 6       |       | | (reserved)  |
| 7       |       | Read only| RUN status  |
| 8      |       | Read/Write|  Sensor sector address (default = 0 is set in the firmware) |
| 9       |       | | (reserved) |
| 10      |       | Read/Write | Row Start. When SET command is issued, the value is set. (default = 0)  |
| 11      |       | Read/Write  |  Row End   (default = 47) |
| 12      |       | Read/Write  |  Column Start  (default = 0) |
| 13      |       | Read/Write  |  Column End    (default = 15) |
| 14      |       | | (reserved) |
| 15      |       | | (reserved) |
| 16      |       | Read only | FIFO status |

* Row/Column selector : The defalt value ( row_start=0, row_end=0, col_start=0, col_end=0 ) is taken as row_start=0, row_end=47, col_start=0, col_end=15 to prevent the situation of issuing SET command without any input number for row/column.   

* Run status : 0 (INITIAL), 1 (IDLE), 2 (STARTUP), 3 (WAIT), 4 (BUSY)  
 -- There is no trigger in this version,  and BUSY status is only prepared for future application. 
 -- INITIAL or IDLE status represents the system is in condition to have stopped. ( or not started yet )   
 -- STARTUP is the status just after START command and this status is issuing veto on filling FIFO for 4000 clocks (100MHz) to cover the timing of RESET etc. accompanied with START. However, 4000 clocks with 100Mhz is really short, and is difficult to observe this status.  
-- The meaning of Wait is the status for waiting next trigger. Since there is no trigger yet, the running (filling into FIFO normally) status is this WAIT status.   
 
* FIFO stats : D[2:0] (3bit). [ FIFO_full,FIFO_programmable_Full,FIFO_empty ]  
-- FIFO_empty : fifo is empty. Note that this signal synchronizes with FIFO write side, and for example, not empty state, can be possibly marked as FIF_empty with the situation of a few word left in the FIFO.   
-- FIFO_programmable_full : the number of words in the FIFO reaches a threshold. When it is asserted, further writing to the FIFO is stopped.   
-- FIFO_full : Since the FIFO_programmable_full prevents the FIFO to be completely full, it can be asserted only when the timing of FIFO clear(reset).   


\[  **Data Format**  \]

| Name  | Description (Value) |
|:--:|:--|
| Frame Header   |  0xaaaa_aaaa      |
| Row Header (row 1) | 0xf [D31..D28], event_type [D27..D26], row_counter [D25..D20], 4bit counter [D19..D16], column_start [D15..D12], column_end [D11..D8], 00 [D7..D6], memory address [D5..D0] |
| Data (row 1) |  ch2 [D31..D16], ch1 [D15..D0] |
| Data (row 1) |  ch4 [D31..D16], ch3 [D15..D0] |
| Data (row 1) |  ch6 [D31..D16], ch5 [D15..D0] |
| Data (row 1) |  ch8 [D31..D16], ch7 [D15..D0] |
| Data (row 1) |  ch10 [D31..D16], ch9 [D15..D0] |
| Data (row 1) |  ch12 [D31..D16], ch11 [D15..D0] |
| Data (row 1) |  ch14 [D31..D16], ch13 [D15..D0] |
| Data (row 1) |  ch16 [D31..D16], ch15 [D15..D0] |
| Row Footer (row 1)| 0xe [D31..D28], row_start [D27..D22], row_end [D21..D16], 16bit frame counter [D15..D0], |
| Row Header (row 2) |  ... |
| Data (row 2) | ... |
| Data (row 2) | ... |
| ... | ... |
| ... | ... |
| Row Footer (row 48)|
| Frame Footer   |  0xf0f0_f0f0     |

