module set_chip_addr (
    input          CLK,
    input          CLK_IPCORE,
    input          RST,
    input          SET,
    input   [7:0]  CHIP_ADDR_IN,
    output  [3:0]  CHIP_ADDR_OUT
);


// DFF to make timing edge flat  
(* mark_debug = "true" *) reg set_dff, set_dffdff;
(* mark_debug = "true" *) reg [7:0] chip_addr_in_dff = 8'h0; 
(* mark_debug = "true" *) reg [7:0] chip_addr_in_dffdff = 8'h0;
always @( posedge CLK )
begin
    chip_addr_in_dff <= CHIP_ADDR_IN;
    chip_addr_in_dffdff <= chip_addr_in_dff;
    set_dff <= SET;
    set_dffdff <= set_dff;
end

wire wr_addr = 1'b1; // Address "1"
wire rd_addr = 1'b1; // Address "1" 
reg wr_addr_r; // Address "1"
reg rd_addr_r; // Address "1" 
always @( posedge CLK )
begin
    wr_addr_r <= wr_addr;
    rd_addr_r <= rd_addr;
end

(* mark_debug = "true" *) wire [7:0] chip_address_out;
(* mark_debug = "true" *) reg  [7:0] chip_address_out_dff = 8'h0;
wire full, empty;

reg wr_en_mem;
reg rd_en_mem;
wire wr_en_High = 1'b1;
wire rd_en_High = 1'b1;

always @( posedge CLK )
begin
    if ( set_dff == 1'b1 ) begin
        wr_en_mem <= 1'b1;
        rd_en_mem <= 1'b0;
    end
    else begin
        wr_en_mem <= 1'b0;
        rd_en_mem <= 1'b1;
    end
end


coregen_mem8_chipaddr clk_crossing_mem8 (
  .clka  (  CLK                 ),    // input wire clka
  .ena   (  wr_en_mem           ),    // input wire ena
  .wea   (  set_dffdff          ),    // input wire [0 : 0] wea
  .addra (  wr_addr_r           ),    // input wire [0 : 0] addra
  .dina  (  chip_addr_in_dffdff ),    // input wire [7 : 0] dina
  .clkb  (  CLK_IPCORE          ),    // input wire clkb
  .enb   (  rd_en_mem           ),    // input wire enb
  .addrb (  rd_addr_r           ),    // input wire [0 : 0] addrb
  .doutb (  chip_address_out    )     // output wire [7 : 0] doutb
);

  
always @( posedge CLK_IPCORE )
begin
    chip_address_out_dff <= chip_address_out;
end

assign CHIP_ADDR_OUT = chip_address_out_dff[3:0];


endmodule

