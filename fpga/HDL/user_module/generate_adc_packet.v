module generate_adc_packet (
    input          CLK,          
    input          RST,
    input  [15:0]  FIFO_OUT1,
    input  [15:0]  FIFO_OUT2,          
    output [31:0]  DATA_OUT 
);

// Counter (Header + 24words + footer) = 26 clocks 
reg [7:0] cnt = 0;
always @( posedge CLK )
begin
    if ( RST )
        cnt <= 8'h0;
    else if ( cnt == 8'd25 )
        cnt <= 8'h0;
    else
        cnt <= cnt + 1'h1;
end

// Generate Data Set
(* keep = "true" *) reg  [31:0] data = 32'b0;
always @( posedge CLK )
begin
    if ( cnt == 8'd0 )  begin   // header
        data <= 32'haaaaaaaa;   
    end    
    else if ( cnt == 8'd25 )   // footer
        data <= 32'hf0f0f0f0;
    else begin
        data <= { FIFO_OUT1, FIFO_OUT2 };
    end
end

assign DATA_OUT = data;

endmodule

