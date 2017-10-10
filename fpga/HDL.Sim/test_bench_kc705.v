`timescale 1ns/1ps

module test_bench_kc705;

localparam CLKNUM    = 1000;
localparam STEP_PCIe = 8; 
localparam STEP_FPGA = 5; 
//localparam STEP      = 10;
//localparam STEP      = 8;
localparam STEP      = 3.2;

reg SYSCLK_P;
reg SYSCLK_N; 
reg PCIE_PERST_B_LS;
reg PCIE_REFCLK_N;
reg PCIE_REFCLK_P;
reg [7:0] PCIE_RX_N, PCIE_RX_P;

wire [3:0] GPIO_LED;
wire [7:0] PCIE_TX_N, PCIE_TX_P;
    
kc705 kc705_ins(
    .SYSCLK_P          (  SYSCLK_P        ),
    .SYSCLK_N          (  SYSCLK_N        ),
    .PCIE_PERST_B_LS   (  PCIE_PERST_B_LS ),
    .PCIE_REFCLK_N     (  PCIE_REFCLK_N   ),
    .PCIE_REFCLK_P     (  PCIE_REFCLK_P   ),
    .PCIE_RX_N         (  PCIE_RX_N       ),
    .PCIE_RX_P         (  PCIE_RX_P       ),
    .GPIO_LED          (  GPIO_LED        ), 
    .PCIE_TX_N         (  PCIE_TX_N       ),
    .PCIE_TX_P         (  PCIE_TX_P       )
);

    
always begin
  PCIE_REFCLK_P = 0; PCIE_REFCLK_N = 1; #(STEP_PCIe/2);
  PCIE_REFCLK_P = 1; PCIE_REFCLK_N = 0; #(STEP_PCIe/2);
end
    
    
always begin
//  SYSTEM_CLOCK_P = 0; SYSTEM_CLOCK_N = 1; #(STEP_FPGA/2);
//  SYSTEM_CLOCK_P = 1; SYSTEM_CLOCK_N = 0; #(STEP_FPGA/2);
  SYSCLK_P = 0; SYSCLK_N = 1; #(2.500);
  SYSCLK_P = 1; SYSCLK_N = 0; #(2.500);
end
    
    
reg WREN;
reg EN;   // Enable signal : without it, no read/write
reg [4:0] ADDR;
reg [7:0] DATA;
reg [7:0] DATA_R;

reg [31:0] DIN_FIFO;
reg RD_EN_FIFO, WR_EN_FIFO;


initial begin
    #(STEP*500); 
    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00001; DATA = 8'b00001111;  // RESET Signal
    #(STEP*5) EN = 1'h1;  WREN = 1'h0;
//    #(STEP*5) EN = 1'h0;

    #(STEP*1000); 
    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00001; DATA = 8'b00001111;  // RESET Signal
    #(STEP*5) EN = 1'h1;  WREN = 1'h0;
    
// Memory TEST ::  OK !!        
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00001; DATA = 8'b00000001;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00010; DATA = 8'b00000010;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00011; DATA = 8'b00000011;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00100; DATA = 8'b00000100;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00101; DATA = 8'b00000101;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00110; DATA = 8'b00000110;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00111; DATA = 8'b00000111;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b01000; DATA = 8'b00001000;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b01001; DATA = 8'b00001001;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h1; ADDR = 5'b00001; DATA = 8'b00001111;
//    #(STEP*5) EN = 1'h1;  WREN = 1'h0;
    
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00001; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00010; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00011; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00100; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00101; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00110; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b00111; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b01000; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h1; ADDR = 5'b01001; DATA_R = kc705_ins.user_r_mem_8_data;
//    #(STEP*5) EN = 1'h0;
         
// FIFO TEST ::  OK !!
//    #(STEP*1) DIN_FIFO = 32'h0000fff0; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff1; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff2; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff3; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff4; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff5; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff6; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff7; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff8; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
//    #(STEP*1) DIN_FIFO = 32'h0000fff9; WR_EN_FIFO = 1'b1; RD_EN_FIFO = 1'b0;
    
//    #(STEP*10) DIN_FIFO = 32'h0000f0f0; WR_EN_FIFO = 1'b0; RD_EN_FIFO = 1'b1;
    
    #(STEP*CLKNUM);
end

assign kc705_ins.user_w_mem_8_wren = WREN;  // Behavior simulation is ok, but Post-imple, the net is gone.
//assign kc705_ins.user_mem_8_write_en = WREN;

assign kc705_ins.user_mem_8_addr = ADDR;
assign kc705_ins.user_w_mem_8_data = DATA;

assign kc705_ins.user_r_mem_8_rden = EN;     // Behavior simulation is ok, but Post-imple, the net is gone.
//assign kc705_ins.user_mem_8_enable = EN;     // Behavior simulation is ok, but Post-imple, the net is gone.

//assign kc705_ins.din_async_fifo = DIN_FIFO;
//assign kc705_ins.wr_en = WR_EN_FIFO;
//assign kc705_ins.user_r_read_32_rden = RD_EN_FIFO;


endmodule

