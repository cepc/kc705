module adjust_cnvclk_timing (
    input          CLK_IN,      // 100MHz Clock 
    input          CNVCLK_IN,
    input  [3:0]   DELAY,
    input          SET_PARAM, 
    output         CNVCLK_OUT
);

// Timing delay : It can be changed when "set" signal arrives.
reg [3:0] NSTEP = 4'h0;
always @( posedge CLK_IN )     // 100MHz clock to samle CNVCLK
begin
    if ( SET_PARAM == 1'b1 )
        NSTEP <= DELAY;
    else
        NSTEP <= NSTEP;
end


// Make 100MHz edge of CNVCLK
reg cnvclk_dff1;
reg cnvclk_dff2;
reg cnvclk_dff3;
always @( posedge CLK_IN )     // 100MHz clock to samle CNVCLK
begin
    cnvclk_dff1 <= CNVCLK_IN;
    cnvclk_dff2 <= cnvclk_dff1;
    cnvclk_dff3 <= cnvclk_dff2;
end


// Adjust CNVCLK
reg       cnvclk_add = 1'b0;
reg [3:0] cnt_cnv = 4'hf;
always @( posedge CLK_IN )
begin
    if ( cnvclk_dff1 == 1'b0 && cnvclk_dff2 == 1'b1 ) begin
        cnt_cnv <= 4'h0;
        cnvclk_add <= 1'b1;
    end
    else if ( cnt_cnv == NSTEP ) begin
        cnvclk_add <= 1'b0;
        cnt_cnv <= cnt_cnv;
    end
    else begin
        cnt_cnv <= cnt_cnv + 4'h1;
        cnvclk_add <= 1'b1;
    end
end

assign CNVCLK_OUT = ( cnvclk_dff3 || cnvclk_add );    // Enlarged CNVCLK
    
    
    
endmodule
