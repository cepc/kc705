module kc705 (
    input        CLKOUTN_n,
    input        CLKOUTN_p,
    output       CNV,
    output       SCK_n,
    output       SCK_p,
    input        SDO10_n, SDO11_n, SDO12_n, SDO13_n, SDO14_n, SDO15_n, SDO16_n, 
    input        SDO10_p, SDO11_p, SDO12_p, SDO13_p, SDO14_p, SDO15_p, SDO16_p,    
    input        SDO1_n, SDO2_n, SDO3_n, SDO4_n, SDO5_n, SDO6_n, SDO7_n, SDO8_n, SDO9_n,
    input        SDO1_p, SDO2_p, SDO3_p, SDO4_p, SDO5_p, SDO6_p, SDO7_p, SDO8_p, SDO9_p,
    input        SR_Top_out_c,
    output       clk_div,
    output [6:0] d_A,
    output       oe,
    input        rst,
    input        start,
    output       sync,
    input        sysclk_n,
    input        sysclk_p,
    input        PCIE_PERST_B_LS,
    input        PCIE_REFCLK_N,
    input        PCIE_REFCLK_P,
    input  [7:0] PCIE_RX_N,
    input  [7:0] PCIE_RX_P,
    output [3:0] GPIO_LED,
    output [7:0] PCIE_TX_N,
    output [7:0] PCIE_TX_P
);
   



//**********************************************//
//*****  System Clock generated from MMCM  *****//
//**********************************************//

(* MARK_DEBUG="true" *)wire clk200M;
(* MARK_DEBUG="true" *)wire clk100M;
(* MARK_DEBUG="true" *)wire clk100M_2;
wire clk40M;

coregen_sysclk  sysclk_ins_test(
    .clk_out1   (  clk100M  ),  // 100MHz clock
    .clk_out2   (  clk40M   ),  //  40MHz clock
    .clk_out3   (  clk100M_2),  // 100MHz clock
    .clk_in1    (  clk200M  )
//    .clk_in1_p  (  sysclk_p ),
//    .clk_in1_n  (  sysclk_n )
);
 
// Reset Signal 
(* mark_debug = "true" *) wire rst_command; 


//**********************************************//
//*****  Generate 2MHz/4MHz clock & reset  *****//
//**********************************************//
(* MARK_DEBUG="true" *)wire clk2M;
(* MARK_DEBUG="true" *)wire rst2M;
wire clk4M;
wire rst4M;
gen_user_clock gen_user_clock_ins (
    .CLK_IN      (  clk100M_2   ),        // 100MHz clock in
    .CLK_40M_IN  (  clk40M      ),        //  40MHz clock in
    .RST_IN      (  rst_command ),        // 100MHz reset in  
    .CLK_2M_OUT  (  clk2M       ),        //   2MHz clock out
    .RST_2M_OUT  (  rst2M       ),        //   2MHz reset out (5 cycles)
    .CLK_4M_OUT  (  clk4M       ),        //   4MHz clock out
    .RST_4M_OUT  (  rst4M       )         //   4MHz reset out (5 cycles)
);


//*************************************************************//
//*****  ADC IP Core : Communication with the chip & ADC  *****//
//*************************************************************//

wire [15:0] data01, data02, data03, data04, data05, data06, data07, data08;
wire [15:0] data09, data10, data11, data12, data13, data14, data15, data16;

//SR_Top_out's falling edge symbolsize the last row of one frame
(* MARK_DEBUG="true" *)wire SR_Top_out;        
// cnvclk is the clock(2MHz) we use to read the data, 
// I suggest using the negedge of it to read the data
(* MARK_DEBUG="true" *)wire cnvclk;          

