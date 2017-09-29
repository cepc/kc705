// Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2014.3.1 (win64) Build 1056140 Thu Oct 30 17:03:40 MDT 2014
// Date        : Fri Aug 18 20:18:25 2017
// Host        : daq-PC running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               f:/Ryuta/FPGA/KC705_V1.0/kc705_v1.0.srcs/sources_1/ip/coregen_sysclk/coregen_sysclk_stub.v
// Design      : coregen_sysclk
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module coregen_sysclk(clk_in1_p, clk_in1_n, clk_out1)
/* synthesis syn_black_box black_box_pad_pin="clk_in1_p,clk_in1_n,clk_out1" */;
  input clk_in1_p;
  input clk_in1_n;
  output clk_out1;
endmodule
