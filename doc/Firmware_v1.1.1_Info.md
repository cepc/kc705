# KC705 Firmware Information
Kintex\-7 FPGA KC705

### Version 1.1.1: Last update ( 02/11/2018 )

**Change from v1.1**  
Disable row/column selection. (values can be written in the 
register but no effect)  

* Command Line access (using xillybus executable)
> hexdump.exe \-C \-v \-n 32 \\\\.\\xillybus_mem_8

* Write register:
> memwrite.exe  \\\\.\\xillybus_mem_8  [address]  [value]

* Read register:
> memread.exe  \\\\.\\xillybus_mem_8  [address]

  
\[  **Register Table**  \]
  
| Address | Value | Description |
|--:|:--:|:--|
| 1       |  15   |  Reset      |
| 2       |  10   |  Set        |
| 3       |  15   |  Start      |
| 4       |  15   |  Stop       |
| 5       |       | (reserved)  |
| 6       |       | (reserved)  |
| 7     || FIFO Full Flag (value = 1, when FIFO is full, otherwise 0 ) |
| 8       |       |  Sensor Chip address (default = 0 is set in the firmware) |
| 9       |       | (reserved) |
| 10      |       | Disabled  |
| 11      |       | Disabled  |
| 12      |       | Disabled  |
| 13      |       |  Disabled |
| 14      |       | (reserved) |
| 15      |       | (reserved) |
| 16      |       | (reserved)|



\[  **Data Format**  \]

| ÅgNameÅh  | Description (Value) |
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

