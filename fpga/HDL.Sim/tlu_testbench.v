`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/03/16 11:54:07
// Design Name: 
// Module Name: tlu_testbench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module tlu_testbench(

    );
    
    reg clk;
    reg trigger_p,trigger_n;
    reg busy_p,trigger_clk_p;
    reg trigger_valid,cnt_valid;
    reg [15:0] cnt;
    
    initial
    begin
        clk = 1'b0;
        trigger_p = 1'b0;
        trigger_n = 1'b1;
        #1000
        trigger_p = 1'b1;
        trigger_n = 1'b0;
        #2000
        trigger_p = 1'b0;
        trigger_n = 1'b1;
    end
    
    always #100 clk = ~clk;
    
    tlu_handshake tlu_handshake_inst (
        .CLK(clk),
        .RST_SYS(1'b0),
        .TRIGGER_p(trigger_p),
        .TRIGGER_n(trigger_n),
        .RESET_p(1'b0),
        .RESET_n(1'b1),
        .BUSY_p(busy_p),
        .BUSY_n(),
        .TRIGGER_CLOCK_p(trigger_clk_p),
        .TRIGGER_CLOCK_n(),
        .TRIGGER_VALID(trigger_valid),
        .TRIGGER_CNT_VALID(cnt_valid),
        .TRIGGER_CNT(cnt)
        );       
    
endmodule