ADC_REC_LVDS AD_data_receiver(
     .DATA1         (  data01       ), 
     .DATA2         (  data02       ), 
     .DATA3         (  data03       ), 
     .DATA4         (  data04       ), 
     .DATA5         (  data05       ), 
     .DATA6         (  data06       ), 
     .DATA7         (  data07       ), 
     .DATA8         (  data08       ), 
     .DATA9         (  data09       ), 
     .DATA10        (  data10       ), 
     .DATA11        (  data11       ), 
     .DATA12        (  data12       ), 
     .DATA13        (  data13       ), 
     .DATA14        (  data14       ), 
     .DATA15        (  data15       ), 
     .DATA16        (  data16       ), 
     .d_A           (  d_A          ), 
     .sysclk_p      (  sysclk_p     ), 
     .sysclk_n      (  sysclk_n     ), 
     .SDO10_n       (  SDO10_n      ),
     .SDO10_p       (  SDO10_p      ),
     .SDO11_n       (  SDO11_n      ),
     .SDO11_p       (  SDO11_p      ),
     .SDO12_n       (  SDO12_n      ),
     .SDO12_p       (  SDO12_p      ),
     .SDO13_n       (  SDO13_n      ),
     .SDO13_p       (  SDO13_p      ),
     .SDO14_n       (  SDO14_n      ),
     .SDO14_p       (  SDO14_p      ),
     .SDO15_n       (  SDO15_n      ),
     .SDO15_p       (  SDO15_p      ),
     .SDO16_n       (  SDO16_n      ),
     .SDO16_p       (  SDO16_p      ),
     .SDO1_n        (  SDO1_n       ),
     .SDO1_p        (  SDO1_p       ),
     .SDO2_n        (  SDO2_n       ),
     .SDO2_p        (  SDO2_p       ),
     .SDO3_n        (  SDO3_n       ),
     .SDO3_p        (  SDO3_p       ),
     .SDO4_n        (  SDO4_n       ),
     .SDO4_p        (  SDO4_p       ),
     .SDO5_n        (  SDO5_n       ),
     .SDO5_p        (  SDO5_p       ),
     .SDO6_n        (  SDO6_n       ),
     .SDO6_p        (  SDO6_p       ),
     .SDO7_n        (  SDO7_n       ),
     .SDO7_p        (  SDO7_p       ),
     .SDO8_n        (  SDO8_n       ),
     .SDO8_p        (  SDO8_p       ),
     .SDO9_n        (  SDO9_n       ),
     .SDO9_p        (  SDO9_p       ),
     .CLKOUTN_p     (  CLKOUTN_p    ), 
     .CLKOUTN_n     (  CLKOUTN_n    ), 
     .rst           (  rst          ), 
     .start         (  start        ), 
     .CNV           (  CNV          ), 
     .cnvclk        (  cnvclk       ), 
     .SCK_p         (  SCK_p        ), 
     .SCK_n         (  SCK_n        ), 
     .SR_Top_out_c  (  SR_Top_out_c ), 
     .SR_Top_out    (  SR_Top_out   ), 
     .clk           (  clk200M      ),  // Global Buffer Out
     .oe            (  oe           ), 
     .clk_div       (  clk_div      ), 
     .sync          (  sync         )
);
     
     
//******************************************************//
//*****   Xillybus IP Core : Access to PCI Express *****//
//******************************************************//

// Clock and quiesce
wire     xillybus_user_clk;
wire     quiesce;

// Memory array
reg [7:0]     demoarray[0:31];

// Wires related to /dev/xillybus_mem_8
(* keep = "true" *) wire       user_r_mem_8_rden;
wire       user_r_mem_8_empty;
//reg [7:0]  user_r_mem_8_data;
wire [7:0]  user_r_mem_8_data;
wire       user_r_mem_8_eof;
wire       user_r_mem_8_open;
(* keep = "true" *) wire       user_w_mem_8_wren;
wire       user_w_mem_8_full;
wire [7:0] user_w_mem_8_data;
wire       user_w_mem_8_open;
wire [4:0] user_mem_8_addr;
wire       user_mem_8_addr_update;

// Wires related to /dev/xillybus_read_32
wire       user_r_read_32_rden;
wire       user_r_read_32_empty;
wire [31:0] user_r_read_32_data;
wire        user_r_read_32_eof;
wire        user_r_read_32_open;

// Wires related to /dev/xillybus_read_8
wire        user_r_read_8_rden;
wire        user_r_read_8_empty;
wire [7:0]  user_r_read_8_data;
wire        user_r_read_8_eof;
wire        user_r_read_8_open;

// Wires related to /dev/xillybus_write_32
wire        user_w_write_32_wren;
wire        user_w_write_32_full;
wire [31:0] user_w_write_32_data;
wire        user_w_write_32_open;

// Wires related to /dev/xillybus_write_8
wire        user_w_write_8_wren;
wire        user_w_write_8_full;
wire [7:0]  user_w_write_8_data;
wire        user_w_write_8_open;

