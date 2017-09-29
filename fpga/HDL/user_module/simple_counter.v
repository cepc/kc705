module simple_counter (
    input  wire CLK,
    input  wire RST,
    output wire[31:0] TEST_OUT,
    output wire WR_EN
);

/*reg [7:0] counter; */
reg [7:0] counter = 0;
reg fifo_wr_en = 0;

always @( posedge CLK ) 
begin
    if ( RST ) begin
        counter <= 8'h0;
        fifo_wr_en <= 0;
    end
    else begin
        if ( counter == 100 )begin
          // counter <= 8'h0;
          fifo_wr_en <= 0;
        end
        else begin
          counter <= counter + 8'h1;
          fifo_wr_en <= 1;
        end
    end   
end

assign TEST_OUT = {{24'h0},{counter}}; 
assign WR_EN = fifo_wr_en;

endmodule