# KC705@Firmware Information
Kintex-7 FPGA KC705

#### Version 1.1: Last update ( 02/05/2018 )


* Command Line access (using xillybus executable)

Just check: 
> hexdump.exe ?C ?v ?n 32 \\.\xillybus_mem_8

Write register:
> memwrite.exe  \\.\xillybus_mem_8  [address]  [value]

Read register:
> memread.exe  \\.\xillybus_mem_8  [address]


|header1|header2|header3|

