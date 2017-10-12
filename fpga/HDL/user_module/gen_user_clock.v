module gen_user_clock (
    input  RST,       // 100MHz reset in 
    input  CLK_IN,    // 100MHz clock in
    output CLK_OUT,   //   2MHz clock out
    output RST_2M_OUT //   2MHz reset out (5 clock cycles) 
);

// Generate 2MHz user clock
reg clk_2M = 1'h0;
reg [7:0] cnt = 8'h0;
always @( posedge CLK_IN ) 
begin
    if ( RST )           
        cnt <= 8'h0;
    else if ( cnt == 8'd24 )
    begin
        cnt <= cnt + 1'h1;
        clk_2M <= 1'h1;
    end       
    else if ( cnt == 8'd49 )
    begin
		cnt <= 8'h0;
		clk_2M <= 1'h0;
    end
	else
		cnt <= cnt + 1'h1;
end

assign CLK_OUT = clk_2M;
    
 // Generate Reset Pulse (in 5 clock cycles) for 2MHz user clock 
 reg [9:0] rst_cnt_2M = 10'h0; 
 reg       rst_2M = 1'b0;
 always @( posedge CLK_IN ) 
 begin
     if ( RST )   
     begin
        rst_cnt_2M <= 10'h0;
        rst_2M     <= 1'b1;
     end
     else if ( rst_cnt_2M < 10'd250 )    // (100MHz * 50clock)=2MHz * 5cycles 
     begin
        rst_cnt_2M <= rst_cnt_2M + 1'h1;
        rst_2M     <= 1'b1;
     end
     else
        rst_2M     <= 1'b0;
end

assign RST_2M_OUT = rst_2M;

endmodule