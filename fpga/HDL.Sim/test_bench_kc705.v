`timescale 1ns/1ps

module test_bench_kc705;

localparam CLKNUM    = 1000;
localparam STEP_PCIe = 8; 
localparam STEP_FPGA = 5; 
localparam STEP      = 1.0;

reg clkoutn_n, clkoutn_p;
wire cnv, sck_n, sck_p;
wire cnv_p, cnv_n;
reg sdo10_n, sdo11_n, sdo12_n, sdo13_n, sdo14_n, sdo15_n, sdo16_n;
reg sdo10_p, sdo11_p, sdo12_p, sdo13_p, sdo14_p, sdo15_p, sdo16_p; 
reg sdo1_n, sdo2_n, sdo3_n, sdo4_n, sdo5_n, sdo6_n, sdo7_n, sdo8_n, sdo9_n;
reg sdo1_p, sdo2_p, sdo3_p, sdo4_p, sdo5_p, sdo6_p, sdo7_p, sdo8_p, sdo9_p;
reg sr_top_out_c;
wire clk_div;
wire [6:0] d_a;
wire oe;
reg rst, start;
wire sync; 


// For Xillybus
reg SYSCLK_P;
reg SYSCLK_N; 
reg PCIE_PERST_B_LS;
reg PCIE_REFCLK_N;
reg PCIE_REFCLK_P;
reg [7:0] PCIE_RX_N, PCIE_RX_P;

wire [3:0] GPIO_LED;
wire [7:0] PCIE_TX_N, PCIE_TX_P;

// For TLU
reg TRIGGER_p;
reg TRIGGER_n;
wire BUSY_p;
wire BUSY_n;
wire TRIGGER_CLOCK_p;
wire TRIGGER_CLOCK_n;

    
kc705 kc705_ins(
    .d_A           (  d_a          ), 
    .SDO10_n       (  sdo10_n      ),
    .SDO10_p       (  sdo10_p      ),
    .SDO11_n       (  sdo11_n      ),
    .SDO11_p       (  sdo11_p      ),
    .SDO12_n       (  sdo12_n      ),
    .SDO12_p       (  sdo12_p      ),
    .SDO13_n       (  sdo13_n      ),
    .SDO13_p       (  sdo13_p      ),
    .SDO14_n       (  sdo14_n      ),
    .SDO14_p       (  sdo14_p      ),
    .SDO15_n       (  sdo15_n      ),
    .SDO15_p       (  sdo15_p      ),
    .SDO16_n       (  sdo16_n      ),
    .SDO16_p       (  sdo16_p      ),
    .SDO1_n        (  sdo1_n       ),
    .SDO1_p        (  sdo1_p       ),
    .SDO2_n        (  sdo2_n       ),
    .SDO2_p        (  sdo2_p       ),
    .SDO3_n        (  sdo3_n       ),
    .SDO3_p        (  sdo3_p       ),
    .SDO4_n        (  sdo4_n       ),
    .SDO4_p        (  sdo4_p       ),
    .SDO5_n        (  sdo5_n       ),
    .SDO5_p        (  sdo5_p       ),
    .SDO6_n        (  sdo6_n       ),
    .SDO6_p        (  sdo6_p       ),
    .SDO7_n        (  sdo7_n       ),
    .SDO7_p        (  sdo7_p       ),
    .SDO8_n        (  sdo8_n       ),
    .SDO8_p        (  sdo8_p       ),
    .SDO9_n        (  sdo9_n       ),
    .SDO9_p        (  sdo9_p       ),
    .CLKOUTN_p     (  clkoutn_p    ), 
    .CLKOUTN_n     (  clkoutn_n    ), 
    .rst           (  rst          ), 
    .start         (  start        ), 
    .CNV_n         (  cnv_n        ),  
    .CNV_p         (  cnv_p        ),  
    .SCK_p         (  sck_p        ), 
    .SCK_n         (  sck_n        ), 
    .SR_Top_out_c  (  sr_top_out_c ), 
    .oe            (  oe           ), 
    .clk_div       (  clk_div      ), 
    .sync          (  sync         ),
    .sysclk_p          (  SYSCLK_P        ),
    .sysclk_n          (  SYSCLK_N        ),
    .PCIE_PERST_B_LS   (  PCIE_PERST_B_LS ),
    .PCIE_REFCLK_N     (  PCIE_REFCLK_N   ),
    .PCIE_REFCLK_P     (  PCIE_REFCLK_P   ),
    .PCIE_RX_N         (  PCIE_RX_N       ),
    .PCIE_RX_P         (  PCIE_RX_P       ),
    .GPIO_LED          (  GPIO_LED        ), 
    .PCIE_TX_N         (  PCIE_TX_N       ),
    .PCIE_TX_P         (  PCIE_TX_P       ),
    .TRIGGER_p         (  TRIGGER_p       ),  
    .TRIGGER_n         (  TRIGGER_n       ),  
    .BUSY_p            (  BUSY_p          ),  
    .BUSY_n            (  BUSY_n          ),  
    .TRIGGER_CLOCK_p   (  TRIGGER_CLOCK_p ),  
    .TRIGGER_CLOCK_n   (  TRIGGER_CLOCK_n )   
);

    
always begin
  PCIE_REFCLK_P = 0; PCIE_REFCLK_N = 1; #(STEP_PCIe/2);  // 125MHz FPGA-PCIE clock 
  PCIE_REFCLK_P = 1; PCIE_REFCLK_N = 0; #(STEP_PCIe/2);
end
    
always begin
  clkoutn_p = 0; clkoutn_n = 1; #(2.500);    // 200MHz differential clock in
  clkoutn_p = 1; clkoutn_n = 0; #(2.500);    // 200MHz differential clock in
end    

always begin
  SYSCLK_P = 0; SYSCLK_N = 1; #(2.500);    // 200MHz differential clock in
  SYSCLK_P = 1; SYSCLK_N = 0; #(2.500);    // 200MHz differential clock in
end
    
reg CLK100;
always begin
  CLK100 = 0;  #(5.000);    // 100MHz clock in
  CLK100 = 1;  #(5.000);    
end
    
assign kc705_ins.clk100M = CLK100;
assign kc705_ins.clk100M_2 = CLK100;

reg CLK200;
always begin
  CLK200 = 0;  #(2.500);    // 200MHz clock in
  CLK200 = 1;  #(2.500);    
end
assign kc705_ins.clk200M = CLK200;

reg CNVCLK;
always begin
  CNVCLK = 0;  #(400.0);    // conversion clock
  CNVCLK = 1;  #(100.0);    // conversion clock
end
assign kc705_ins.cnvclk = CNVCLK;

reg WREN;
reg EN;   // Enable signal : without it, no read/write
reg [4:0] ADDR;
reg [7:0] DATA;
reg [7:0] DATA_R;

reg [31:0] DIN_FIFO;
reg RD_EN_FIFO, WR_EN_FIFO;

reg TRIGGER_VALID;

initial begin

    #(STEP*2000); 
    #(STEP*50) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00001; DATA = 8'b00001111;  // RESET Signal
    #(STEP*50) EN = 1'h1;  WREN = 1'h0;

    #(STEP*8000); 
    
    // Start 
    #(STEP*50) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00011; DATA = 8'b00001111;  // Start Signal
    #(STEP*50) EN = 1'h1;  WREN = 1'h0;
    #(STEP*5) EN = 1'h0;
    

    #(STEP*CLKNUM);
end

assign kc705_ins.user_w_mem_8_wren = WREN;  // Behavior simulation is ok, but Post-imple, the net is gone.
//assign kc705_ins.user_mem_8_write_en = WREN;

assign kc705_ins.user_mem_8_addr = ADDR;
assign kc705_ins.user_w_mem_8_data = DATA;

assign kc705_ins.user_r_mem_8_rden = EN;     // Behavior simulation is ok, but Post-imple, the net is gone.
//assign kc705_ins.user_mem_8_enable = EN;     // Behavior simulation is ok, but Post-imple, the net is gone.


endmodule

