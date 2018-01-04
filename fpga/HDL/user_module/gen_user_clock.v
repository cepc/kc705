module gen_user_clock (
    input  CLK_IN,       //  100MHz clock in
    input  CLK_40M_IN,   //   40MHz clock in
    input  RST_IN,       //  100MHz reset in (5 clock cycles)  
    output CLK_2M_OUT,   //    2MHz clock out
    output RST_2M_OUT,   //    2MHz reset out (5 clock cycles) 
    output CLK_4M_OUT,   //    4MHz clock out
    output RST_4M_OUT    //    4MHz reset out (5 clock cycles) 
);

// Generate 2MHz user clock
reg clk_2M = 1'h0;
reg [7:0] cnt = 8'h0;
always @( posedge CLK_IN )  // 100 MHz clock 
begin
    if ( RST_IN )     
    begin      
        cnt <= 8'h0;
        clk_2M <= 1'h0;
    end
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

assign CLK_2M_OUT = clk_2M;
    
    
// Generate Reset Pulse (in 5 clock cycles) for 2MHz user clock 
reg [9:0] rst_cnt_2M = 10'h0; 
reg       rst_2M = 1'b0;
always @( posedge CLK_IN )  // 100 MHz clock 
begin
    if ( RST_IN )   
    begin
       rst_cnt_2M <= 10'h0;
       rst_2M     <= 1'b1;
    end
    else if ( rst_cnt_2M < 10'd250 )    // (100MHz * 250clocks)=2MHz * 5clocks 
    begin
       rst_cnt_2M <= rst_cnt_2M + 1'h1;
       rst_2M     <= 1'b1;
    end
    else
       rst_2M     <= 1'b0;
end

assign RST_2M_OUT = rst_2M;


// Generate 4MHz user clock
reg clk_4M = 1'h0;
reg [7:0] cnt_4M = 8'h0;
always @( posedge CLK_40M_IN )  // 40 MHz clock 
begin
    if ( RST_IN )          
    begin 
        cnt_4M <= 8'h0;
        clk_4M <= 1'h0;
    end
    else if ( cnt_4M == 8'd4 )
    begin
        cnt_4M <= cnt_4M + 1'h1;
        clk_4M <= 1'h1;
    end       
    else if ( cnt_4M == 8'd9 )
    begin
		cnt_4M <= 8'h0;
		clk_4M <= 1'h0;
    end
	else
		cnt_4M <= cnt_4M + 1'h1;
end

assign CLK_4M_OUT = clk_4M;


// Generate Reset Pulse (in 5 clock cycles) for 4MHz user clock 
reg [9:0] rst_cnt_4M = 10'h0; 
reg       rst_4M = 1'b0;
always @( posedge CLK_IN )  // 100 MHz clock 
begin
    if ( RST_IN )   
    begin
       rst_cnt_4M <= 10'h0;
       rst_4M     <= 1'b1;
    end
    else if ( rst_cnt_4M < 10'd125 )    // (100MHz * 125clock)=4MHz * 5clocks 
    begin
       rst_cnt_4M <= rst_cnt_4M + 1'h1;
       rst_4M     <= 1'b1;
    end
    else
       rst_4M     <= 1'b0;
end

assign RST_4M_OUT = rst_4M;



endmodule