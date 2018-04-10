`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC
// Engineer: ChesterYang feifei55@mail.ustc.edu.cn
// 
// Create Date: 2018/03/16 10:40:47
// Design Name: 
// Module Name: tlu_handshake
// Project Name: kc705
// Target Devices: 
// Tool Versions: vivado 2017.2
// Description: Achieve handshake with TLU
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module tlu_handshake(
    input CLK,         //the practical maximum frequency of Trigger-Clock would be in region of 10MHz
    input RST_SYS,         //reset, active high
    input TRIGGER_p,   //differential Trigger inputs
    input TRIGGER_n,
//    input TRIGGER_TTL,   //LVTTL TRIGGER INPUT
//    input RESET_p,     //differential Reset inputs. NOT used.
//    input RESET_n,
    output BUSY_p,     //differential Busy outputs
    output BUSY_n,
    output TRIGGER_CLOCK_p,   //differential Trigger-Clock outputs 
    output TRIGGER_CLOCK_n,
    output TRIGGER_VALID,
    output TRIGGER_CNT_VALID,
    output [15:0] TRIGGER_CNT
    );
    
    (*mark_debug = "true"*) wire trigger,trigger_clock;
    (*mark_debug = "true"*) reg busy;
    (*mark_debug = "true"*) reg trigger_clock_en;
//    wire reset;
    reg [15:0] trigger_cnt_tmp;
    reg [4:0] cnt_16;
    
    //Buffers//////////////////////////////////////////
    IBUFDS #(
        .DIFF_TERM("TRUE"),       // Differential Termination
        .IBUF_LOW_PWR("FALSE"),     // Low power="TRUE", Highest performance="FALSE" 
        .IOSTANDARD("LVDS_25")     // Specify the input I/O standard
     ) IBUFDS_inst1 (
        .O(trigger),  // Buffer output
        .I(TRIGGER_p),  // Diff_p buffer input (connect directly to top-level port)
        .IB(TRIGGER_n) // Diff_n buffer input (connect directly to top-level port)
     );

//    IBUF #(
//        .IBUF_LOW_PWR("TRUE"),  // Low power (TRUE) vs. performance (FALSE) setting for referenced I/O standards 
//        .IOSTANDARD("LVTTL")  // Specify the input I/O standard
//    ) IBUF_inst (
//        .O(trigger),     // Buffer output
//        .I(TRIGGER_TTL)      // Buffer input (connect directly to top-level port)
//    ); 
    
//    IBUFDS #(
//         .DIFF_TERM("TRUE"),       // Differential Termination
//         .IBUF_LOW_PWR("FALSE"),     // Low power="TRUE", Highest performance="FALSE" 
//         .IOSTANDARD("LVDS_25")     // Specify the input I/O standard
//      ) IBUFDS_inst2 (
//         .O(reset),  // Buffer output
//         .I(RESET_p),  // Diff_p buffer input (connect directly to top-level port)
//         .IB(RESET_n) // Diff_n buffer input (connect directly to top-level port)
//      );
      
    OBUFDS #(
         .IOSTANDARD("LVDS_25"), // Specify the output I/O standard
         .SLEW("SLOW")           // Specify the output slew rate
      ) OBUFDS_inst1 (
         .O(BUSY_p),     // Diff_p output (connect directly to top-level port)
         .OB(BUSY_n),   // Diff_n output (connect directly to top-level port)
         .I(busy)      // Buffer input 
      );
      
    OBUFDS #(
         .IOSTANDARD("LVDS_25"), // Specify the output I/O standard
         .SLEW("SLOW")           // Specify the output slew rate
      ) OBUFDS_inst2 (
         .O(TRIGGER_CLOCK_p),     // Diff_p output (connect directly to top-level port)
         .OB(TRIGGER_CLOCK_n),   // Diff_n output (connect directly to top-level port)
         .I(trigger_clock)      // Buffer input 
      ); 
    
    //State Machine///////////////////////////////////
    reg [2:0] TLU_STATE,TLU_STATE_N;
    parameter ST_TLU_IDLE = 3'h0,
        ST_TLU_TRIGGER = 3'h1,
        ST_TLU_GAP     = 3'h2,
        ST_TLU_CNT     = 3'h3;
    
    always @(*)
    begin
        TLU_STATE_N = TLU_STATE;
        case (TLU_STATE)
            ST_TLU_IDLE:
                if (trigger)
                    TLU_STATE_N = ST_TLU_TRIGGER;
            
            ST_TLU_TRIGGER:
                TLU_STATE_N = ST_TLU_CNT;
                
            ST_TLU_CNT:
                if (cnt_16 >= 16)
                    TLU_STATE_N = ST_TLU_IDLE;
        endcase
    end      
        
    always @(posedge CLK or posedge RST_SYS)
    begin
        if (RST_SYS)
        begin
            TLU_STATE        <= ST_TLU_IDLE;
            busy             <= 1'b0;
            trigger_clock_en <= 1'b0;
            cnt_16           <= 5'h0;
        end
        else
        begin
            TLU_STATE <= TLU_STATE_N;
            
            if (TLU_STATE_N == ST_TLU_TRIGGER)
                busy <= 1'b1;
            else if (TLU_STATE_N == ST_TLU_IDLE)
                busy <= 1'b0;
            else if (TLU_STATE == ST_TLU_IDLE)
                busy <= 1'b0;
                
            if (TLU_STATE_N == ST_TLU_CNT)
                trigger_clock_en <= 1'b1;
            else if (TLU_STATE_N == ST_TLU_IDLE)
                trigger_clock_en <= 1'b0;
            else if (TLU_STATE == ST_TLU_IDLE)
                trigger_clock_en <= 1'b0;
                
            if (!trigger_clock_en)
                cnt_16 <= 5'h0;
            else
                cnt_16 <= cnt_16 + 1'b1;                 
        end    
    end
    
    always @(negedge CLK)
    begin
        if (trigger_clock_en && cnt_16 == 5'h0)
            trigger_cnt_tmp <= 16'h0 + trigger;
        else if(trigger_clock_en && cnt_16 < 5'd16)
            trigger_cnt_tmp <= trigger_cnt_tmp + (trigger << cnt_16);
    end
          
    //Assignments///////////////////////////////////////////
    assign trigger_clock = trigger_clock_en ? CLK : 1'b0;     
    assign TRIGGER_VALID = (TLU_STATE != ST_TLU_IDLE);  
    assign TRIGGER_CNT_VALID = (TLU_STATE_N == ST_TLU_IDLE) && (TLU_STATE != ST_TLU_IDLE);
    assign TRIGGER_CNT = TRIGGER_CNT_VALID ? trigger_cnt_tmp : TRIGGER_CNT;
    
endmodule
