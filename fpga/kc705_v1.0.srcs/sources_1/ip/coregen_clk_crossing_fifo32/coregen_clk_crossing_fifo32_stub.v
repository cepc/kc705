// Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2014.3.1 (win64) Build 1056140 Thu Oct 30 17:03:40 MDT 2014
// Date        : Thu Sep 21 11:18:09 2017
// Host        : daq-PC running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               C:/Users/daq/Desktop/Active_FPGA/KC705_V1.1/kc705_v1.0.srcs/sources_1/ip/coregen_clk_crossing_fifo32/coregen_clk_crossing_fifo32_stub.v
// Design      : coregen_clk_crossing_fifo32
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "fifo_generator_v12_0,Vivado 2014.3.1" *)
module coregen_clk_crossing_fifo32(rst, wr_clk, rd_clk, din, wr_en, rd_en, dout, full, empty, rd_data_count, wr_data_count)
/* synthesis syn_black_box black_box_pad_pin="rst,wr_clk,rd_clk,din[31:0],wr_en,rd_en,dout[31:0],full,empty,rd_data_count[14:0],wr_data_count[14:0]" */;
  input rst;
  input wr_clk;
  input rd_clk;
  input [31:0]din;
  input wr_en;
  input rd_en;
  output [31:0]dout;
  output full;
  output empty;
  output [14:0]rd_data_count;
  output [14:0]wr_data_count;
endmodule
