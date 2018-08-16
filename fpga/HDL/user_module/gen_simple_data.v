module gen_simple_data (
    input          CLK,
    input          RST,
    output [15:0]  DATA_OUT,
    output         SR_OUT
);


// Counter : 48 clocks (for column ID) 
reg [5:0] cnt  = 6'h0;
always @( posedge CLK )
begin
    if ( RST )
        cnt <= 6'h0;
    else if ( cnt == 6'd47 ) 
        cnt <= 6'h0;
    else
        cnt <= cnt + 6'h1;
end


// Generate Dummy Data 
reg  [11:0] data = 12'h0;
reg  [3:0] frame_cnt = 4'h0;

always @( posedge CLK )
begin
    if ( cnt == 6'h0 )
    begin
        data <= 12'h0;
        frame_cnt <= frame_cnt + 4'h1;   
    end
    else
        data <=  data + 12'h1;
end

// Generate SR_OUT Pulse
reg  sr_out_reg = 1'b0;
always @( posedge CLK )
begin
    if ( cnt == 6'd47 )       // High at the final row
        sr_out_reg <= 1'b1;   
    else
        sr_out_reg <= 1'b0; 
end


assign DATA_OUT = {frame_cnt , data};
assign SR_OUT   = sr_out_reg;

endmodule

