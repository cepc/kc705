// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.2 (win64) Build 1909853 Thu Jun 15 18:39:09 MDT 2017
// Date        : Tue Jan 23 16:19:37 2018
// Host        : PC-201707151623 running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -mode pin_planning CPS_ReadOut_TOP_stub.v
// Design      : CPS_ReadOut_TOP
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg900-2
// --------------------------------------------------------------------------------
module CPS_ReadOut_TOP(DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7, DATA8, DATA9, DATA10, DATA11, DATA12, DATA13, DATA14, DATA15, DATA16, addr_sel, d_A, sysclk_p, sysclk_n, rst, start, SDO1_p, SDO1_n, SDO2_p, SDO2_n, SDO3_p, SDO3_n, SDO4_p, SDO4_n, SDO5_p, SDO5_n, SDO6_p, SDO6_n, SDO7_p, SDO7_n, SDO8_p, SDO8_n, SDO9_p, SDO9_n, SDO10_p, SDO10_n, SDO11_p, SDO11_n, SDO12_p, SDO12_n, SDO13_p, SDO13_n, SDO14_p, SDO14_n, SDO15_p, SDO15_n, SDO16_p, SDO16_n, CLKOUTN_p, CLKOUTN_n, CNV_p, CNV_n, SCK_p, SCK_n, SR_Top_out, clk, cnvclk, SR_Top_out_c, oe, clk_div, sync);
  output [15:0] DATA1;
  output [15:0] DATA2;
  output [15:0] DATA3;
  output [15:0] DATA4;
  output [15:0] DATA5;
  output [15:0] DATA6;
  output [15:0] DATA7;
  output [15:0] DATA8;
  output [15:0] DATA9;
  output [15:0] DATA10;
  output [15:0] DATA11;
  output [15:0] DATA12;
  output [15:0] DATA13;
  output [15:0] DATA14;
  output [15:0] DATA15;
  output [15:0] DATA16;
  input [3:0] addr_sel;
  output [6:0] d_A;
  input sysclk_p;
  input sysclk_n;
  input rst;
  input start;
  input SDO1_p;
  input SDO1_n;
  input SDO2_p;
  input SDO2_n;
  input SDO3_p;
  input SDO3_n;
  input SDO4_p;
  input SDO4_n;
  input SDO5_p;
  input SDO5_n;
  input SDO6_p;
  input SDO6_n;
  input SDO7_p;
  input SDO7_n;
  input SDO8_p;
  input SDO8_n;
  input SDO9_p;
  input SDO9_n;
  input SDO10_p;
  input SDO10_n;
  input SDO11_p;
  input SDO11_n;
  input SDO12_p;
  input SDO12_n;
  input SDO13_p;
  input SDO13_n;
  input SDO14_p;
  input SDO14_n;
  input SDO15_p;
  input SDO15_n;
  input SDO16_p;
  input SDO16_n;
  input CLKOUTN_p;
  input CLKOUTN_n;
  output CNV_p;
  output CNV_n;
  output SCK_p;
  output SCK_n;
  output SR_Top_out;
  output clk;
  output cnvclk;
  input SR_Top_out_c;
  output oe;
  output clk_div;
  output sync;

endmodule
