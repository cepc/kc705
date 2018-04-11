`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC
// Engineer: ChesterYang feifei55@mail.ustc.edu.cn
// 
// Create Date: 2018/04/03 10:45:27
// Design Name: 
// Module Name: circle_buffer_read_write_ctrl
// Project Name: kc705
// Target Devices: 
// Tool Versions: vivado 2017.2
// Description: Control the 32*48200 circle buffer 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module circle_buffer_read_write_ctrl(
    input CLK,
    input RST,
    input TRIGGER,
    input [15:0] TRIGGER_CNT,
    input FRAME_END,
    input BUFFER_WREN,
	input [31:0] BUFFER_DOUT,
    output reg [15:0] BUFFER_ADDRA,
    output reg [15:0] BUFFER_ADDRB,
    output reg FIFO_WREN,
	output [31:0] FIFO_DIN
    );

(*mark_debug = "true"*) reg [15:0] tmp_addra;
(*mark_debug = "true"*) reg [31:0] tmp_fifo_din;
(*mark_debug = "true"*) reg [4:0] seq_order;
(*mark_debug = "true"*) reg [8:0] inframe_cnt;
(*mark_debug = "true"*) reg flag_buf_rd;

(*mark_debug = "true"*) reg [3:0] BUFFER_STATE,BUFFER_STATE_N;
parameter ST_BUFFER_IDLE = 4'h0,
	  ST_BUFFER_TRIGGER = 4'h1,
	  ST_BUFFER_LATENCY = 4'h2,
	  ST_BUFFER_READ    = 4'h3,
	  ST_BUFFER_TRAILER = 4'h4;

always @(*)
begin
	BUFFER_STATE_N = BUFFER_STATE;
	case(BUFFER_STATE)
		ST_BUFFER_IDLE:
			if(TRIGGER && BUFFER_WREN)
				BUFFER_STATE_N = ST_BUFFER_TRIGGER;

		ST_BUFFER_TRIGGER:
			if(FRAME_END)
				BUFFER_STATE_N = ST_BUFFER_LATENCY;
				
		ST_BUFFER_LATENCY:
		    BUFFER_STATE_N = ST_BUFFER_READ;

		ST_BUFFER_READ:
			if(BUFFER_ADDRB == tmp_addra + 1'b1)             //go foward n frames, tmp_addra + 1'b1 + n*484
				BUFFER_STATE_N = ST_BUFFER_TRAILER;

		ST_BUFFER_TRAILER:
			BUFFER_STATE_N = ST_BUFFER_IDLE;
	endcase
end			

always @(posedge CLK or posedge RST)
begin
	if(RST)
	begin
		BUFFER_STATE <= ST_BUFFER_IDLE;
		BUFFER_ADDRA <= 16'h0;
		BUFFER_ADDRB <= 16'h0;
		FIFO_WREN    <= 1'b0;
		tmp_fifo_din <= 32'h0;
		tmp_addra    <= 16'h0;
		flag_buf_rd  <= 1'b0;
		seq_order    <= 5'h0;
		inframe_cnt  <= 9'h0;
	end
	else
	begin
		BUFFER_STATE <= BUFFER_STATE_N;

		if(BUFFER_WREN)
            BUFFER_ADDRA <= BUFFER_ADDRA + 1'b1;       //increase addra by 1 when writing into buffer

		if(BUFFER_STATE == ST_BUFFER_TRIGGER && FRAME_END)
		begin
			tmp_addra    <= BUFFER_ADDRA;
			BUFFER_ADDRB <= BUFFER_ADDRA - 16'd2419;   //go back n frames  :   n*484-1=2409 when n=5
		end
		else if(BUFFER_STATE == ST_BUFFER_LATENCY)
		    BUFFER_ADDRB <= BUFFER_ADDRB + 1'b1;
		else if(BUFFER_STATE == ST_BUFFER_READ) 
			BUFFER_ADDRB <= BUFFER_ADDRB + 1'b1;
			
	    if(BUFFER_STATE == ST_BUFFER_IDLE)
	    begin
	        seq_order   <= 5'h0;
	        inframe_cnt <= 9'h0;
	    end
	    else if(BUFFER_STATE == ST_BUFFER_READ && inframe_cnt == 9'd483)
	    begin
	        inframe_cnt <= 9'h0;
	        seq_order   <= seq_order + 1'b1;
	    end
	    else if(BUFFER_STATE == ST_BUFFER_READ)
	        inframe_cnt <= inframe_cnt + 1'b1;

		if(BUFFER_STATE == ST_BUFFER_READ)
		begin
			FIFO_WREN     <= 1'b1;
			if(inframe_cnt == 9'h2)
			begin
			    flag_buf_rd <= 1'b0;
			    tmp_fifo_din <= {TRIGGER_CNT, 11'h0, seq_order};
			end
			else
			    flag_buf_rd <= 1'b1;
		end
		else if(BUFFER_STATE == ST_BUFFER_TRAILER)
		begin
			FIFO_WREN     <= 1'b0;                    
			flag_buf_rd   <= 1'b0;
		end
		else  
			FIFO_WREN   <= 1'b0;
	end
end

assign FIFO_DIN = flag_buf_rd ? BUFFER_DOUT : tmp_fifo_din;
    
endmodule
