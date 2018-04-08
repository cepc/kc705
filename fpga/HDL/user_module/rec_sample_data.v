module rec_sample_data (
    input          CLK,
    input          CNVCLK,
    input          RST,
    input          SR_OUT_IN,
    output         SR_OUT_OUT,
    input  [15:0]  DATA_IN,
    output [15:0]  DATA_OUT
);

// Sampling data & sr_out at the negative edge of cnvclk 
// Additional one clock delay for SR_OUT 
(* mark_debug = "true" *) reg [15:0] data_cnvclk, dff_data_cnvclk;
reg srout_cnvclk, dff_srout_cnvclk, dffdff_srout_cnvclk;
always @( negedge CNVCLK )
begin
    data_cnvclk <= DATA_IN;
    srout_cnvclk <= SR_OUT_IN;
    
    dff_data_cnvclk     <= data_cnvclk;
    dff_srout_cnvclk    <= srout_cnvclk;
    dffdff_srout_cnvclk <= dff_srout_cnvclk;
end


// Samling data & sr_out at the positive edge of clk
(* mark_debug = "true" *) reg [15:0] data_clk, dff_data_clk;
reg srout_clk, dff_srout_clk;
always @( posedge CLK )
begin
    data_clk <= dff_data_cnvclk;
    srout_clk <= dffdff_srout_cnvclk;
    
    dff_data_clk <= data_clk;
    dff_srout_clk <= srout_clk;
end

assign DATA_OUT = dff_data_clk;
assign SR_OUT_OUT = dff_srout_clk;


endmodule

