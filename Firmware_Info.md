# KC705@Firmware Information
Kintex\-7 FPGA KC705

#### Version 1.1: Last update ( 02/05/2018 )


* Command Line access (using xillybus executable)
> hexdump.exe \-C \-v \-n 32 \\\\.\\xillybus_mem_8

* Write register:
> memwrite.exe  \\\\.\\xillybus_mem_8  [address]  [value]

* Read register:
> memread.exe  \\\\.\\xillybus_mem_8  [address]

  
\[  ## Register Table \]
  
| Address | Value | Description |
|--:|:--:|:--:|
| 1       |  15   |  Reset      |
| 2       |  10   |  Set        |
| 3       |  15   |  Start      |
| 4       |  15   |  Stop       |
| 5       |       | (reserved)  |
| 6       |       | (reserved   |
| 7     || FIFO Full Flag (value = 1, when FIFO is full, otherwise 0 ) |
| 8       |       |  Sensor Chip address (default = 0 is set in the firmware) |
| 9       |       | (reserved) |
| 10     |       | Row Start. When set command is issued, the value is set. (default = 0) |
| 11      |       |  Row End   (default = 47)   |
| 12      |       |  Column Start  (default = 0) |
| 13      |       |  Column End    (default = 15) |
| 14      |       | (reserved) |
| 15      |       | (reserved) |
| 16      |       | (reserved)|


