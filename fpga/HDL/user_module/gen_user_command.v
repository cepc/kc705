module gen_user_command (
    input  wire       CLK,
    input  wire [7:0] mem8_in_port_b,
    output wire [7:0] mem8_out_port_b,
    output wire [4:0] mem8_addr_port_b,
    output wire       mem8_access_en_port_b,
    output wire       mem8_w_enable_port_b,
    input  wire       FIFO_OVER_FLOW_FLAG,
    output wire [5:0] ROW_READ_START,
    output wire [5:0] ROW_READ_END,
    output wire [3:0] COLUMN_READ_START,
    output wire [3:0] COLUMN_READ_END,
    output wire [7:0] CHIP_ADDRESS,
    output wire       INIT,
    output wire       INIT_PULSE,
    output wire       START,
    output wire       START_PULSE,
    output wire       STOP,
    output wire       STOP_PULSE,
    output wire       RST,
    output wire       RST_PULSE
);


// Generating 10 clock cycle
reg [7:0] cnt = 8'h0;
always @( posedge CLK )
begin
    if ( cnt == 8'd9 )
        cnt <= 8'h0;
    else
        cnt <= cnt + 8'h1;
end

// -----------------------------------------------//
// switch         ------|_|-----------
// address change --------|_|----------
// check          ------------|_|------
//                        <--> is 4 clocks 
// ----------------------------------------------//

wire switch = ( cnt == 8'h1 );
wire check  = ( cnt == 8'h7 );  // more than 3 clocks after memory address set is ok
wire half   = ( cnt >= 8'h2 && cnt <= 8'h6 );  
wire last_half = ( cnt >= 8'h7 );

// Memory Address Counter. Maximum 32*2 = 6bit. 
reg [5:0] ad_cnt  = 6'h0;
always @( posedge CLK )
begin
    if ( switch ) begin
        if ( ad_cnt == 6'd63 ) 
            ad_cnt <= 6'h0;
        else
            ad_cnt <= ad_cnt + 6'h1;
    end
end


// Memory Address 
// Address  0 - 7  : Run Control commands ( reset , start, ,,, )
// Address  8 - 15 : Data Mode settings I. ( chip address, row/column, ,,, )
// Address 16 - 23 : Data Mode settings II. ( suppression mode, timing delay, nframe, ,,, )
// Address 24 - 31 : Reserved.     

reg       mem_access_rden  = 1'h0;
reg [4:0] mem_address_rden = 5'h0;

always @( posedge CLK )
begin
    case ( ad_cnt )
        6'd0  :  mem_address_rden <= 5'd0;    // Memory Address 0  :  Slot for communication test  
        6'd1  :  mem_address_rden <= 5'd1;    // Memory Address 1  : "Reset" command    
        6'd2  :  mem_address_rden <= 5'd2;    // Memory Address 2  : "Initilization" command         
        6'd3  :  mem_address_rden <= 5'd3;    // Memory Address 3  : "Start" command
        6'd4  :  mem_address_rden <= 5'd4;    // Memory Address 4  : "Stop"  command
        6'd5  :  mem_address_rden <= 5'd5;    // Memory Address 5  :     reserved
        6'd6  :  mem_address_rden <= 5'd6;    // Memory Address 6  :     reserved
        6'd7  :  mem_address_rden <= 5'd7;    // Memory Address 7  :     reserved  ---> FIFO Overflow Flag
        
        6'd8  :  mem_address_rden <= 5'd8;    // Memory Address 8  : "Chip sector Address"       
        6'd9  :  mem_address_rden <= 5'd9;    // Memory Address 9  :     reserved
        6'd10 :  mem_address_rden <= 5'd10;   // Memory Address 10 : "row start"
        6'd11 :  mem_address_rden <= 5'd11;   // Memory Address 11 : "row end"
        6'd12 :  mem_address_rden <= 5'd12;   // Memory Address 12 : "column start"
        6'd13 :  mem_address_rden <= 5'd13;   // Memory Address 13 : "column end"
        6'd14 :  mem_address_rden <= 5'd14;   // Memory Address 14 :     reserved
        6'd15 :  mem_address_rden <= 5'd15;   // Memory Address 15 :     reserved

//        6'd16 :  mem_address_rden <= 5'd16;   // Memory Address 16 :     reserved
//        6'd17 :  mem_address_rden <= 5'd17;   // Memory Address 17 :     reserved
//        6'd18 :  mem_address_rden <= 5'd18;   // Memory Address 18 :     reserved
//        6'd19 :  mem_address_rden <= 5'd19;   // Memory Address 19 :     reserved
//        6'd20 :  mem_address_rden <= 5'd20;   // Memory Address 20 :     reserved
//        6'd21 :  mem_address_rden <= 5'd21;   // Memory Address 21 :     reserved
//        6'd22 :  mem_address_rden <= 5'd22;   // Memory Address 22 :     reserved
//        6'd23 :  mem_address_rden <= 5'd23;   // Memory Address 23 :     reserved
//        6'd24 :  mem_address_rden <= 5'd24;   // Memory Address 24 :     reserved
//        6'd25 :  mem_address_rden <= 5'd25;   // Memory Address 25 :     reserved
//        6'd26 :  mem_address_rden <= 5'd26;   // Memory Address 26 :     reserved
//        6'd27 :  mem_address_rden <= 5'd27;   // Memory Address 27 :     reserved
//        6'd28 :  mem_address_rden <= 5'd28;   // Memory Address 28 :     reserved
//        6'd29 :  mem_address_rden <= 5'd29;   // Memory Address 29 :     reserved
//        6'd30 :  mem_address_rden <= 5'd30;   // Memory Address 30 :     reserved
//        6'd31 :  mem_address_rden <= 5'd31;   // Memory Address 31 :     reserved
        default:  mem_address_rden <= 5'd0;   
    endcase
end

// Memory Read Enable 
always @( posedge CLK )
begin
    //if ( ad_cnt >= 6'd1 && ad_cnt <= 6'd31 )
    if ( ad_cnt >= 6'd1 && ad_cnt <= 6'd15 )    // Read Enable 1-15 
        mem_access_rden <= 1'h1;
    else
        mem_access_rden <= 1'h0;
end


// Shift register to hold data from memory
reg [7:0] mem8_reg1;
reg [7:0] mem8_reg2;
reg [7:0] mem8_reg3;
always @( posedge CLK )
begin
    mem8_reg1 <= mem8_in_port_b;
    mem8_reg2 <= mem8_reg1;
    mem8_reg3 <= mem8_reg2;
end

// Read Data from Memory 
reg [5:0] row_start_reg     = 6'h0;
reg [5:0] row_end_reg       = 6'h0;
reg [3:0] col_start_reg     = 4'h0;
reg [3:0] col_end_reg       = 4'h0;
reg [7:0] cmos_addr_reg     = 8'h0;

always @( posedge CLK )
begin
    if ( last_half ) begin
        case ( ad_cnt )
            6'd8  :  cmos_addr_reg <= mem8_reg2[7:0];      // "chip address" 
            6'd10 :  row_start_reg <= mem8_reg2[5:0];      // "row" start
            6'd11 :  row_end_reg   <= mem8_reg2[5:0];      // "row" end
            6'd12 :  col_start_reg <= mem8_reg2[3:0];      // "column" start
            6'd13 :  col_end_reg   <= mem8_reg2[3:0];      // "column" end
            default : ;
        endcase
    end
end

assign ROW_READ_START    = row_start_reg ;
assign ROW_READ_END      = row_end_reg ;
assign COLUMN_READ_START = col_start_reg ;
assign COLUMN_READ_END   = col_end_reg ;
assign CHIP_ADDRESS      = cmos_addr_reg ;




////////////////////////////  RESET  ////////////////////////////

// Generate "Reset" 
reg gen_reset  =1'h0;
reg reset_flag =1'h0;
always @( posedge CLK )
begin
    if ( ad_cnt == 6'd0 ) begin
        gen_reset <= 1'h0;
        reset_flag <= 1'h0;
    end
    if( ad_cnt == 6'd1 ) begin
        if ( check && mem8_reg2==8'b00001111) begin
            gen_reset  <= 1'h1;   // Generate "Reset" Pulse
            reset_flag <= 1'h1;   // "Reset" flag ON
        end
        else
            gen_reset  <= 1'h0;
    end
end

reg [7:0] rst_cnt = 8'h0;
reg       rst_reg = 1'h0;
reg       rst_on  = 1'h0;
always @( posedge CLK )
begin
    if ( gen_reset )
    begin
        rst_reg <= 1'h1;
        rst_cnt <= 8'h0;
        rst_on  <= 1'h1;
    end
    else 
    begin
        if ( rst_on )
        begin
            rst_cnt <= rst_cnt + 8'h1;
 
            if ( rst_cnt==8'd3 ) // 4 clock cycles + 1 clock (gen_reset) 
            begin
                rst_reg <= 1'h0;
                rst_on  <= 1'h0;
            end
        end
    end
end

assign RST        = (rst_reg || gen_reset);  
assign RST_PULSE  = gen_reset;  


// Turn OFF "Reset" 
reg [7:0] mem8_out_reg_rst     = 8'h0;
reg       mem_access_wren_rst  = 1'h0;
reg [4:0] mem_address_wren_rst = 5'h0;
reg       wr_en_rst            = 1'h0;
always @( posedge CLK )
begin
    if( reset_flag == 1'h1 ) // High only when the reset was issued.
    begin
        if( ad_cnt == 6'd33 ) begin
            if ( half ) begin
                mem_access_wren_rst  <= 1'h1;
                mem_address_wren_rst <= 5'd1;      // Address 1
                wr_en_rst <= 1'h1;                 // Trun ON Write Enable
                mem8_out_reg_rst <= 8'b00000000;   // Erase "reset" 
            end
            else begin
                mem_access_wren_rst  <= 1'h0;
                mem_address_wren_rst <= 5'h0;
                wr_en_rst <= 1'h0;                // Trun OFF Write Enable  
            end
        end
    end
end
             

////////////////////////////  INIT  ////////////////////////////
    
// Generate "Init" 
reg gen_init  = 1'h0;
reg init_flag = 1'h0;
reg second_time = 1'h0;
always @( posedge CLK )
begin
    if ( ad_cnt == 6'd0 ) begin
        gen_init <= 1'h0;
        if ( init_flag == 1'h1 ) begin
            if ( second_time == 1'h0 ) begin
                second_time <= 1'h1;
                init_flag <= 1'h0;
            end
            else begin
                second_time <= 1'h0;
                init_flag <= 1'h0;
            end
        end
    end
    else if ( ad_cnt == 6'd2 ) begin
        if ( check && mem8_reg2==8'b00001010) begin
            gen_init  <= 1'h1;   // Generate "Initilization" Pulse
            init_flag <= 1'h1;   // "Init" flag ON
        end
        else
            gen_init  <= 1'h0;
    end
end

reg [7:0] init_cnt = 8'h0;
reg       init_reg = 1'h0;
reg       init_on  = 1'h0;
always @( posedge CLK )
begin
    if ( gen_init )
    begin
        init_reg <= 1'h1;
        init_cnt <= 8'h0;
        init_on  <= 1'h1;
    end
    else 
    begin
        if ( init_on )
        begin
            init_cnt <= init_cnt + 8'h1;
 
            if ( init_cnt==8'd3 ) // 4 clock cycles + 1 clock (gen_init) 
            begin
                init_reg <= 1'h0;
                init_on  <= 1'h0;
            end
        end
    end
end

assign INIT        = (init_reg || gen_init);  
assign INIT_PULSE  = gen_init;  


// Turn OFF "Init" 
reg [7:0] mem8_out_reg_init     = 8'h0;
reg       mem_access_wren_init  = 1'h0;
reg [4:0] mem_address_wren_init = 5'h0;
reg       wr_en_init            = 1'h0;
always @( posedge CLK )
begin
    if( init_flag == 1'h1 ) // High only when the init was issued.
    begin
        if( ad_cnt == 6'd35 ) begin
            if ( half ) begin
                if ( second_time == 1'h1 ) begin        // "Init" pulse is generated two times !
                    mem_access_wren_init  <= 1'h1;      //  at the "second" time, erase the init signal.
                    mem_address_wren_init <= 5'd2;      // Address 2
                    wr_en_init <= 1'h1;                 // Trun ON Write Enable
                    mem8_out_reg_init <= 8'b00000000;   // Erase "reset" 
                end
            end
            else begin
                mem_access_wren_init  <= 1'h0;
                mem_address_wren_init <= 5'h0;
                wr_en_init <= 1'h0;                // Trun OFF Write Enable  
            end
        end
    end
end



////////////////////////////  START  ////////////////////////////

// Generate "Start" 
reg gen_start  =1'h0;
reg start_flag =1'h0;
always @( posedge CLK )
begin
    if ( ad_cnt == 6'd0 ) begin
        gen_start <= 1'h0;
        start_flag <= 1'h0;
    end
    if( ad_cnt == 6'd3 ) begin
        if ( check && mem8_reg2==8'b00001111) begin
            gen_start  <= 1'h1;   // Generate "Start" Pulse
            start_flag <= 1'h1;   // "Start" flag ON
        end
        else
            gen_start  <= 1'h0;
    end
end

reg [7:0] start_cnt = 8'h0;
reg       start_reg = 1'h0;
reg       start_on  = 1'h0;
always @( posedge CLK )
begin
    if ( gen_start )
    begin
        start_reg <= 1'h1;
        start_cnt <= 8'h0;
        start_on  <= 1'h1;
    end
    else 
    begin
        if ( start_on )
        begin
            start_cnt <= start_cnt + 8'h1;
 
            if ( start_cnt==8'd3 ) // 4 clock cycles + 1 clock (gen_start) 
            begin
                start_reg <= 1'h0;
                start_on  <= 1'h0;
            end
        end
    end
end

assign START        = (start_reg || gen_start);  
assign START_PULSE  = gen_start;  


// Turn OFF "Start" 
reg [7:0] mem8_out_reg_start     = 8'h0;
reg       mem_access_wren_start  = 1'h0;
reg [4:0] mem_address_wren_start = 5'h0;
reg       wr_en_start            = 1'h0;
always @( posedge CLK )
begin
    if( start_flag == 1'h1 ) // High only when the start was issued.
    begin
        if( ad_cnt == 6'd37 ) begin
            if ( half ) begin
                mem_access_wren_start  <= 1'h1;
                mem_address_wren_start <= 5'd3;      // Address 3
                wr_en_start <= 1'h1;                 // Trun ON Write Enable
                mem8_out_reg_start <= 8'b00000000;   // Erase "start" 
            end
            else begin
                mem_access_wren_start  <= 1'h0;
                mem_address_wren_start <= 5'h0;
                wr_en_start <= 1'h0;                // Trun OFF Write Enable  
            end
        end
    end
end
             
             
////////////////////////////  STOP  ////////////////////////////

// Generate "Stop" 
reg gen_stop  =1'h0;
reg stop_flag =1'h0;
always @( posedge CLK )
begin
    if ( ad_cnt == 6'd0 ) begin
        gen_stop <= 1'h0;
        stop_flag <= 1'h0;
    end
    if( ad_cnt == 6'd4 ) begin
        if ( check && mem8_reg2==8'b00001111) begin
            gen_stop  <= 1'h1;   // Generate "Stop" Pulse
            stop_flag <= 1'h1;   // "Stop" flag ON
        end
        else
            gen_stop  <= 1'h0;
    end
end

reg [7:0] stop_cnt = 8'h0;
reg       stop_reg = 1'h0;
reg       stop_on  = 1'h0;
always @( posedge CLK )
begin
    if ( gen_stop )
    begin
        stop_reg <= 1'h1;
        stop_cnt <= 8'h0;
        stop_on  <= 1'h1;
    end
    else 
    begin
        if ( stop_on )
        begin
            stop_cnt <= stop_cnt + 8'h1;
 
            if ( stop_cnt==8'd3 ) // 4 clock cycles + 1 clock (gen_stop) 
            begin
                stop_reg <= 1'h0;
                stop_on  <= 1'h0;
            end
        end
    end
end

assign STOP        = (stop_reg || gen_stop);  
assign STOP_PULSE  = gen_stop;  


// Turn OFF "Stop" 
reg [7:0] mem8_out_reg_stop     = 8'h0;
reg       mem_access_wren_stop  = 1'h0;
reg [4:0] mem_address_wren_stop = 5'h0;
reg       wr_en_stop            = 1'h0;
always @( posedge CLK )
begin
    if( stop_flag == 1'h1 ) // High only when the stop was issued.
    begin
        if( ad_cnt == 6'd39 ) begin
            if ( half ) begin
                mem_access_wren_stop  <= 1'h1;
                mem_address_wren_stop <= 5'd4;      // Address 4
                wr_en_stop <= 1'h1;                 // Trun ON Write Enable
                mem8_out_reg_stop <= 8'b00000000;   // Erase "stop" 
            end
            else begin
                mem_access_wren_stop  <= 1'h0;
                mem_address_wren_stop <= 5'h0;
                wr_en_stop <= 1'h0;                // Trun OFF Write Enable  
            end
        end
    end
end

/////////////////   FIFO Overflow  ///////////////////
reg fifo_overflow_flag =1'h1;

reg [7:0] mem8_out_reg_overflow     = 8'h0;
reg       mem_access_wren_overflow  = 1'h0;
reg [4:0] mem_address_wren_overflow = 5'h0;
reg       wr_en_overflow            = 1'h0;
always @( posedge CLK )
begin
//    if( fifo_overflow_flag == 1'h1 ) // High only when the stop was issued.
//    begin
        if( ad_cnt == 6'd41 ) begin
            if ( half ) begin
                mem_access_wren_overflow  <= 1'h1;
                mem_address_wren_overflow <= 5'd7;      // Address 7  --- Temporary !!
                wr_en_overflow <= 1'h1;                 // Trun ON Write Enable
                mem8_out_reg_overflow <= FIFO_OVER_FLOW_FLAG;   // Write fifo overflow flag
            end
            else begin
                mem_access_wren_overflow  <= 1'h0;
                mem_address_wren_overflow <= 5'h0;
                wr_en_overflow <= 1'h0;                // Trun OFF Write Enable  
            end
        end
//    end
end


///////////////////////////////////////////
// Variables for memory writing: set "reset" & "init" & "start" & "stop" value as "0" 
reg [7:0] mem8_out_reg     = 8'h0;
reg       mem_access_wren  = 1'h0;
reg [4:0] mem_address_wren = 5'h0;
reg       wr_en            = 1'h0;

always @( posedge CLK )
begin
    if( ad_cnt == 6'd33 ) begin                   // Erase RESET 
        mem_access_wren  <= mem_access_wren_rst;
        mem_address_wren <= mem_address_wren_rst;
        wr_en            <= wr_en_rst;
        mem8_out_reg     <= mem8_out_reg_rst;
    end
    else if ( ad_cnt == 6'd35 ) begin             // Erase Init
        mem_access_wren  <= mem_access_wren_init;
        mem_address_wren <= mem_address_wren_init;
        wr_en            <= wr_en_init;
        mem8_out_reg     <= mem8_out_reg_init;
    end
    else if ( ad_cnt == 6'd37 ) begin             // Erase Start
        mem_access_wren  <= mem_access_wren_start;
        mem_address_wren <= mem_address_wren_start;
        wr_en            <= wr_en_start;
        mem8_out_reg     <= mem8_out_reg_start;
    end
    else if ( ad_cnt == 6'd39 ) begin             // Erase Start
        mem_access_wren  <= mem_access_wren_stop;
        mem_address_wren <= mem_address_wren_stop;
        wr_en            <= wr_en_stop;
        mem8_out_reg     <= mem8_out_reg_stop;
    end
    else if ( ad_cnt == 6'd41 ) begin             // FIFO OverFlow Flag
        mem_access_wren  <= mem_access_wren_overflow;
        mem_address_wren <= mem_address_wren_overflow;
        wr_en            <= wr_en_overflow;
        mem8_out_reg     <= mem8_out_reg_overflow;
    end
    else begin
        mem8_out_reg     <= 8'h0;
        mem_access_wren  <= 1'h0;
        mem_address_wren <= 5'h0;
        wr_en            <= 1'h0;
    end
end

assign mem8_w_enable_port_b  = wr_en;
assign mem8_out_port_b       = mem8_out_reg;
    
    
assign mem8_access_en_port_b  = ( mem_access_rden | mem_access_wren );
assign mem8_addr_port_b       = ( mem_address_rden | mem_address_wren );


endmodule