xillybus xillybus_ins (

  // Ports related to /dev/xillybus_mem_8
  // FPGA to CPU signals:
  .user_r_mem_8_rden       (  user_r_mem_8_rden      ),
  .user_r_mem_8_empty      (  user_r_mem_8_empty     ),
  .user_r_mem_8_data       (  user_r_mem_8_data      ),
  .user_r_mem_8_eof        (  user_r_mem_8_eof       ),
  .user_r_mem_8_open       (  user_r_mem_8_open      ),

  // CPU to FPGA signals:
  .user_w_mem_8_wren       (  user_w_mem_8_wren      ),
  .user_w_mem_8_full       (  user_w_mem_8_full      ),
  .user_w_mem_8_data       (  user_w_mem_8_data      ),
  .user_w_mem_8_open       (  user_w_mem_8_open      ),

  // Address signals:
  .user_mem_8_addr         (  user_mem_8_addr        ),
  .user_mem_8_addr_update  (  user_mem_8_addr_update ),


  // Ports related to /dev/xillybus_read_32
  // FPGA to CPU signals:
  .user_r_read_32_rden     (  user_r_read_32_rden    ),
  .user_r_read_32_empty    (  user_r_read_32_empty   ),
  .user_r_read_32_data     (  user_r_read_32_data    ),
  .user_r_read_32_eof      (  user_r_read_32_eof     ),
  .user_r_read_32_open     (  user_r_read_32_open    ),

  // Ports related to /dev/xillybus_write_32
  // CPU to FPGA signals:
  .user_w_write_32_wren    (  user_w_write_32_wren   ),
  .user_w_write_32_full    (  user_w_write_32_full   ),
  .user_w_write_32_data    (  user_w_write_32_data   ),
  .user_w_write_32_open    (  user_w_write_32_open   ),

  // Ports related to /dev/xillybus_read_8
  // FPGA to CPU signals:
  .user_r_read_8_rden      (  user_r_read_8_rden     ),
  .user_r_read_8_empty     (  user_r_read_8_empty    ),
  .user_r_read_8_data      (  user_r_read_8_data     ),
  .user_r_read_8_eof       (  user_r_read_8_eof      ),
  .user_r_read_8_open      (  user_r_read_8_open     ),

  // Ports related to /dev/xillybus_write_8
  // CPU to FPGA signals:
  .user_w_write_8_wren     (  user_w_write_8_wren    ),
  .user_w_write_8_full     (  user_w_write_8_full    ),
  .user_w_write_8_data     (  user_w_write_8_data    ),
  .user_w_write_8_open     (  user_w_write_8_open    ),


  // Signals to top level
  .PCIE_PERST_B_LS         (  PCIE_PERST_B_LS        ),
  .PCIE_REFCLK_N           (  PCIE_REFCLK_N          ),
  .PCIE_REFCLK_P           (  PCIE_REFCLK_P          ),
  .PCIE_RX_N               (  PCIE_RX_N              ),
  .PCIE_RX_P               (  PCIE_RX_P              ),
  .GPIO_LED                (  GPIO_LED               ),
  .PCIE_TX_N               (  PCIE_TX_N              ),
  .PCIE_TX_P               (  PCIE_TX_P              ),
  .bus_clk                 (  xillybus_user_clk      ),
  .quiesce                 (  quiesce                )
);

// A simple inferred RAM
/*
always @(posedge xillybus_user_clk)
  begin
 if (user_w_mem_8_wren)
   demoarray[user_mem_8_addr] <= user_w_mem_8_data;
 
 if (user_r_mem_8_rden)
   user_r_mem_8_data <= demoarray[user_mem_8_addr];      
  end

assign  user_r_mem_8_empty = 0;
assign  user_r_mem_8_eof = 0;
assign  user_w_mem_8_full = 0;
*/

assign  user_r_mem_8_empty = 0;
assign  user_r_mem_8_eof = 0;
assign  user_w_mem_8_full = 0;

//***********************************************************//
//*****  FIFO-8bit :: original FIFO from xillydemo.v    *****//
//***********************************************************//
// 8-bit loopback
fifo_8x2048 fifo_8 (
  .clk     (  xillybus_user_clk                          ),
  .srst    ( !user_w_write_8_open && !user_r_read_8_open ),
  .din     (  user_w_write_8_data                        ),
  .wr_en   (  user_w_write_8_wren                        ),
  .rd_en   (  user_r_read_8_rden                         ),
  .dout    (  user_r_read_8_data                         ),
  .full    (  user_w_write_8_full                        ),
  .empty   (  user_r_read_8_empty                        )
);

