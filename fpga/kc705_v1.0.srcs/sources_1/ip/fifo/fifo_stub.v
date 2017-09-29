// Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2014.3.1 (win64) Build 1056140 Thu Oct 30 17:03:40 MDT 2014
// Date        : Fri Aug 18 20:42:56 2017
// Host        : daq-PC running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               f:/Ryuta/FPGA/KC705_V1.0/kc705_v1.0.srcs/sources_1/ip/fifo/fifo_stub.v
// Design      : fifo
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "fifo_generator_v12_0,Vivado 2014.3.1" *)
module fifo(clk, rst, din, wr_en, rd_en, dout, full, empty, data_count, prog_full, prog_empty)
/* synthesis syn_black_box black_box_pad_pin="clk,rst,din[15:0],wr_en,rd_en,dout[15:0],full,empty,data_count[9:0],prog_full,prog_empty" */;
  input clk;
  input rst;
  input [15:0]din;
  input wr_en;
  input rd_en;
  output [15:0]dout;
  output full;
  output empty;
  output [9:0]data_count;
  output prog_full;
  output prog_empty;
endmodule
