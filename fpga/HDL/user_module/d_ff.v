module d_ff (
    input          CLK,
    input          RST,
    input  [15:0]  DATA_IN,
    output [15:0]  DATA_OUT
);

reg [15:0] data_reg;

assign DATA_OUT = data_reg;

always @( posedge CLK )
begin
    if ( RST )
        data_reg <= 16'h0;
    else
        data_reg <= DATA_IN;
end


endmodule