assign  user_r_read_8_eof = 0;
      
//***************************************************************//
//*****  Block RAM (Memory) for handshak between PC & FPGA  *****//
//***************************************************************//

wire mem8_port_b_rd_en;
wire mem8_port_b_wr_en;
wire [4:0] mem8_port_b_addr;
wire [7:0] mem8_port_b_wdata;
wire [7:0] mem8_port_b_rdata;

wire user_mem_8_write_en = user_w_mem_8_wren;
wire user_mem_8_enable = (user_w_mem_8_wren | user_r_mem_8_rden);
//wire user_mem_8_enable = 1'b1;  // Always Enable 
 
coregen_user_mem8 user_mem8_inst (
  .clka  (  xillybus_user_clk   ),      // input wire clka
  .ena   (  user_mem_8_enable   ),      // input wire ena
  .wea   (  user_mem_8_write_en ),      // input wire [0 : 0] wea
  .addra (  user_mem_8_addr     ),      // input wire [4 : 0] addra
  .dina  (  user_w_mem_8_data   ),      // input wire [7 : 0] dina
  .douta (  user_r_mem_8_data   ),      // output wire [7 : 0] douta  
  .clkb  (  clk100M             ),      // input wire clkb
  .enb   (  mem8_port_b_rd_en   ),      // input wire enb
  .web   (  mem8_port_b_wr_en   ),      // input wire [0 : 0] web
  .addrb (  mem8_port_b_addr    ),      // input wire [4 : 0] addrb
  .dinb  (  mem8_port_b_wdata   ),      // input wire [7 : 0] dinb
  .doutb (  mem8_port_b_rdata   )       // output wire [7 : 0] doutb
);

//assign user_r_mem_8_data_in = user_r_mem_8_data; 

//****************************************//
//*****  Generate Veto Logic Signal  *****//
//****************************************//

// Veto Logic to prevent further writing when reading from the FIFO
(* mark_debug = "true" *) wire full_async_fifo;
wire [14:0] rd_data_count;
(* mark_debug = "true" *) wire [14:0] wr_data_count;

(* mark_debug = "true" *) reg [9:0] veto_cnt = 10'h0;
(* mark_debug = "true" *) reg fifo_write_veto_r = 0;
always @( posedge clk100M )
begin
    if( rst_command ) begin
        fifo_write_veto_r <= 1'b1;
        veto_cnt <= 10'h0;
    end
    else if ( veto_cnt < 10'd450 )      // original 200 --> 200+50*5=450
        veto_cnt <= veto_cnt + 1'h1;
    else if (veto_cnt ==10'd450 ) begin  // original 200 --> 450
        veto_cnt <= veto_cnt + 1'h1;
        fifo_write_veto_r <= 1'b0;
    end
//    else
//        fifo_write_veto_r <= 1'b0;
    else if ( full_async_fifo == 1 )
        fifo_write_veto_r <= 1'b1;
end
wire fifo_write_veto = fifo_write_veto_r;


//********************************************************//
//*****  Asynchronous FIFO for temporal data storage *****//
//********************************************************//

wire rden_High = 1'b1; // For Simulation test
(* mark_debug = "true" *) wire [31:0] adc_fifo_out32;
(* mark_debug = "true" *) wire adcdata_wren;


// Generate Dummy Data -- Total 8 channel : Only for simulation purpose
wire [15:0] data01_start, data02_start, data03_start, data04_start;
wire [15:0] data05_start, data06_start, data07_start, data08_start;
wire sr_out01_start, sr_out02_start, sr_out03_start, sr_out04_start;
wire sr_out05_start, sr_out06_start, sr_out07_start, sr_out08_start;

gen_simple_data   top_dummy_data_inst_data01( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data01_start ), .SR_OUT( sr_out01_start ) );
gen_simple_data   top_dummy_data_inst_data02( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data02_start ), .SR_OUT( sr_out02_start ) );
gen_simple_data   top_dummy_data_inst_data03( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data03_start ), .SR_OUT( sr_out03_start ) );
gen_simple_data   top_dummy_data_inst_data04( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data04_start ), .SR_OUT( sr_out04_start ) );

