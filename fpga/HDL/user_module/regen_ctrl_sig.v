module regenerate_ctrl_signal (
    input          CLK_IN,
    input          CLK2M_IN,
    input          RST_IN,
    input          CNVCLK_IN,
    input  [5:0]   NSTEP1,
    input  [5:0]   NSTEP2,
    output         CLK2M_OUT,
    output         RST2M_OUT,
    output         CNVCLK_OUT
);


// For CLK2M & RST2M
// Generate 2MHz user clock
reg clk_2M = 1'h0;
reg [5:0] cnt = 6'h0;
reg [5:0] int_cnt = 6'h0;
always @( posedge CLK_IN )  // 100 MHz clock 
begin
    if ( RST_IN )     
    begin      
        cnt     <= 8'h0;
        int_cnt <= 6'h0;
        clk_2M  <= 1'h0;
    end
    else if ( cnt == 8'h0 ) begin
        if ( (int_cnt < NSTEP1) && (NSTEP1 != 0) )
            int_cnt <= int_cnt + 6'h1;
        else 
            cnt <= 1'h1;
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

assign CLK2M_OUT = clk_2M;


// Generate Reset Pulse (in 5 clock cycles) for 2MHz user clock 
reg [9:0] rst_cnt = 10'h0; 
reg [7:0] rst_int_cnt = 6'h0;
reg       rst_2M = 1'b0;
always @( posedge CLK_IN )  // 100 MHz clock 
begin
    if ( RST_IN )   
    begin
       rst_cnt  <= 10'h0;
       rst_int_cnt  <= 6'h0;
    end
    else if ( rst_cnt == 8'h0 ) begin
            if ( (rst_int_cnt < NSTEP1) && (NSTEP1 != 0) )
                rst_int_cnt <= rst_int_cnt + 6'h1;
            else begin
                rst_cnt <= 1'h1;
                rst_2M <= 1'b1;
            end
    end 
    else if ( rst_cnt < 10'd250 ) begin   // (100MHz * 250clocks)=2MHz * 5clocks 
       rst_cnt <= rst_cnt + 1'h1;
       rst_2M  <= 1'b1;
    end
    else
       rst_2M     <= 1'b0;
end

assign RST2M_OUT = rst_2M;


// For CNVCLK
reg cnvclk_dff1;
reg cnvclk_dff2;
reg cnvclk_dff3;
always @( posedge CLK_IN )
begin
    cnvclk_dff1 <= CNVCLK_IN;
    cnvclk_dff2 <= cnvclk_dff1;
    cnvclk_dff3 <= cnvclk_dff2;
end

reg       cnvclk_add = 1'b0;
reg [7:0] cnt_cnv = 8'hff;
always @( posedge CLK_IN )
begin
    if ( cnvclk_dff1 == 1'b0 && cnvclk_dff2 == 1'b1 ) begin
        cnt_cnv <= 8'h0;
        cnvclk_add <= 1'b1;
    end
    else if ( cnt_cnv == NSTEP2 ) begin
        cnvclk_add <= 1'b0;
        cnt_cnv <= cnt_cnv;
    end
    else begin
        cnt_cnv <= cnt_cnv + 8'h1;
        cnvclk_add <= 1'b1;
    end
end

assign CNVCLK_OUT = ( cnvclk_dff3 || cnvclk_add );    // Enlarged CNVCLK
    
    
    
endmodule
