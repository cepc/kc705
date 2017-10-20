module kc705 (
    input        SYSCLK_P,
    input        SYSCLK_N,
    input        PCIE_PERST_B_LS,
    input        PCIE_REFCLK_N,
    input        PCIE_REFCLK_P,
    input  [7:0] PCIE_RX_N,
    input  [7:0] PCIE_RX_P,
    output [3:0] GPIO_LED,
    output [7:0] PCIE_TX_N,
    output [7:0] PCIE_TX_P
);
   

// Reset Signal 
(* mark_debug = "true" *) wire rst; 

//**********************************************//
//*****  System Clock generated from MMCM  *****//
//**********************************************//

wire clk;
coregen_sysclk  sysclk_ins(
    .clk_out1   (  clk      ),  // 100MHz clock
    .clk_in1_p  (  SYSCLK_P ),
    .clk_in1_n  (  SYSCLK_N )
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
wire       user_r_mem_8_rden;
wire       user_r_mem_8_empty;
//reg [7:0]  user_r_mem_8_data;
wire [7:0]  user_r_mem_8_data;
wire       user_r_mem_8_eof;
wire       user_r_mem_8_open;
wire       user_w_mem_8_wren;
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

// The timing of "user_r_mem_8_data" should be checked 
// since reg[7:0] --> wire[7:0]
wire user_mem_8_write_en = user_w_mem_8_wren;
wire user_mem_8_enable = (user_w_mem_8_wren | user_r_mem_8_rden);
//wire user_mem_8_enable = 1'b1;  // Always Enable 
 
coregen_user_mem8 user_mem8_inst (
  .clka  (  xillybus_user_clk   ),      // input wire clka
  .ena   (  user_mem_8_enable   ),      // input wire ena
  .wea   (  user_mem_8_write_en ),      // input wire [0 : 0] wea
//  .ena   (  user_r_mem_8_rden   ),      // input wire ena
//  .wea   (  user_w_mem_8_wren   ),      // input wire [0 : 0] wea
  .addra (  user_mem_8_addr     ),      // input wire [4 : 0] addra
  .dina  (  user_w_mem_8_data   ),      // input wire [7 : 0] dina
  .douta (  user_r_mem_8_data   ),      // output wire [7 : 0] douta  
  .clkb  (  clk                 ),      // input wire clkb
  .enb   (  mem8_port_b_rd_en   ),      // input wire enb
  .web   (  mem8_port_b_wr_en   ),      // input wire [0 : 0] web
  .addrb (  mem8_port_b_addr    ),      // input wire [4 : 0] addrb
  .dinb  (  mem8_port_b_wdata   ),      // input wire [7 : 0] dinb
  .doutb (  mem8_port_b_rdata   )       // output wire [7 : 0] doutb
);

//assign user_r_mem_8_data_in = user_r_mem_8_data; 

//********************************************************//
//*****  Asynchronous FIFO for temporal data storage *****//
//********************************************************//

(* mark_debug = "true" *) wire clk2M;
(* mark_debug = "true" *) wire rst2M;
gen_user_clock gen_user_clock_ins (
    .RST        (  rst    ),
    .CLK_IN     (  clk    ),
    .CLK_OUT    (  clk2M  ),
    .RST_2M_OUT (  rst2M  )
);

// Veto Logic to prevent further writing when reading from the FIFO
(* mark_debug = "true" *) wire full_async_fifo;
//wire [9:0] rd_data_count;
//(* mark_debug = "true" *) wire [9:0] wr_data_count;
wire [14:0] rd_data_count;
(* mark_debug = "true" *) wire [14:0] wr_data_count;

(* mark_debug = "true" *) reg [9:0] veto_cnt = 10'h0;
(* mark_debug = "true" *) reg fifo_write_veto_r = 0;
always @( posedge clk )
begin
    if( rst ) begin
        fifo_write_veto_r <= 1'b1;
        veto_cnt <= 10'h0;
    end
    else if ( veto_cnt < 10'd450 )      // original 200 --> 200+50*5=450
        veto_cnt <= veto_cnt + 1'h1;
    else if (veto_cnt ==10'd450 ) begin  // original 200 --> 450
        veto_cnt <= veto_cnt + 1'h1;
        fifo_write_veto_r <= 1'b0;
    end
    else if ( full_async_fifo == 1 )
        fifo_write_veto_r <= 1'b1;
end
wire fifo_write_veto = fifo_write_veto_r;


wire rden_High = 1'b1; // For Simulation test

(* mark_debug = "true" *) wire [15:0] ADC_FIFO_OUT1, ADC_FIFO_OUT2;
(* mark_debug = "true" *) wire [7:0] frame_count1, frame_count2;
(* mark_debug = "true" *) wire [31:0] adc_fifo_out32;

generate_adc_packet gen_adc_packet_ins (
    .CLK       (  clk2M          ),
    .RST       (  rst2M          ),
    .FIFO_OUT1 (  ADC_FIFO_OUT1  ),
    .FIFO_OUT2 (  ADC_FIFO_OUT2  ),
    .DATA_OUT  (  adc_fifo_out32 )
);  

// Generate Simple Data Structure 
//generate_data_packet gen_data_packet_ins (
//    .CLK      (  clk2M          ),
//    .RST      (  rst2M          ),
//    .DATA_OUT (  adc_fifo_out32 )
//);  
(* mark_debug = "true" *) wire adc_fifo_wren = ( !fifo_write_veto );  // For Data Generator

coregen_clk_crossing_fifo32 clk_crossing_fifo32_ins (
//  .rst           (  rst                  ),  // input wire rst
  .rst           (  rst2M                ),  // input wire rst
  .wr_clk        (  clk2M                ),  // input wire wr_clk
  .rd_clk        (  xillybus_user_clk    ),  // input wire rd_clk
  .din           (  adc_fifo_out32       ),  // input wire [31 : 0] din
  .wr_en         (  adc_fifo_wren        ),  // input wire wr_en
  .rd_en         (  user_r_read_32_rden  ),  // input wire rd_en
//  .rd_en         (  rden_High            ),  // input wire rd_en
  .dout          (  user_r_read_32_data  ),  // output wire [31 : 0] dout
  .full          (  full_async_fifo      ),  // output wire full
  .empty         (  user_r_read_32_empty ),  // output wire empty
  .rd_data_count (  rd_data_count        ),  // output wire [9 : 0] rd_data_count
  .wr_data_count (  wr_data_count        )   // output wire [9 : 0] wr_data_count
);

//**********************************************//
//*****  Reset Signal Generator (Temporal) *****//
//**********************************************//

(* mark_debug = "true" *) wire rst_pulse;
gen_user_reset gen_user_reset_ins(
    .CLK                    (  clk               ),
    .mem8_in_port_b         (  mem8_port_b_rdata ),
    .mem8_out_port_b        (  mem8_port_b_wdata ),
    .mem8_addr_port_b       (  mem8_port_b_addr  ),
    .mem8_access_en_port_b  (  mem8_port_b_rd_en ),
    .mem8_w_enable_port_b   (  mem8_port_b_wr_en ),
    .RST                    (  rst               ),   // 100MHz 5 clock cycles
    .RST_PULSE              (  rst_pulse         )    // 100MHz 1 clock cycle
);
     
     
//************************************************//
//*****  ADC Part :: Simulator  ******************//
//************************************************//
wire CNV1, CNV2;
wire SCK1, SCK2;
wire [6:0] CH_en1, CH_en2;
wire start1, start2;
wire LE1, LE2;
wire ADC_rst1, ADC_rst2;
wire [15:0] ADC_DO1, ADC_DO2;
wire ADC_SCK1, ADC_SCK2;
wire cnv_cnt_out1, cnv_cnt_out2;
wire SR_OUT1, SR_OUT2;
//wire [7:0] frame_count;
(* mark_debug = "true" *) wire [9:0] data_count1, data_count2;
//wire [15:0] ADC_FIFO_OUT;

ADC_test ADC_test_ins1(
    .sys_clk_adc     (  clk           ),
    .rst_n           ( !rst           ),
    .CNV             (  CNV1          ),
    .SCK             (  SCK1          ),
    .CH_en           (  CH_en1        ),
    .start           (  start1        ),
    .LE              (  LE1           ),
    .ADC_rst         (  ADC_rst1      ),
    .ADC_DO          (  ADC_DO1       ),
    .ADC_SCK         (  ADC_SCK1      ),
    .frame_count     (  frame_count1  ),
    .data_count      (  data_count1   ),
    .ADC_FIFO_OUT    (  ADC_FIFO_OUT1 )
//    .cnv_cnt_out     (  cnv_cnt_out1  ),
//    .SR_OUT_out      (  SR_OUT1       )
);

ADC_test ADC_test_ins2(
    .sys_clk_adc     (  clk           ),
    .rst_n           ( !rst           ),
    .CNV             (  CNV2          ),
    .SCK             (  SCK2          ),
    .CH_en           (  CH_en2        ),
    .start           (  start2        ),
    .LE              (  LE2           ),
    .ADC_rst         (  ADC_rst2      ),
    .ADC_DO          (  ADC_DO2       ),
    .ADC_SCK         (  ADC_SCK2      ),
    .frame_count     (  frame_count2  ),
    .data_count      (  data_count2   ),
    .ADC_FIFO_OUT    (  ADC_FIFO_OUT2 )
//    .cnv_cnt_out     (  cnv_cnt_out2  ),
//    .SR_OUT_out      (  SR_OUT2       )
);

//wire CNV;
//wire SCK;
//wire [6:0] CH_en;
//wire start;
//wire LE;
//wire ADC_rst;
//wire [15:0] ADC_DO;
//wire ADC_SCK;
//(* mark_debug = "true" *) wire [9:0] data_count;

//ADC_test ADC_test_ins(
//    .sys_clk_adc     (  clk          ),
//    .rst_n           ( !rst          ),
//    .CNV             (  CNV          ),
//    .SCK             (  SCK          ),
//    .CH_en           (  CH_en        ),
//    .start           (  start        ),
//    .LE              (  LE           ),
//    .ADC_rst         (  ADC_rst      ),
//    .ADC_DO          (  ADC_DO       ),
//    .ADC_SCK         (  ADC_SCK      ),
//    .frame_count     (  frame_count  ),
//    .data_count      (  data_count   ),
//    .ADC_FIFO_OUT    (  ADC_FIFO_OUT )
//);

//***********************************************//

     
endmodule   

  