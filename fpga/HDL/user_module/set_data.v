module set_data (
    input          CLK,
    input          RST,
    input   [1:0]  CH_ID,     // ID="01" (header), "10" (data), "11" (footer)
    input  [15:0]  DATA_IN,
    input          SR_OUT_IN,
    output         FIRST_ROW_FLAG,
    output [15:0]  DATA_OUT,
    output  [5:0]  MEM_ADDR,
    output         MEM_WREN
);

// DFF to make timing edge flat  
reg sr_out_dff;
reg [15:0] data_in_dff;
always @( posedge CLK )
begin
    sr_out_dff  <= SR_OUT_IN;
    data_in_dff <= DATA_IN;
end

// Delay for 16bit-data: 48 clock 
wire [15:0] data_dff_out;
delay_ff delay_data_inst (
    .CLK      (  CLK          ),
    .RST      (  RST          ),
    .DATA_IN  (  data_in_dff  ),
    .DATA_OUT (  data_dff_out )
);


// Counter : 48 clocks (for column ID) 
reg   [5:0] cnt  = 6'h0;
reg   [3:0] nevt = 4'h0;
reg   [1:0] event_type = 2'h0;    // Event type="01"(dummy data), type="01"(pedestal), type="10"(readout)
always @( posedge CLK )
begin
    if ( RST ) begin
        cnt <= 6'h0;
        event_type <= 2'b00;  // Dummy Data: event_type = "00" 
    end
    else if ( sr_out_dff ) begin
        cnt <= 6'h0;
        nevt <= nevt + 4'h1;
        event_type <= 2'b10;  // Data accompanied with SR_OUT: event_type = "10"
    end
    else if ( cnt == 6'd47 ) begin
        cnt <= 6'h0;
        nevt <= nevt + 4'h1;
        event_type <= 2'b00;  // Without SR_OUT --> Dummy Data : event_type = "00"
    end
    else
        cnt <= cnt + 6'h1;
end


// Generate Header/Trailer
reg  [15:0] data_dffdff;
reg  [15:0] header = 16'h0;
reg  [15:0] footer = 16'h0;
reg   [3:0] header_word = 4'hf;   // header = "1111" 
reg   [3:0] footer_word = 4'he;   // footer = "1110"
always @( posedge CLK )
begin
    if ( cnt == 6'h0 ) begin
        header <= { header_word, event_type, cnt, nevt };
        footer <= { footer_word, event_type, cnt, nevt };
        data_dffdff <= data_dff_out;
    end
    else begin
        header <= { header_word, event_type, cnt, nevt };
        footer <= { footer_word, event_type, cnt, nevt };
        data_dffdff <= data_dff_out;
    end
end


// Memory Address
reg   [5:0] addr =  6'h0;
always @( posedge CLK )
begin
    if ( cnt == 6'h0 )
        addr <= 6'h0;
    else
        addr <= addr + 6'h1;
end

// Memory Write Enable
reg  wren =  1'b1;  // Always Enable

// ID="01" (header), "10" (data), "11" (footer)
reg  [15:0] p0_data = 16'h0;
reg  [15:0] p1_data = 16'h0;
reg   [5:0] p0_addr =  6'h0;
reg   [5:0] p1_addr =  6'h0;
always @( posedge CLK )
begin
    case ( CH_ID )
        2'd1 : p0_data <= header;
        2'd2 : p0_data <= data_dffdff;
        2'd3 : p0_data <= footer;
        default : p0_data <= 16'hff;
    endcase
    
    p1_data <= p0_data;
    
    //p1_addr <= addr;
    p0_addr <= addr;
    p1_addr <= p0_addr;
end


// Make output flag to tell the start timing of first row 
reg first_row_flag_reg = 1'b0;
always @( posedge CLK )
begin
    if ( p1_addr == 6'h2 )           // 3 clock delay 
        first_row_flag_reg <= 1'b1;  
    else
        first_row_flag_reg <= 1'b0;
end

assign FIRST_ROW_FLAG = first_row_flag_reg;
assign DATA_OUT  = p1_data;
assign MEM_ADDR  = p1_addr;
assign MEM_WREN  = wren;

endmodule

