module xillydemo
  (
  input  PCIE_PERST_B_LS,
  input  PCIE_REFCLK_N,
  input  PCIE_REFCLK_P,
  input [7:0] PCIE_RX_N,
  input [7:0] PCIE_RX_P,
  output [3:0] GPIO_LED,
  output [7:0] PCIE_TX_N,
  output [7:0] PCIE_TX_P
  );
  // Clock and quiesce
  wire  bus_clk;
  wire  quiesce;
  
  //...
   parameter do_in1=0;
   parameter do_in2=1;
   parameter do_in3=0;
   parameter do_in4=1;
   parameter do_in5=0;
   parameter do_in6=1;
   parameter do_in7=0;
   parameter do_in8=1;
   parameter do_in9=0;
   parameter do_in10=1;
   parameter do_in11=0;
   parameter do_in12=1;
   parameter do_in13=0;
   parameter do_in14=1;
   parameter do_in15=0;
   parameter do_in16=1;


  // Wires related to /dev/xillybus_mem_32
  wire  user_r_mem_32_rden;
  wire  user_r_mem_32_empty;
  wire [31:0] user_r_mem_32_data;
  wire  user_r_mem_32_eof;
  wire  user_r_mem_32_open;
  wire  user_w_mem_32_wren;
  wire  user_w_mem_32_full;
  wire [31:0] user_w_mem_32_data;
  wire  user_w_mem_32_open;
  wire [31:0] user_mem_32_addr;
  wire  user_mem_32_addr_update;

  // Wires related to /dev/xillybus_read_32
  wire  user_r_read_32_rden;
  wire  user_r_read_32_empty;
  wire [31:0] user_r_read_32_data;
  wire  user_r_read_32_eof;
  wire  user_r_read_32_open;

  // Wires related to /dev/xillybus_read_8
  wire  user_r_read_8_rden;
  wire  user_r_read_8_empty;
  wire [7:0] user_r_read_8_data;
  wire  user_r_read_8_eof;
  wire  user_r_read_8_open;

  // Wires related to /dev/xillybus_write_32
  wire  user_w_write_32_wren;
  wire  user_w_write_32_full;
  wire [31:0] user_w_write_32_data;
  wire  user_w_write_32_open;

  // Wires related to /dev/xillybus_write_8
  wire  user_w_write_8_wren;
  wire  user_w_write_8_full;
  wire [7:0] user_w_write_8_data;
  wire  user_w_write_8_open;
  
  wire [31:0]done;
  wire wr_wr;
  
   reg [15:0]count1=0;
   reg [15:0]count2=0;
   reg [31:0]data=0;
   reg [31:0]data1=0;
   reg [31:0]data2=0;
   reg [31:0]data3=0;
   reg [31:0]data4=0;
   reg [31:0]data5=0;
   reg [31:0]data6=0;
   reg [31:0]data7=0;
   reg [31:0]data8=0;
   reg [63:0]tim=0;
   reg wr;
   
 
   xillybus xillybus_ins (
 
     // Ports related to /dev/xillybus_mem_32
     // FPGA to CPU signals:
     .user_r_mem_32_rden(user_r_mem_32_rden),
     .user_r_mem_32_empty(user_r_mem_32_empty),
     .user_r_mem_32_data(user_r_mem_32_data),
     .user_r_mem_32_eof(user_r_mem_32_eof),
     .user_r_mem_32_open(user_r_mem_32_open),
 
     // CPU to FPGA signals:
     .user_w_mem_32_wren(user_w_mem_32_wren),
     .user_w_mem_32_full(user_w_mem_32_full),
     .user_w_mem_32_data(user_w_mem_32_data),
     .user_w_mem_32_open(user_w_mem_32_open),
 
     // Address signals:
     .user_mem_32_addr(user_mem_32_addr),
     .user_mem_32_addr_update(user_mem_32_addr_update),
 
 
     // Ports related to /dev/xillybus_read_32
     // FPGA to CPU signals:
     .user_r_read_32_rden(user_r_read_32_rden),
     .user_r_read_32_empty(user_r_read_32_empty),
     .user_r_read_32_data(user_r_read_32_data),
     .user_r_read_32_eof(user_r_read_32_eof),
     .user_r_read_32_open(user_r_read_32_open),
 
 
     // Ports related to /dev/xillybus_read_8
     // FPGA to CPU signals:
     .user_r_read_8_rden(user_r_read_8_rden),
     .user_r_read_8_empty(user_r_read_8_empty),
     .user_r_read_8_data(user_r_read_8_data),
     .user_r_read_8_eof(user_r_read_8_eof),
     .user_r_read_8_open(user_r_read_8_open),
 
 
     // Ports related to /dev/xillybus_write_32
     // CPU to FPGA signals:
     .user_w_write_32_wren(user_w_write_32_wren),
     .user_w_write_32_full(user_w_write_32_full),
     .user_w_write_32_data(user_w_write_32_data),
     .user_w_write_32_open(user_w_write_32_open),
 
 
     // Ports related to /dev/xillybus_write_8
     // CPU to FPGA signals:
     .user_w_write_8_wren(user_w_write_8_wren),
     .user_w_write_8_full(user_w_write_8_full),
     .user_w_write_8_data(user_w_write_8_data),
     .user_w_write_8_open(user_w_write_8_open),
 
 
     // General signals
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
  
 
   /*A simple inferred RAM
   always @(posedge bus_clk)
     begin
    if (user_w_mem_32_wren)
      demoarray[user_mem_32_addr] <= user_w_mem_32_data;
    
    if (user_r_mem_32_rden)
      user_r_mem_32_data <= demoarray[user_mem_32_addr];      
     end
 
   assign  user_r_mem_32_empty = 0;
   assign  user_r_mem_32_eof = 0;
   assign  user_w_mem_32_full = 0;*/
   always @(posedge bus_clk)
     begin
            case(count1)////////Deserialize  chuan/bing /////////pixel  1
                  0:  data1[15] <= do_in1;
                  1:  data1[14] <= do_in1;
                  2:  data1[13] <= do_in1;
                  3:  data1[12] <= do_in1;
                  4:  data1[11] <= do_in1;
                  5:  data1[10] <= do_in1;
                  6:  data1[9]  <= do_in1;
                  7:  data1[8]  <= do_in1;
                  8:  data1[7]  <= do_in1;
                  9:  data1[6]  <= do_in1;
                  10: data1[5]  <= do_in1;
                  11: data1[4]  <= do_in1;
                  12: data1[3]  <= do_in1;
                  13: data1[2]  <= do_in1;
                  14: data1[1]  <= 0;
                  15: data1[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  2
            
                  0:  data1[31] <= do_in2;
                  1:  data1[30] <= do_in2;
                  2:  data1[29] <= do_in2;
                  3:  data1[28] <= do_in2;
                  4:  data1[27] <= do_in2;
                  5:  data1[26] <= do_in2;
                  6:  data1[25]  <= do_in2;
                  7:  data1[24]  <= do_in2;
                  8:  data1[23]  <= do_in2;
                  9:  data1[22]  <= do_in2;
                  10: data1[21]  <= do_in2;
                  11: data1[20]  <= do_in2;
                  12: data1[19]  <= do_in2;
                  13: data1[18]  <= do_in2;
                  14: data1[17]  <= 0;
                  15: data1[16]  <= 0;
            endcase
            case(count1)///////////////pixel  3
                  0:  data2[15] <= do_in3;
                  1:  data2[14] <= do_in3;
                  2:  data2[13] <= do_in3;
                  3:  data2[12] <= do_in3;
                  4:  data2[11] <= do_in3;
                  5:  data2[10] <= do_in3;
                  6:  data2[9]  <= do_in3;
                  7:  data2[8]  <= do_in3;
                  8:  data2[7]  <= do_in3;
                  9:  data2[6]  <= do_in3;
                  10: data2[5]  <= do_in3;
                  11: data2[4]  <= do_in3;
                  12: data2[3]  <= do_in3;
                  13: data2[2]  <= do_in3;
                  14: data2[1]  <= 0;
                  15: data2[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  4
            
                  0:  data2[31] <= do_in4;
                  1:  data2[30] <= do_in4;
                  2:  data2[29] <= do_in4;
                  3:  data2[28] <= do_in4;
                  4:  data2[27] <= do_in4;
                  5:  data2[26] <= do_in4;
                  6:  data2[25]  <= do_in4;
                  7:  data2[24]  <= do_in4;
                  8:  data2[23]  <= do_in4;
                  9:  data2[22]  <= do_in4;
                  10: data2[21]  <= do_in4;
                  11: data2[20]  <= do_in4;
                  12: data2[19]  <= do_in4;
                  13: data2[18]  <= do_in4;
                  14: data2[17]  <= 0;
                  15: data2[16]  <= 0;
            endcase
            case(count1)///////////////pixel  5
                  0:  data3[15] <= do_in5;
                  1:  data3[14] <= do_in5;
                  2:  data3[13] <= do_in5;
                  3:  data3[12] <= do_in5;
                  4:  data3[11] <= do_in5;
                  5:  data3[10] <= do_in5;
                  6:  data3[9]  <= do_in5;
                  7:  data3[8]  <= do_in5;
                  8:  data3[7]  <= do_in5;
                  9:  data3[6]  <= do_in5;
                  10: data3[5]  <= do_in5;
                  11: data3[4]  <= do_in5;
                  12: data3[3]  <= do_in5;
                  13: data3[2]  <= do_in5;
                  14: data3[1]  <= 0;
                  15: data3[0]  <= 0;
            endcase         
            case(count1)//////////Deserialize  chuan/bing ///////pixel  6
            
                  0:  data3[31] <= do_in6;
                  1:  data3[30] <= do_in6;
                  2:  data3[29] <= do_in6;
                  3:  data3[28] <= do_in6;
                  4:  data3[27] <= do_in6;
                  5:  data3[26] <= do_in6;
                  6:  data3[25]  <= do_in6;
                  7:  data3[24]  <= do_in6;
                  8:  data3[23]  <= do_in6;
                  9:  data3[22]  <= do_in6;
                  10: data3[21]  <= do_in6;
                  11: data3[20]  <= do_in6;
                  12: data3[19]  <= do_in6;
                  13: data3[18]  <= do_in6;
                  14: data3[17]  <= 0;
                  15: data3[16]  <= 0;
            endcase
            case(count1)///////////////pixel  7
                  0:  data4[15] <= do_in7;
                  1:  data4[14] <= do_in7;
                  2:  data4[13] <= do_in7;
                  3:  data4[12] <= do_in7;
                  4:  data4[11] <= do_in7;
                  5:  data4[10] <= do_in7;
                  6:  data4[9]  <= do_in7;
                  7:  data4[8]  <= do_in7;
                  8:  data4[7]  <= do_in7;
                  9:  data4[6]  <= do_in7;
                  10: data4[5]  <= do_in7;
                  11: data4[4]  <= do_in7;
                  12: data4[3]  <= do_in7;
                  13: data4[2]  <= do_in7;
                  14: data4[1]  <= 0;
                  15: data4[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  8
            
                  0:  data4[31] <= do_in8;
                  1:  data4[30] <= do_in8;
                  2:  data4[29] <= do_in8;
                  3:  data4[28] <= do_in8;
                  4:  data4[27] <= do_in8;
                  5:  data4[26] <= do_in8;
                  6:  data4[25]  <= do_in8;
                  7:  data4[24]  <= do_in8;
                  8:  data4[23]  <= do_in8;
                  9:  data4[22]  <= do_in8;
                  10: data4[21]  <= do_in8;
                  11: data4[20]  <= do_in8;
                  12: data4[19]  <= do_in8;
                  13: data4[18]  <= do_in8;
                  14: data4[17]  <= 0;
                  15: data4[16]  <= 0;
            endcase
            case(count1)///////////////pixel  9
                  0:  data5[15] <= do_in9;
                  1:  data5[14] <= do_in9;
                  2:  data5[13] <= do_in9;
                  3:  data5[12] <= do_in9;
                  4:  data5[11] <= do_in9;
                  5:  data5[10] <= do_in9;
                  6:  data5[9]  <= do_in9;
                  7:  data5[8]  <= do_in9;
                  8:  data5[7]  <= do_in9;
                  9:  data5[6]  <= do_in9;
                  10: data5[5]  <= do_in9;
                  11: data5[4]  <= do_in9;
                  12: data5[3]  <= do_in9;
                  13: data5[2]  <= do_in9;
                  14: data5[1]  <= 0;
                  15: data5[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  10
            
                  0:  data5[31] <= do_in10;
                  1:  data5[30] <= do_in10;
                  2:  data5[29] <= do_in10;
                  3:  data5[28] <= do_in10;
                  4:  data5[27] <= do_in10;
                  5:  data5[26] <= do_in10;
                  6:  data5[25]  <= do_in10;
                  7:  data5[24]  <= do_in10;
                  8:  data5[23]  <= do_in10;
                  9:  data5[22]  <= do_in10;
                  10: data5[21]  <= do_in10;
                  11: data5[20]  <= do_in10;
                  12: data5[19]  <= do_in10;
                  13: data5[18]  <= do_in10;
                  14: data5[17]  <= 0;
                  15: data5[16]  <= 0;
            endcase
            case(count1)///////////////pixel  11
                  0:  data6[15] <= do_in11;
                  1:  data6[14] <= do_in11;
                  2:  data6[13] <= do_in11;
                  3:  data6[12] <= do_in11;
                  4:  data6[11] <= do_in11;
                  5:  data6[10] <= do_in11;
                  6:  data6[9]  <= do_in11;
                  7:  data6[8]  <= do_in11;
                  8:  data6[7]  <= do_in11;
                  9:  data6[6]  <= do_in11;
                  10: data6[5]  <= do_in11;
                  11: data6[4]  <= do_in11;
                  12: data6[3]  <= do_in11;
                  13: data6[2]  <= do_in11;
                  14: data6[1]  <= 0;
                  15: data6[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  12
            
                  0:  data6[31] <= do_in12;
                  1:  data6[30] <= do_in12;
                  2:  data6[29] <= do_in12;
                  3:  data6[28] <= do_in12;
                  4:  data6[27] <= do_in12;
                  5:  data6[26] <= do_in12;
                  6:  data6[25]  <= do_in12;
                  7:  data6[24]  <= do_in12;
                  8:  data6[23]  <= do_in12;
                  9:  data6[22]  <= do_in12;
                  10: data6[21]  <= do_in12;
                  11: data6[20]  <= do_in12;
                  12: data6[19]  <= do_in12;
                  13: data6[18]  <= do_in12;
                  14: data6[17]  <= 0;
                  15: data6[16]  <= 0;
            endcase
            case(count1)///////////////pixel  13
                  0:  data7[15] <= do_in13;
                  1:  data7[14] <= do_in13;
                  2:  data7[13] <= do_in13;
                  3:  data7[12] <= do_in13;
                  4:  data7[11] <= do_in13;
                  5:  data7[10] <= do_in13;
                  6:  data7[9]  <= do_in13;
                  7:  data7[8]  <= do_in13;
                  8:  data7[7]  <= do_in13;
                  9:  data7[6]  <= do_in13;
                  10: data7[5]  <= do_in13;
                  11: data7[4]  <= do_in13;
                  12: data7[3]  <= do_in13;
                  13: data7[2]  <= do_in13;
                  14: data7[1]  <= 0;
                  15: data7[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  14
            
                  0:  data7[31] <= do_in14;
                  1:  data7[30] <= do_in14;
                  2:  data7[29] <= do_in14;
                  3:  data7[28] <= do_in14;
                  4:  data7[27] <= do_in14;
                  5:  data7[26] <= do_in14;
                  6:  data7[25]  <= do_in14;
                  7:  data7[24]  <= do_in14;
                  8:  data7[23]  <= do_in14;
                  9:  data7[22]  <= do_in14;
                  10: data7[21]  <= do_in14;
                  11: data7[20]  <= do_in14;
                  12: data7[19]  <= do_in14;
                  13: data7[18]  <= do_in14;
                  14: data7[17]  <= 0;
                  15: data7[16]  <= 0;
            endcase
            case(count1)///////////////pixel  15
                  0:  data8[15] <= do_in15;
                  1:  data8[14] <= do_in15;
                  2:  data8[13] <= do_in15;
                  3:  data8[12] <= do_in15;
                  4:  data8[11] <= do_in15;
                  5:  data8[10] <= do_in15;
                  6:  data8[9]  <= do_in15;
                  7:  data8[8]  <= do_in15;
                  8:  data8[7]  <= do_in15;
                  9:  data8[6]  <= do_in15;
                  10: data8[5]  <= do_in15;
                  11: data8[4]  <= do_in15;
                  12: data8[3]  <= do_in15;
                  13: data8[2]  <= do_in15;
                  14: data8[1]  <= 0;
                  15: data8[0]  <= 0;
            endcase
            case(count1)//////////Deserialize  chuan/bing ///////pixel  16
            
                  0:  data8[31] <= do_in16;
                  1:  data8[30] <= do_in16;
                  2:  data8[29] <= do_in16;
                  3:  data8[28] <= do_in16;
                  4:  data8[27] <= do_in16;
                  5:  data8[26] <= do_in16;
                  6:  data8[25]  <= do_in16;
                  7:  data8[24]  <= do_in16;
                  8:  data8[23]  <= do_in16;
                  9:  data8[22]  <= do_in16;
                  10: data8[21]  <= do_in16;
                  11: data8[20]  <= do_in16;
                  12: data8[19]  <= do_in16;
                  13: data8[18]  <= do_in16;
                  14: data8[17]  <= 0;
                  15: data8[16]  <= 0;
            endcase
       end      
    
 //data     
  always @(posedge bus_clk)
   begin
            begin
             count1<=count1+1;  
             count2<=count2+1;  
             end
         if(count2==30)
              begin
                wr<=1;
                data[31:0]<=32'b00000000000000000000000000000000;
              end
         else if(count2==36)
               begin
                 wr<=1;
                data[31:0]<=tim[31:0];
               end
         else if(count1==70)
               begin
                  wr<=1;
                  data<=32'b00000000000000000000000000000000;
                end               
         else if(count1==72)
               begin
                  wr<=1;
                  data<=data1;
               end
 
         else if(count1==74)
               begin
                 wr<=1;
                 data<=data2;
               end
         else if(count1==76)
               begin
                   wr<=1;
                   data<=data3;
                end 
         else if(count1==78)
               begin
                  wr<=1;
                  data<=data4;
                end
         else if(count1==80)
               begin
                  wr<=1;
                 data<=data5;
               end
        else if(count1==82)
               begin
                  wr<=1;
                  data<=data6;
                end
         else if(count1==84)
                begin
                   wr<=1;
                   data<=data7;
                 end   
        else if(count1==86)
                 begin
                   wr<=1;
                   data<=data8;
                 end
        else if(count1==88)
                begin
                   wr<=1;
                   data<=32'b00000000000000000000000000000000;
                end                 
         else if(count1==124)
                 begin
                 count1<=0;
                 end
         else if(count2==5999)
                    begin
                     tim<=tim+1;
                     wr<=1;
                     data<=32'b11111111111111111111111111111111;                     
                     count2<=0;                    
                    end
           else
                     wr<=0;
    end
   assign done = data;
   assign wr_wr = wr;  

   // 32-bit loopback
   fifo_32x512 fifo_32
     (
      .clk(bus_clk),
      .srst(!user_r_read_32_open),
      .din(done),
      .wr_en(wr_wr),
      .rd_en(user_r_read_32_rden),
      .dout(user_r_read_32_data),
      .full(user_w_write_32_full),
      .empty(user_r_read_32_empty)
      );

   assign  user_r_read_32_eof = 0;
   
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

   assign  user_r_read_8_eof = 0;
   
endmodule
