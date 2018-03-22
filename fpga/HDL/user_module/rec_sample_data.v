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

// Check SR_OUT : -- maybe better to separate it. (it is possible that the compiler alredy seprate it)
// if SR_OUT is always "LOW", no "SR_OUT" status. 
// if SR_OUT is always "High", erase "SR_OUT" but "SR_OUT" abnormal status.
// otherwise. it is fine.

// For the moment, only take an action when the "SR_OUT" is always "High"  
//reg [7:0] srout_cnt = 8'h0;
//always @( posedge CLK )
//begin
//    if( dff_srout_clk == 1'b1 ) begin 
//        if ( srout_cnt > 8'd200 )     // if count = 200, stop increment to prevent loop-back.
//            srout_cnt <= srout_cnt;
//        else
//            srout_cnt <= srout_cnt + 10'h1;
//        end
//    else
//        srout_cnt <= 8'h0;            
//end

         
// Clock Crossing by Using FIFO
//wire [17:0] din, dout;
//wire wr_en_High = 1'b1;
//wire rd_en_High = 1'b1;
//wire full, empty;
//wire cnvclk_inv = ~CNVCLK;
//assign din  = {1'b1, dffdff_srout_cnvclk, dff_data_cnvclk};
// 
//coregen_clk2M_crossing_fifo  clk2M_crossing_fifo (
//        .wr_clk  (  cnvclk_inv ),    // input wire wr_clk
//        .wr_clk  (  CNVCLK     ),    // input wire wr_clk
//        .rd_clk  (  CLK        ),    // input wire rd_clk
//        .din     (  din        ),    // input wire [17 : 0] din
//        .wr_en   (  wr_en_High ),    // input wire wr_en
//        .rd_en   (  rd_en_High ),    // input wire rd_en
//        .dout    (  dout       ),    // output wire [17 : 0] dout
//        .full    (  full       ),    // output wire full
//        .empty   (  empty      )     // output wire empty
//);
//
//wire [15:0] dff_data_clk;
//wire dff_srout_clk;
//assign dff_data_clk = dout[15:0];
//assign dff_srout_clk = dout[16]; 


assign DATA_OUT = dff_data_clk;
assign SR_OUT_OUT = dff_srout_clk;


endmodule

