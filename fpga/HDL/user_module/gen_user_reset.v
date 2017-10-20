module gen_user_reset (
    input  wire       CLK,
    input  wire [7:0] mem8_in_port_b,
    output wire [7:0] mem8_out_port_b,
    output wire [4:0] mem8_addr_port_b,
    output wire       mem8_access_en_port_b,
    output wire       mem8_w_enable_port_b,
    output wire       RST,
    output wire       RST_PULSE
);

// Generating 1/100 samlping clock
reg [7:0] cnt = 8'h0;
always @( posedge CLK )
begin
    if ( cnt == 8'd100 )
        cnt <= 8'h0;
    else
        cnt <= cnt + 1'h1;
end

wire check = (cnt==8'h1);
//(* mark_debug = "true" *) wire check = (cnt==8'h1);

// Send ping to Memory block
reg       check_flag = 1'h0;
reg [7:0] rden_cnt = 8'h0;
reg       mem_access_rden  = 1'h0;
reg [4:0] mem_address_rden = 5'b0;
//(* mark_debug = "true" *) reg       mem_access_rden  = 1'h0;
//(* mark_debuf = "true" *) reg [4:0] mem_address_rden = 5'b0;
always @( posedge CLK )
begin
    if ( check )
    begin
        rden_cnt <= 1'h0;
        check_flag <= 1'h1;
    end
    else
    begin
        if ( check_flag )
        begin
            rden_cnt <= rden_cnt + 1'h1;
            
            mem_access_rden  <= 1'h1;
            mem_address_rden <= 5'b00001; // Reset Address    
            if ( rden_cnt==8'd5 ) 
            begin
                mem_access_rden  <= 1'h0; 
                mem_address_rden <= 5'b0;
                check_flag <= 1'h0;
            end  
        end
        else 
            rden_cnt <= 1'h0; 
    end
end


// Read & Send RESET signal
reg gen_reset=1'h0;
reg [7:0] mem8_reg1;
reg [7:0] mem8_reg2;
reg [7:0] mem8_reg3;
//(* mark_debug = "true" *) reg gen_reset=1'h0;
//(* mark_debug = "true" *) reg [7:0] mem8_reg1;
//(* mark_debug = "true" *) reg [7:0] mem8_reg2;
//(* mark_debug = "true" *) reg [7:0] mem8_reg3;
always @( posedge CLK )
begin
    mem8_reg1 <= mem8_in_port_b;
    mem8_reg2 <= mem8_reg1;
    mem8_reg3 <= mem8_reg2;
    if ( mem8_reg3==8'b00000000 && mem8_reg2==8'b00001111)
        gen_reset <= 1'h1;   // Generate Reset Pulse
    else
        gen_reset <= 1'h0;
end        

// Generate Reset Pulse for 5 clock cycles
reg [7:0] rst_cnt = 8'h0;
reg       rst_reg = 1'b0;
reg       rst_on = 1'b0;

always @( posedge CLK )
begin
    if ( gen_reset )
    begin
        rst_reg <= 1'b1;
        rst_cnt <= 8'h0;
        rst_on  <= 1'b1;
    end
    else 
    begin
        if ( rst_on )
        begin
            rst_cnt <= rst_cnt + 1'h1;
 
            if ( rst_cnt==8'd3 ) // 4 clock cycles + 1 clock (gen_reset) 
            begin
                rst_reg <= 1'b0;
                rst_on  <= 1'h0;
            end
        end
    end
end


assign RST        = (rst_reg || gen_reset);  
assign RST_PULSE  = gen_reset;  


// Turn Off "Reset" 
reg [7:0] wren_cnt = 8'h0;
reg [7:0] mem8_out_reg;
reg       mem_access_wren = 1'h0;
//(* mark_debug = "true" *) reg       mem_access_wren = 1'h0;
reg [4:0] mem_address_wren = 5'b0;
reg       wr_en = 1'h0;
reg       after_reset_flag = 1'h0;
//(* mark_debug = "true" *) reg       after_reset_flag = 1'h0;
always @( posedge CLK )
begin
    if ( gen_reset )
    begin
        wren_cnt <= 8'h0;
        after_reset_flag <= 1'h1;
    end
    else 
    begin
        if ( after_reset_flag )
        begin
            wren_cnt <= wren_cnt + 1'h1;
        
            if ( wren_cnt==8'd5 )
            begin
                mem_access_wren  <= 1'h1;
                mem_address_wren <= 5'b00001;
                wr_en <= 1'h1;          // Trun ON Write Enable
                mem8_out_reg <= 8'b00000000;
            end
            else if ( wren_cnt==8'd10 )
            begin
                mem_access_wren  <= 1'h0;
                mem_address_wren <= 5'b0;
                wr_en <= 1'h0;         // Trun OFF Write Enable                
            end
            else if ( wren_cnt==8'd15 )
                after_reset_flag <= 1'h0;
        end  
        else 
            wren_cnt <= 1'h0;
    end
end

assign mem8_w_enable_port_b  = wr_en;
assign mem8_out_port_b       = mem8_out_reg;
    
    
assign mem8_access_en_port_b  = ( mem_access_rden | mem_access_wren );
assign mem8_addr_port_b       = ( mem_address_rden | mem_address_wren );

endmodule