gen_simple_data   top_dummy_data_inst_data05( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data05_start ), .SR_OUT( sr_out05_start ) );
gen_simple_data   top_dummy_data_inst_data06( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data06_start ), .SR_OUT( sr_out06_start ) );
gen_simple_data   top_dummy_data_inst_data07( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data07_start ), .SR_OUT( sr_out07_start ) );
gen_simple_data   top_dummy_data_inst_data08( .CLK ( clk_div ), .RST ( rst2M ), 
.DATA_OUT( data08_start ), .SR_OUT( sr_out08_start ) );



wire [15:0] tmp_data_out;
wire tmp_wren;
recieve_adc_packet rec_adc_packet_ins (
    .CLK       (  clk100M          ),
    .CLK2M     (  clk2M            ),
    .CLK2M_DIV (  clk_div          ),
    .CNVCLK    (  cnvclk           ),
//    .SR_OUT    (  sr_out01_start   ),
    .SR_OUT    (  SR_Top_out       ),
    .RST       (  rst_command      ),
    .RST2M     (  rst2M            ),
//    .DATA01    (  data01_start     ),
//    .DATA02    (  data02_start     ),
//    .DATA03    (  data03_start     ),
//    .DATA04    (  data04_start     ),
//    .DATA05    (  data05_start     ),
//    .DATA06    (  data06_start     ),
//    .DATA07    (  data07_start     ),
//    .DATA08    (  data08_start     ),
    .DATA01    (  data01           ),
    .DATA02    (  data02           ),
    .DATA03    (  data03           ),
    .DATA04    (  data04           ),
    .DATA05    (  data05           ),
    .DATA06    (  data06           ),
    .DATA07    (  data07           ),
    .DATA08    (  data08           ),
    .DATA09    (  data09           ),
    .DATA10    (  data10           ),
    .DATA11    (  data11           ),
    .DATA12    (  data12           ),
    .DATA13    (  data13           ),
    .DATA14    (  data14           ),
    .DATA15    (  data15           ),
    .DATA16    (  data16           ),
    .DATA_OUT  (  adc_fifo_out32   ),
    .WR_EN     (  adcdata_wren     )
);      


wire almost_full_flag;
(* mark_debug = "true" *) wire adc_fifo_wren = ( !fifo_write_veto && adcdata_wren );  // For Data Generator
//(* mark_debug = "true" *) wire adc_fifo_wren = ( !fifo_write_veto && adcdata_wren && !almost_full_flag );  // For Data Generator

coregen_clk_crossing_fifo32 clk_crossing_fifo32_ins (
  .rst           (  rst_command          ),  // input wire rst
  .wr_clk        (  clk100M              ),  // input wire wr_clk
  .rd_clk        (  xillybus_user_clk    ),  // input wire rd_clk
  .din           (  adc_fifo_out32       ),  // input wire [31 : 0] din
  .wr_en         (  adc_fifo_wren        ),  // input wire wr_en
  .rd_en         (  user_r_read_32_rden  ),  // input wire rd_en
//  .rd_en         (  rden_High            ),  // input wire rd_en
  .dout          (  user_r_read_32_data  ),  // output wire [31 : 0] dout
  .full          (  full_async_fifo      ),  // output wire full
  .almost_full   (  almost_full_flag     ),  // output wire almost_full
  .empty         (  user_r_read_32_empty ),  // output wire empty
  .rd_data_count (  rd_data_count        ),  // output wire [14 : 0] rd_data_count
  .wr_data_count (  wr_data_count        )   // output wire [14 : 0] wr_data_count
);

//**********************************************//
//*****  Reset Signal Generator (Temporal) *****//
//**********************************************//

(* mark_debug = "true" *) wire rst_pulse;
gen_user_reset gen_user_reset_ins(
    .CLK                    (  clk100M           ),
    .mem8_in_port_b         (  mem8_port_b_rdata ),
    .mem8_out_port_b        (  mem8_port_b_wdata ),
    .mem8_addr_port_b       (  mem8_port_b_addr  ),
    .mem8_access_en_port_b  (  mem8_port_b_rd_en ),
    .mem8_w_enable_port_b   (  mem8_port_b_wr_en ),
    .RST                    (  rst_command       ),   // 100MHz 5 clock cycles
    .RST_PULSE              (  rst_pulse         )    // 100MHz 1 clock cycle
);
     
     


     
endmodule   

  