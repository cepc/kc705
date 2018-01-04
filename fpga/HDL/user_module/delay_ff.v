module delay_ff (
    input          CLK,
    input          RST,
    input  [15:0]  DATA_IN,
    output [15:0]  DATA_OUT
);

parameter DELAY_COUNT = 48;

wire [15:0] data[DELAY_COUNT:0];

assign data[0] = DATA_IN;
assign DATA_OUT = data[DELAY_COUNT];

generate 
    genvar i;
    for (i=0; i < DELAY_COUNT; i=i+1) begin : gen_d_ff
        d_ff u_d_ff (
            .CLK       (  CLK       ),
            .RST       (  RST       ),
            .DATA_IN   (  data[i]   ),
            .DATA_OUT  (  data[i+1] )
        );
    end
endgenerate


endmodule
