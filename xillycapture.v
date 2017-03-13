module xillycapture
  (
   input  PCIE_PERST_B_LS,
   input  PCIE_REFCLK_N,
   input  PCIE_REFCLK_P,
   input [3:0] PCIE_RX_N,
   input [3:0] PCIE_RX_P,
   output [3:0] GPIO_LED,
   output [3:0] PCIE_TX_N,
   output [3:0] PCIE_TX_P
   );
   // Clock and quiesce
   wire 	bus_clk;
   wire 	quiesce;
   
   // Memory array
   reg [7:0] 	demoarray[0:31];

   // Wires related to /dev/xillybus_mem_8
   wire       user_r_mem_8_rden;
   wire       user_r_mem_8_empty;
   reg [7:0]  user_r_mem_8_data;
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

   // Wires and registers related to data capturing
   wire        capture_clk;
   reg [31:0]  capture_data;
   wire        capture_en;
   reg [4:0]   slowdown;
   wire        capture_full;

   reg 	       capture_open;
   reg 	       capture_open_cross;
   reg 	       capture_has_been_full;
   reg 	       capture_has_been_nonfull;
   reg 	       has_been_full_cross;
   reg 	       has_been_full;
	     
   
   xillybus xillybus_ins (

			  // Ports related to /dev/xillybus_mem_8
			  // FPGA to CPU signals:
			  .user_r_mem_8_rden(user_r_mem_8_rden),
			  .user_r_mem_8_empty(user_r_mem_8_empty),
			  .user_r_mem_8_data(user_r_mem_8_data),
			  .user_r_mem_8_eof(user_r_mem_8_eof),
			  .user_r_mem_8_open(user_r_mem_8_open),

			  // CPU to FPGA signals:
			  .user_w_mem_8_wren(user_w_mem_8_wren),
			  .user_w_mem_8_full(user_w_mem_8_full),
			  .user_w_mem_8_data(user_w_mem_8_data),
			  .user_w_mem_8_open(user_w_mem_8_open),

			  // Address signals:
			  .user_mem_8_addr(user_mem_8_addr),
			  .user_mem_8_addr_update(user_mem_8_addr_update),


			  // Ports related to /dev/xillybus_read_32
			  // FPGA to CPU signals:
			  .user_r_read_32_rden(user_r_read_32_rden),
			  .user_r_read_32_empty(user_r_read_32_empty),
			  .user_r_read_32_data(user_r_read_32_data),
			  .user_r_read_32_eof(user_r_read_32_eof),
			  .user_r_read_32_open(user_r_read_32_open),

			  // Ports related to /dev/xillybus_write_32
			  // CPU to FPGA signals:
			  .user_w_write_32_wren(user_w_write_32_wren),
			  .user_w_write_32_full(user_w_write_32_full),
			  .user_w_write_32_data(user_w_write_32_data),
			  .user_w_write_32_open(user_w_write_32_open),

			  // Ports related to /dev/xillybus_read_8
			  // FPGA to CPU signals:
			  .user_r_read_8_rden(user_r_read_8_rden),
			  .user_r_read_8_empty(user_r_read_8_empty),
			  .user_r_read_8_data(user_r_read_8_data),
			  .user_r_read_8_eof(user_r_read_8_eof),
			  .user_r_read_8_open(user_r_read_8_open),

			  // Ports related to /dev/xillybus_write_8
			  // CPU to FPGA signals:
			  .user_w_write_8_wren(user_w_write_8_wren),
			  .user_w_write_8_full(user_w_write_8_full),
			  .user_w_write_8_data(user_w_write_8_data),
			  .user_w_write_8_open(user_w_write_8_open),


			  // Signals to top level
			  .PCIE_PERST_B_LS(PCIE_PERST_B_LS),
			  .PCIE_REFCLK_N(PCIE_REFCLK_N),
			  .PCIE_REFCLK_P(PCIE_REFCLK_P),
			  .PCIE_RX_N(PCIE_RX_N),
			  .PCIE_RX_P(PCIE_RX_P),
			  .GPIO_LED(GPIO_LED),
			  .PCIE_TX_N(PCIE_TX_N),
			  .PCIE_TX_P(PCIE_TX_P),
			  .bus_clk(bus_clk),
			  .quiesce(quiesce)
			  );
   
   // Data capture section
   // ====================

   always @(posedge capture_clk)
     begin
	if (capture_en)
	  capture_data <= capture_data + 1; // Bogus data source
	
	// The slowdown register limits the data pace to 1/32 the bus_clk
	// when capture_clk = bus_clk. This is necessary, because the
	// core in the evaluation kit is configured for simplicity, and
	// not for performance. Sustained data rates of 200 MB/sec are
	// easily reached with performance-oriented setting.
	// The slowdown register has no function in a real-life application.
	slowdown <= slowdown + 1;

	// capture_has_been_full remembers that the FIFO has been full
	// until the file is closed. capture_has_been_nonfull prevents
	// capture_has_been_full to respond to the initial full condition
	// every FIFO displays on reset.

	if (!capture_full)
	  capture_has_been_nonfull <= 1;
	else if (!capture_open)
	  capture_has_been_nonfull <= 0;
	
	if (capture_full && capture_has_been_nonfull)
	  capture_has_been_full <= 1;
	else if (!capture_open)
	  capture_has_been_full <= 0;
	
     end

   // The dependency on slowdown is only for bogus data
   assign capture_en = capture_open && !capture_full && 
		       !capture_has_been_full &&
		       (slowdown == 0);
      
   // Clock crossing logic: bus_clk -> capture_clk
   always @(posedge capture_clk)
     begin
	capture_open_cross <= user_r_read_32_open;
	capture_open <= capture_open_cross;
     end

   // Clock crossing logic: capture_clk -> bus_clk
   always @(posedge bus_clk)
     begin
	has_been_full_cross <= capture_has_been_full;
	has_been_full <= has_been_full_cross;
     end
   
   // The user_r_read_32_eof signal is required to go from '0' to '1' only on
   // a clock cycle following an asserted read enable, according to Xillybus'
   // core API. This is assured, since it's a logical AND between
   // user_r_read_32_empty and has_been_full. has_been_full goes high when the
   // FIFO is full, so it's guaranteed that user_r_read_32_empty is low when
   // that happens. On the other hand, user_r_read_32_empty is a FIFO's empty
   // signal, which naturally meets the requirement.
   
   assign user_r_read_32_eof = user_r_read_32_empty && has_been_full;
   assign user_w_write_32_full = 0;
   
   // The data capture clock here is bus_clk for simplicity, but clock domain
   // crossing is done properly, so capture_clk can be an independent clock
   // without any other changes.
   
   assign capture_clk = bus_clk;
   
   async_fifo_32 fifo_32 
     (
      .rst(!user_r_read_32_open),
      .wr_clk(capture_clk),
      .rd_clk(bus_clk),
      .din(capture_data),
      .wr_en(capture_en),
      .rd_en(user_r_read_32_rden),
      .dout(user_r_read_32_data),
      .full(capture_full),
      .empty(user_r_read_32_empty)
      );

   // Everything below is copied from xillydemo.v
   
   // A simple inferred RAM
   always @(posedge bus_clk)
     begin
	if (user_w_mem_8_wren)
	  demoarray[user_mem_8_addr] <= user_w_mem_8_data;
	
	if (user_r_mem_8_rden)
	  user_r_mem_8_data <= demoarray[user_mem_8_addr];	  
     end

   assign  user_r_mem_8_empty = 0;
   assign  user_r_mem_8_eof = 0;
   assign  user_w_mem_8_full = 0;
   
   // Data from the write_32 is discarded. Ignore wren and data, and
   // never assert full.
   assign user_w_write_32_full = 0;

   // 8-bit loopback
   fifo_8x2048 fifo_8
     (
      .clk(bus_clk),
      .srst(!user_w_write_8_open && !user_r_read_8_open),
      .din(user_w_write_8_data),
      .wr_en(user_w_write_8_wren),
      .rd_en(user_r_read_8_rden),
      .dout(user_r_read_8_data),
      .full(user_w_write_8_full),
      .empty(user_r_read_8_empty)
      );

   assign user_r_read_8_eof = 0;
endmodule
